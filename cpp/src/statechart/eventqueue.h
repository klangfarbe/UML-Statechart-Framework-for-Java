/*
 * UML statechart framework (http://uml-statecharts.sourceforge.net)
 *
 * Copyright (C) 2006-2007 Christian Mocek (klangfarbe@users.sourceforge.net)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */
#ifndef EVENTQUEUE
#define EVENTQUEUE

#include <list>
#include <set>
#include <vector>

#include <statechart/event.h>
#include <statechart/parameter.h>
#include <statechart/util/thread.h>

namespace statechart {

/**
 * An asynchronous fifo event queue of the statechart. All
 * events added in this queue will be dispatched via a bunch of
 * helper threads. An incoming event and its parameter is
 * deleted after it is dispatched. So all events and parameters
 * dispatched asynchronous must be created on the heap.
 */
  class EventQueue : public util::Thread {
//------------------------------------------------------------------------------
// Event Queue Task
//------------------------------------------------------------------------------
  protected:
    class EventQueueTask {
    public:
      /**
       * Creates a task for the thread pool
       */
      EventQueueTask(util::SmartPointer<Metadata> _data, 
                     util::SmartPointer<Event> _event, 
                     util::SmartPointer<Parameter> _parameter) 
      : event(_event), data(_data), parameter(_parameter) {
      };

      /**
       * Frees the memory allocated for the event and parameter
       */
      virtual ~EventQueueTask() {
      };

    public:
      util::SmartPointer<Event> event;
      util::SmartPointer<Metadata> data;
      util::SmartPointer<Parameter> parameter;
    };

//------------------------------------------------------------------------------
// Event Queue Worker Thread
//------------------------------------------------------------------------------
  protected:
    class EventQueueWorkerThread : public util::Thread {
    private:
      util::Mutex taskMutex;
      util::Condition taskCondition;
      EventQueue* eventQueue;
      EventQueueTask* task;

    public:
      EventQueueWorkerThread(EventQueue* eventQueue);
      void execute(EventQueueTask* task);
      virtual void run();
    };

//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
  public:
    /**
     * Creates a new event queue with the given number of helper threads for
     * dispatching an event.
     */
    EventQueue(Statechart* statechart, unsigned int numberOfWorkerThreads);

    /**
     * Default destructor
     */
    virtual ~EventQueue();

    /**
     * Dispateches an event from the event queue and delegates it to a
     * worker thread.
     */
    void run();

  protected:

    /**
     * Adds an event to the queue. Event and Parameter will be
     * deleted after dispatching.
     */
    virtual void add(util::SmartPointer<Metadata>& data, 
                     util::SmartPointer<Event>& event, 
                     util::SmartPointer<Parameter>& parameter);

    /**
     * Releases the given Metadata so the next event for this
     * specific data can be dispatched. 
     */
    void release(util::SmartPointer<Metadata>& data);

    /**
     * Releases a worker for a new task
     */
    void release(EventQueueWorkerThread* thread);

    /**
     * Returns the next event from the event list. Ovverride it to
     * specify the behaviour.
     */
    virtual EventQueueTask* getNextTask();

//------------------------------------------------------------------------------
// Attributes
//------------------------------------------------------------------------------
  protected:
    /// Reference to the statechart this event queue belongs to.
    Statechart* statechart;

    /// The list of enqueued events
    std::list<EventQueueTask*> eventList;

    /// List containing all available workers
    std::list<EventQueueWorkerThread*> availableWorkers;

    /**
     * Temporary container where metadata objects are stored to make
     * sure only one event for each data is dispatched at a time.
     */
    std::list<util::SmartPointer<Metadata> > dataSet;

    /// Mutex to lock the dataSet
    util::Mutex dataSetMutex;

    /// Mutex to lock the eventList
    util::Mutex eventListMutex;

    /// Mutex to lock the list of available workers
    util::Mutex availableWorkersMutex;

    /// Trigger for reacting when an event has added
    util::Condition eventListCondition;

    /// Trigger for reacting when an worker ws released
    util::Condition availableWorkersCondition;

    /// Number of threads to spawn
    unsigned int numberOfWorkerThreads;

    friend class Statechart;
    friend class EventQueueWorkerThread;
  };
}

#endif

