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
#include <statechart/timeouteventqueue.h>
#include <statechart/util/time.h>

using namespace std;
using namespace statechart;
using namespace statechart::util;

//------------------------------------------------------------------------------

TimeoutEventQueue::TimeoutEventQueue(Statechart* statechart, unsigned int numberOfWorkerThreads) 
: EventQueue(statechart, numberOfWorkerThreads) {
}

//------------------------------------------------------------------------------

void TimeoutEventQueue::add(State* state, 
                            SmartPointer<Metadata>& data, 
                            SmartPointer<Event>& event, 
                            SmartPointer<Parameter>& parameter) {
  TimeoutEvent* e = dynamic_cast<TimeoutEvent*>(spGetImpl(event));

  if(!e) {
    return;
  }

  ScopeMutex g(eventListMutex);
  list<EventQueueTask*>::iterator eventIterator;
  for(eventIterator = eventList.begin(); eventIterator != eventList.end(); eventIterator++) {
    TimeoutEvent* listevent = dynamic_cast<TimeoutEvent*>(spGetImpl((*eventIterator)->event));
    // check if the timeout of the listevent is smaller than the new event
    if(listevent->getTimeout() > e->getTimeout()) {
      break;
    }
  }
  eventList.insert(eventIterator, new TimeoutEventQueueTask(state, data, event, parameter));
  eventListCondition.signal();
}

//------------------------------------------------------------------------------

void TimeoutEventQueue::remove(State* state, SmartPointer<Metadata>& data) {
  ScopeMutex g(eventListMutex);
  for(list<EventQueueTask*>::iterator eventIterator = eventList.begin(); 
     eventIterator != eventList.end(); eventIterator++) {
    TimeoutEventQueueTask* task = dynamic_cast<TimeoutEventQueueTask*>(*eventIterator);
    if(task->source == state && task->data == data) {
      delete task;
      eventIterator = eventList.erase(eventIterator);
    }
  }
  eventListCondition.signal();
}

//------------------------------------------------------------------------------

EventQueue::EventQueueTask* TimeoutEventQueue::getNextTask() {
  // get first element from list and sleep for the time amount
  TimeoutEventQueueTask* task = dynamic_cast<TimeoutEventQueueTask*>(eventList.front());
  TimeoutEvent* event = dynamic_cast<TimeoutEvent*>(spGetImpl(task->event));

  long sleepTime = event->getTimeout() - Time::now();

  if(sleepTime > 0) {
    if(!eventListCondition.wait(sleepTime)) {
      return 0;
    }

    // test for spurious wakeup
    sleepTime = event->getTimeout() - Time::now();
    if(sleepTime > 0) {
      return 0;
    }
  }
  eventList.pop_front();
  return task;
}
