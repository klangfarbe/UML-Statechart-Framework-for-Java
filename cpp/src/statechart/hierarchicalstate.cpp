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
#include <statechart/hierarchicalstate.h>
#include <statechart/concurrentstate.h>
#include <statechart/timeoutevent.h>

using namespace statechart;
using namespace statechart::util;

//------------------------------------------------------------------------------

HierarchicalState::HierarchicalState(Context* parent, Action* entryAction, 
                                     Action* doAction, Action* exitAction, 
                                     const char* name) 
: Context(parent, entryAction, doAction, exitAction, name), history(0) {
  ConcurrentState* c = dynamic_cast<ConcurrentState*>(parent);
  if(c) {
    c->addRegion(this);
  }
}

//------------------------------------------------------------------------------

void HierarchicalState::deactivate(SmartPointer<Metadata>& data, 
                                   SmartPointer<Parameter>& parameter) {
  if(!data->isActive(this)) {
    return;
  }

  StateRuntimedata* statedata = data->getData(this);

  if(!statedata) {
    Context::deactivate(data, parameter);
    return;
  }

  // save the history
  if(history
     && statedata->currentState != startState
     && statedata->currentState != history
     && !dynamic_cast<FinalState*>(statedata->currentState)) {
    history->storeHistory(data);
  }

  /*
   * deactivate substate
   */
  if(statedata->currentState) {
    statedata->currentState->deactivate(data, parameter);
  }

  statedata->currentState = 0;

  Context::deactivate(data, parameter);
}

//------------------------------------------------------------------------------

bool HierarchicalState::dispatch(SmartPointer<Metadata>& data, 
                                 SmartPointer<Event>& event, 
                                 SmartPointer<Parameter>& parameter) {
  if(!data->isActive(this)) {
    return false;
  }

  StateRuntimedata* statedata = data->getData(this);

  // Use startstate on activation if available
  if(!statedata->currentState && startState) {
    data->activate(startState);
    statedata->currentState->activate(data, parameter);
  }

  // delegate event to substate.
  bool rc = false;

  if(statedata->currentState) {
    rc = statedata->currentState->dispatch(data, event, parameter);
  }

  /*
   * If the substate dispatched the event and we reached the finalstate or this
   * state is no longar active, trigger a new dispatch for the end transition, 
   * otherwise return.
   */
  if(!data->isActive(this) || rc && !dynamic_cast<FinalState*>(statedata->currentState)) {
    return rc;
  }

  /*
   * If no substate can handle the event try to find a transition on this state
   * which can. There are 3 possibilities:
   * - The endstate is reached: Call the finishing transition
   * - Handle the event with a transition from this state
   * - Handle the event with a transition inherited from the context
   */
  for(unsigned int i = 0; i < transitions.size(); i++) {
    Transition* t = transitions[i];

    // Filter all finishing transitions if endstate is not active
    if(!dynamic_cast<FinalState*>(statedata->currentState) && !t->hasEvent()) {
      continue;
    }

    /*
     * If the endstate is active remove the event because a finishing transition
     * has never an event. If we don't remove it here, we'll get stuck in the
     * endstate!!!
     */
    if(dynamic_cast<FinalState*>(statedata->currentState)) {
      // try to fire the transition
      SmartPointer<Event> e;
      if(t->execute(e, data, parameter)) {
        return true;
      }
    } else {
      // try to fire the transition
      if(t->execute(event, data, parameter)) {
        return true;
      }
    }    
  }

  return false;
}

//------------------------------------------------------------------------------
