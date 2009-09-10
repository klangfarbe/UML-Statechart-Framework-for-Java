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
#ifndef TIMEOUTEVENTQUEUE
#define TIMEOUTEVENTQUEUE

#include "eventqueue.h"
#include "state.h"
#include "timeoutevent.h"

namespace statechart {

/**
 * Implements a timeout queue thread. All timeout events created
 * by a state are triggered though this class.
 */
  class TimeoutEventQueue : public EventQueue {
//------------------------------------------------------------------------------
// TimeoutEvent Queue Task
//------------------------------------------------------------------------------
  protected:
    class TimeoutEventQueueTask : public EventQueue::EventQueueTask {
    public:
      /**
       * Creates a task for the thread pool
       */
      TimeoutEventQueueTask(State* _source, 
                            util::SmartPointer<Metadata>& data, 
                            util::SmartPointer<Event>& event, 
                            util::SmartPointer<Parameter>& parameter) 
      : EventQueueTask(data, event, parameter), source(_source) {
        assert(source);
      };

    public:
      /// The source state which triggered the timeout event
      State* source;
    };

//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
  public:
    /**
     * Default constructor
     */
    TimeoutEventQueue(Statechart* statechart, unsigned int numberOfWorkerThreads);

    virtual void add(util::SmartPointer<Metadata>& data, 
                     util::SmartPointer<Event>& event, 
                     util::SmartPointer<Parameter>& parameter) {};
    virtual void add(State* state, 
                     util::SmartPointer<Metadata>& data, 
                     util::SmartPointer<Event>& event, 
                     util::SmartPointer<Parameter>& parameter);
    virtual void remove(State* state, util::SmartPointer<Metadata>& data);
    EventQueueTask* getNextTask();
  };
}
#endif

