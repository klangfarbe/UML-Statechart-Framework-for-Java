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
#include <statechart/eventqueue.h>
#include <statechart/statechart.h>

using namespace std;
using namespace statechart;
using namespace statechart::util;

//------------------------------------------------------------------------------

EventQueue::EventQueueWorkerThread::EventQueueWorkerThread(EventQueue* _eventQueue) 
: taskCondition(taskMutex), eventQueue(_eventQueue), task(0) {
}

//------------------------------------------------------------------------------

void EventQueue::EventQueueWorkerThread::execute(EventQueueTask* task) {
  ScopeMutex taskGuard(taskMutex);
  this->task = task;
  taskCondition.signal();
}

//------------------------------------------------------------------------------

void EventQueue::EventQueueWorkerThread::run() {
  while(isRunning()) {
    ScopeMutex taskGuard(taskMutex);
    while(!task && isRunning()) {
      taskCondition.wait();
    }

    if(task && isRunning()) {
      try {
        eventQueue->statechart->dispatch(task->data, task->event, task->parameter);
      } catch(...) {
      }

      EventQueueTask* t = dynamic_cast<EventQueueTask*>(task);

      eventQueue->release(t->data);
      delete task;
      task = 0;
    }
    eventQueue->release(this);
  }
}

//------------------------------------------------------------------------------

EventQueue::EventQueue(Statechart* _statechart, unsigned int _numberOfWorkerThreads) 
: statechart(_statechart), eventListCondition(eventListMutex), 
availableWorkersCondition(availableWorkersMutex), numberOfWorkerThreads(_numberOfWorkerThreads){
}

//------------------------------------------------------------------------------

EventQueue::~EventQueue() {
  for(list<EventQueueTask*>::iterator it = eventList.begin();
     it != eventList.end(); it++) {
    delete *it;
  }
}

//------------------------------------------------------------------------------

void EventQueue::add(SmartPointer<Metadata>& data, 
                     SmartPointer<Event>& event, 
                     SmartPointer<Parameter>& parameter) {
  ScopeMutex g(eventListMutex);
  eventList.push_back(new EventQueueTask(data, event, parameter));
  eventListCondition.signal();
}

//------------------------------------------------------------------------------

void EventQueue::release(SmartPointer<Metadata>& data) {
  ScopeMutex g(dataSetMutex);
  dataSet.remove(data);
}

//------------------------------------------------------------------------------

void EventQueue::release(EventQueueWorkerThread* thread) {
  ScopeMutex g(availableWorkersMutex);
  availableWorkers.push_back(thread);
  availableWorkersCondition.signal();
}

//------------------------------------------------------------------------------

inline EventQueue::EventQueueTask* EventQueue::getNextTask() {
  list<EventQueueTask*>::iterator eventIterator;
  for(eventIterator = eventList.begin(); 
     eventIterator != eventList.end(); eventIterator++) {
    bool found = false;
    // check if another event is currently dispatched for this data 
    ScopeMutex mutex(dataSetMutex);
    list<SmartPointer<Metadata> >::iterator it;
    for(it = dataSet.begin(); it != dataSet.end(); it++) {
      if((*eventIterator)->data == *it) {
        found = true;
        break;
      }
    }

    if(!found) {
      EventQueueTask* task = *eventIterator; 
      dataSet.push_back(task->data);
      eventList.erase(eventIterator);
      return task;
    }
  }
  return 0;
}

//------------------------------------------------------------------------------

void EventQueue::run() {
  // create the worker threads
  vector<Thread*> threads;
  for(unsigned int i = 0; i < numberOfWorkerThreads; i++) {
    EventQueueWorkerThread* worker = new EventQueueWorkerThread(this);
    availableWorkers.push_back(worker);
    threads.push_back(worker);
    worker->start();
  }

  while(isRunning()) {
    EventQueueTask* task = 0;
    // Wait for in incoming event
    {      
      ScopeMutex eventListLock(eventListMutex);
      while(eventList.empty() && isRunning()) {
        eventListCondition.wait();
      }

      if(isRunning()) {
        // get the next event
        task = getNextTask();        
      }
    }

    // we got the next task. Wait for a worker thread to assign
    if(task) {
      ScopeMutex lock(availableWorkersMutex);
      while(availableWorkers.empty() && isRunning()) {
        availableWorkersCondition.wait();
      }

      if(isRunning()) {
        EventQueueWorkerThread* thread = availableWorkers.front();
        availableWorkers.pop_front();
        thread->execute(task);
      }
    } else {
      Thread::sleep(1);
    }
  }

  // shutdown all worker
  for(unsigned int i = 0; i < threads.size(); i++) {
    threads[i]->shutdown();
  }

  ScopeMutex lock(availableWorkersMutex);
  availableWorkers.clear();

  // Free the memory of the worker threads
  for(unsigned int i = 0; i < threads.size(); i++) {
    delete threads[i];
    threads[i] = 0;
  }
}
