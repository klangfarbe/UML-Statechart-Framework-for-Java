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
#include <statechart/statechart.h>
#include <statechart/timeoutevent.h>

using namespace statechart;
using namespace statechart::util;

//------------------------------------------------------------------------------

Statechart::Statechart(unsigned int numberOfWorkerThreads, 
                       unsigned int numberOfTimeoutWorkerThreads) 
: Context(0), eventQueue(this, numberOfWorkerThreads),
timeoutEventQueue(this, numberOfTimeoutWorkerThreads) {
  eventQueue.start();
  timeoutEventQueue.start();
}

//------------------------------------------------------------------------------

Statechart::~Statechart() {
  eventQueue.shutdown();
  timeoutEventQueue.shutdown();
}

//------------------------------------------------------------------------------

bool Statechart::start(SmartPointer<Metadata> data) {
  SmartPointer<Parameter> parameter;
  return start(data, parameter);
}

//------------------------------------------------------------------------------

bool Statechart::start(SmartPointer<Metadata> data, 
                       SmartPointer<Parameter> parameter) {
  data->lock();
  data->reset();
  data->activate(this);
  data->activate(startState);
  data->unlock();
  SmartPointer<Event> event;
  return dispatch(data, event, parameter);
}

//------------------------------------------------------------------------------

bool Statechart::dispatch(SmartPointer<Metadata> data, 
                          SmartPointer<Event> event) {
  SmartPointer<Parameter> parameter;
  return dispatch(data, event, parameter);
}

//------------------------------------------------------------------------------

bool Statechart::dispatch(SmartPointer<Metadata> data, 
                          SmartPointer<Event> event, 
                          SmartPointer<Parameter> parameter) {
  data->lock();
  State* currentState = data->getData(this)->currentState;
  bool rc = currentState->dispatch(data, event, parameter);

  // call dispatch as long as we hit states with end transitions
  SmartPointer<Event> ev;
  do {
    currentState = data->getData(this)->currentState;
  } while(currentState && currentState->dispatch(data, ev, parameter));

  data->unlock();
  return rc;
}

//------------------------------------------------------------------------------

void Statechart::startAsynchron(SmartPointer<Metadata> data) {
  SmartPointer<Parameter> parameter;
  startAsynchron(data, parameter);
}

//------------------------------------------------------------------------------

void Statechart::startAsynchron(SmartPointer<Metadata> data, 
                                SmartPointer<Parameter> parameter) {
  data->lock();
  data->reset();
  data->activate(this);
  data->activate(startState);
  data->unlock();
  SmartPointer<Event> event;
  dispatchAsynchron(data, event, parameter); 
}  

//------------------------------------------------------------------------------

void Statechart::dispatchAsynchron(SmartPointer<Metadata> data, 
                                   SmartPointer<Event> event) {
  SmartPointer<Parameter> parameter;
  dispatchAsynchron(data, event, parameter);
}

//------------------------------------------------------------------------------

void Statechart::dispatchAsynchron(SmartPointer<Metadata> data, 
                                   SmartPointer<Event> event, 
                                   SmartPointer<Parameter> parameter) {
  if(dynamic_cast<TimeoutEvent*>(spGetImpl(event))) {
    timeoutEventQueue.add(this, data, event, parameter);
  } else {
    eventQueue.add(data, event, parameter);
  }
}

