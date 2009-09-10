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
#include "pseudostate.h"
#include "metadata.h"
#include "hierarchicalstate.h"
#include "concurrentstate.h"
#include "statechartexception.h"

using namespace statechart;
using namespace statechart::util;

//------------------------------------------------------------------------------

PseudoState::PseudoState(Context* parent, PseudoStateType type, char* name) 
: State(parent, 0, 0, 0, name) {
  this->type = type;
  if(type == pseudostate_start) {
    if(!parent->startState) {
      parent->startState = this;        
    } else {
      throw new StatechartException("Parent has already a start state!");
    }
  } else if(type == pseudostate_history || type == pseudostate_deep_history) {
    HierarchicalState* h = dynamic_cast<HierarchicalState*>(parent);
    if(h) {
      if(!h->history) {
        h->history = this;
      } else {
        throw new StatechartException("Parent has already a history state!");
      }
    } else {
      throw new StatechartException("Parent is not hierarchical state!");
    }
  }
}

//------------------------------------------------------------------------------

bool PseudoState::activate(SmartPointer<Metadata>& data, 
                           SmartPointer<Parameter>& parameter) {
  data->activate(this);
  StateRuntimedata* d = data->getData(this);

  assert(d);

  if(entryAction) {
    entryAction->execute(data, parameter);
  }

  if(type == pseudostate_history || type == pseudostate_deep_history) {
    for(unsigned int i = 0; i < d->stateset.size(); i++) {
      ((State*)d->stateset[i])->activate(data, parameter);
    }
  } else if(type == pseudostate_fork) {
    // Exclude regions from automatic activation through the and-state
    vector<State*>::iterator p;
    for(unsigned int j = 0; j < transitions.size(); j++) {
      Transition* t = dynamic_cast<Transition*>(transitions[j]);
      // Activate the region at the start state if the guard fails.
      if(t->hasGuard() && !t->guard->check(data, parameter)) {
        continue;
      }

      for(unsigned int i = 0; i < t->activate.size(); i++) {
        if(i + 1 < t->activate.size() && dynamic_cast<ConcurrentState*>(t->activate[i])) {
          ConcurrentState* s = dynamic_cast<ConcurrentState*>(t->activate[i]);
          StateRuntimedata* cd = data->createRuntimedata(s);

          p = find(cd->stateset.begin(), cd->stateset.end(), t->activate[i + 1]);
          if(p == cd->stateset.end()) {
            cd->stateset.push_back(t->activate[i + 1]);
          }
        }
      }
    }
  } else if(type == pseudostate_join) {
    // trigger every action of all incoming transitions
  }

  return true;
}

//------------------------------------------------------------------------------

bool PseudoState::lookup(SmartPointer<Metadata>& data, 
                         SmartPointer<Parameter>& parameter) {  
  // check if all incoming transitions can trigger
  if(type == pseudostate_join) {
    for(unsigned int i = 0; i < incoming.size(); i++) {
      StateRuntimedata* d = data->getData(incoming[i]->deactivate[0]);
      if(!d || d && !d->active || incoming[i]->hasGuard() 
         && !incoming[i]->guard->check(data, parameter)) {
        return false;
      }
    }
  }

  // check if an outgoing transition can trigger
  for(unsigned int i = 0; i < transitions.size(); i++) {
    if(transitions[i]->allowed(data, parameter)) {
      return true;
    }
  }

  return false;
}

//------------------------------------------------------------------------------

bool PseudoState::dispatch(SmartPointer<Metadata>& data, 
                           SmartPointer<Event>& event, 
                           SmartPointer<Parameter>& parameter) {
  if(type == pseudostate_history || type == pseudostate_deep_history) {
    StateRuntimedata* d = data->getData(context);
    assert(d);
    if(d->currentState && d->currentState != this) {
      return d->currentState->dispatch(data, event, parameter);
    }
  }if(type== pseudostate_fork) {
    // enter the state and execute the transitions
    for(unsigned int i = 0; i < transitions.size(); i++) {
      Transition* t = dynamic_cast<Transition*>(transitions[i]);
      t->execute(event, data, parameter);
    }

    return true;
  }

  return State::dispatch(data, event, parameter);
}

//------------------------------------------------------------------------------

void PseudoState::storeHistory(SmartPointer<Metadata>& data) {
  StateRuntimedata* d = data->getData(this);
  if(d) {
    d->stateset.clear();
    PseudoState::calculate(d->stateset, data->getData(context)->currentState, data, type);
  }
}

//------------------------------------------------------------------------------

void PseudoState::calculate(vector<State*>& history,
                            State* s,
                            SmartPointer<Metadata>& data,
                            PseudoStateType type) {
  if(!s) {
    return;
  }

  history.push_back(s);

  if(type == pseudostate_history) {
    return;
  }

  HierarchicalState* h = dynamic_cast<HierarchicalState*>(s);
  if(h) {
    State* substate = data->getData(h)->currentState;
    if(substate) {
      calculate(history, substate, data, type);
    }
  }

  ConcurrentState* c = dynamic_cast<ConcurrentState*>(s);
  if(c) {
    for(unsigned int i = 0; i < c->regions.size(); i++) {
      PseudoState::calculate(history, c->regions[i], data, type);
    }
  }
}

//------------------------------------------------------------------------------

void PseudoState::addIncomingTransition(Transition* t) {
  incoming.push_back(t);
}

