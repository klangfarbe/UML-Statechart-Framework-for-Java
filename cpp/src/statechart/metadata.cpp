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
#include "metadata.h"
#include "state.h"
#include "statechart.h"

using namespace std;
using namespace statechart;
using namespace statechart::util;

//------------------------------------------------------------------------------

Metadata::Metadata() {
  activeStates.clear();
}

//------------------------------------------------------------------------------

Metadata::~Metadata() {
  reset();
}

//------------------------------------------------------------------------------

void Metadata::activate(State* state) {
  // create a new runtime object
  StateRuntimedata* data = getData(state);

  if(!data) {
    data = new StateRuntimedata();    
    activeStates[state] = data;
  }

  data->active = true;
  data->timestamp = Time::now();

  // update the context, if the context is null, we are at toplevel
  Context* context = state->getContext();
  if(context) {
    data = getData(context);
    data->currentState = state;
  }
}

//------------------------------------------------------------------------------

void Metadata::deactivate(State* state) {
  map<State*, StateRuntimedata*>::iterator it = activeStates.find(state);
  if(activeStates.find(state) != activeStates.end()) {
    /*
     * Check if history is stored in the state. If so don't delete data object.
     * Concurent states always clear their stateset when deactivating.
     */
    StateRuntimedata* data = (*it).second;
    PseudoState* p = dynamic_cast<PseudoState*>(state);
    if(p && (p->getType() == pseudostate_history
             || p->getType() == pseudostate_deep_history)) {
      data->active = false;
    } else {
      delete data;
      data = 0;
      activeStates.erase(it);
    }
  }
}

//------------------------------------------------------------------------------

StateRuntimedata* Metadata::createRuntimedata(State* s) {
  StateRuntimedata* data = getData(s);
  if(!data) {
    data = new StateRuntimedata();
    activeStates[s] = data;
  }
  return data;
}

//------------------------------------------------------------------------------

void Metadata::reset() {
  for(map<State*, StateRuntimedata*>::iterator it = activeStates.begin();
     it != activeStates.end(); it++) {
    delete (*it).second;
    (*it).second = 0;
  }
  activeStates.clear();
}
