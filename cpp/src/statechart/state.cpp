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
#include <statechart/state.h>
#include <statechart/context.h>
#include <statechart/statechart.h>
#include <statechart/timeoutevent.h>
#include <statechart/statechartexception.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
using namespace statechart;
using namespace statechart::util;

//------------------------------------------------------------------------------

State::State(Context* parent, Action* entryAction, Action* doAction, 
             Action* exitAction, const char* name) {
  this->context = parent;
  this->statechart = 0;

  // get the statechart, we need it for the event queues
  if(parent) {
    parent->addSubstate(this);
    while(parent->context) {
      parent = parent->context;
    }
    statechart = dynamic_cast<Statechart*>(parent);
    if(!statechart) {
      throw new StatechartException("Cannot determine path to the statechart. Check the hierarchy.");
    }
  }

  this->entryAction = entryAction;
  this->doAction = doAction;
  this->exitAction = exitAction;
  this->name = 0;
  setName(name);
}

//---------------------------------------------------------------------------

State::~State() {
  if(name) {
    free(name);
  }

  delete entryAction;
  delete exitAction;
  delete doAction;

  // delete Transisions
  for(unsigned int i = 0; i < transitions.size(); i++) {
    delete transitions[i];
    transitions[i] = 0;
  }
  transitions.clear();
}

//---------------------------------------------------------------------------

void State::setEntryAction(Action* action) {
  delete entryAction;
  entryAction = action;
}

//---------------------------------------------------------------------------

void State::setDoAction(Action* action) {
  delete doAction;
  doAction = action;
}

//---------------------------------------------------------------------------

void State::setExitAction(Action* action) {
  delete exitAction;
  exitAction = action;
}

//---------------------------------------------------------------------------

bool State::activate(SmartPointer<Metadata>& data, 
                     SmartPointer<Parameter>& parameter) {
  if(!data->isActive(this)) {
    data->activate(this);

    // trigger the timout events if available
    unsigned long time = data->getData(this)->timestamp;
    for(unsigned int i = 0; i < transitions.size(); i++) {
      Transition* t = dynamic_cast<Transition*>(transitions[i]);
      if(t->hasEvent()) {
        TimeoutEvent* event = dynamic_cast<TimeoutEvent*>(t->event);
        if(event) {
          // Add an event with it's absolute time (stateactivation time + timeout)
          SmartPointer<Event> e(new TimeoutEvent(time + event->getTimeout()));
          statechart->timeoutEventQueue.add(this, data, e, parameter);
        }
      }
    }

    if(entryAction) {
      entryAction->execute(data, parameter);
    }

    if(doAction) {
      doAction->execute(data, parameter);
    }
    return true;
  }
  return false;
}

//---------------------------------------------------------------------------

void State::deactivate(SmartPointer<Metadata>& data, 
                       SmartPointer<Parameter>& parameter) {
  if(data->isActive(this)) {
    data->deactivate(this);
    if(exitAction) {
      exitAction->execute(data, parameter);
    }
  }
  statechart->timeoutEventQueue.remove(this, data);
}

//---------------------------------------------------------------------------

bool State::dispatch(SmartPointer<Metadata>& data, 
                     SmartPointer<Event>& event, 
                     SmartPointer<Parameter>& parameter) {
  for(unsigned int i = 0; i < transitions.size(); i++) {
    if(transitions[i]->execute(event, data, parameter)) {
      return true;
    }
  }
  return false;
}

//---------------------------------------------------------------------------

void State::addTransition(Transition* transition) {
  // make sure transition with guards are checked first!
  if(transition->hasGuard()) {
    transitions.insert(transitions.begin(), transition);
  } else {
    transitions.push_back(transition);
  }
}

//---------------------------------------------------------------------------

void State::setName(const char* _name) {
  if(this->name) {
    free(this->name);
    this->name = 0;
  }

  if(_name) {
    this->name = strdup(_name);
  }
}
