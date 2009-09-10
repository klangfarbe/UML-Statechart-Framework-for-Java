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
#include "concurrentstate.h"

using namespace statechart;
using namespace statechart::util;

//------------------------------------------------------------------------------

ConcurrentState::ConcurrentState(Context* parent, Action* entryAction, 
                                 Action* doAction, Action* exitAction, 
                                 char* name) 
  : Context(parent, entryAction, doAction, exitAction, name) {
}

//------------------------------------------------------------------------------

void ConcurrentState::addRegion(HierarchicalState* region) {
  regions.push_back(region);
}

//------------------------------------------------------------------------------

bool ConcurrentState::finished(SmartPointer<Metadata>& data) {
  for(unsigned int i = 0; i < regions.size(); i++) {
    HierarchicalState* h = dynamic_cast<HierarchicalState*>(regions[i]);
    if(h && data->isActive(h) 
       && !dynamic_cast<FinalState*>(data->getData(h)->currentState)) {
      return false;
    }
  }
  return true;
}

//------------------------------------------------------------------------------

bool ConcurrentState::activate(SmartPointer<Metadata>& data, 
                               SmartPointer<Parameter>& parameter) {
  if(Context::activate(data,parameter)) {
    StateRuntimedata* statedata = data->getData(this);
    SmartPointer<Event> event;
    vector<State*>::iterator p;
    for(unsigned int i = 0; i < regions.size(); i++) {
      // check if the region is activated implicit via a incoming transition
      p = find(statedata->stateset.begin(), statedata->stateset.end(), regions[i]);
      if (p == statedata->stateset.end()) {
        HierarchicalState* h = dynamic_cast<HierarchicalState*>(regions[i]);
        if(h->activate(data, parameter)) {
          h->dispatch(data, event, parameter);
        }
      }
    }
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------

void ConcurrentState::deactivate(SmartPointer<Metadata>& data, 
                                 SmartPointer<Parameter>& parameter) {
  data->getData(this)->stateset.clear();
  for(unsigned int i = 0; i < regions.size(); i++) {
    HierarchicalState* h = dynamic_cast<HierarchicalState*>(regions[i]);
    h->deactivate(data, parameter);
  }
  Context::deactivate(data, parameter);
}

//------------------------------------------------------------------------------

bool ConcurrentState::dispatch(SmartPointer<Metadata>& data, 
                               SmartPointer<Event>& event, 
                               SmartPointer<Parameter>& parameter) {
  // at least one region must dispatch the event
  bool dispatched = false;

  StateRuntimedata* statedata = data->getData(this);

  /*
   * Dispatch the event in all regions as long as this state is
   * active. If we don´t check this, an implicit exist would be
   * ignored by this code.
   */
  for(unsigned int i = 0; i < regions.size() && statedata && statedata->active; i++) {
    HierarchicalState* h = dynamic_cast<HierarchicalState*>(regions[i]);
    if(h->dispatch(data , event, parameter)) {
      dispatched = true;
      statedata = data->getData(this);
    }
  }

  if(dispatched) {
    return true;
  }

  /*
   * Dispatch the event on this state. but make sure that all
   * regions are finished before we can leave this state with the
   * final-transition.
   */
  for(unsigned int i = 0; i < transitions.size() && statedata->active; i++) {
    Transition* t = dynamic_cast<Transition*>(transitions[i]);

    // If not finished, ignore all transitions without an event
    if(!t->hasEvent() && !finished(data)) {
      continue;
    }

    if(t->execute(event, data, parameter)) {
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------



