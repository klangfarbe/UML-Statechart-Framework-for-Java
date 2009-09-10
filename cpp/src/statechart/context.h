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
#ifndef CONTEXT_H
#define CONTEXT_H

#include <vector>

#include "state.h"
#include "pseudostate.h"
#include "finalstate.h"

using namespace std;

namespace statechart {

/**
 * Domain of the state. Needed for setting up the hierarchy. This class must
 * never be instantiated directly.
 */
  class Context : public State {
//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
  public:
    /**
     * Creates a context
     */
    Context(Context* parent, Action* entryAction = 0, Action* doAction = 0, 
          Action* exitAction = 0, char* name = 0);

    /**
     * Destructor
     */
    virtual ~Context();

  protected:
    /**
     * Add the substate to the list. Must not be called directly!
     */
    void addSubstate(State* state);

//------------------------------------------------------------------------------
// Attributes
//------------------------------------------------------------------------------
  protected:
    PseudoState* startState;

    /// List of all substates. Is needed only for destruction of the statechart
    std::vector<State*> substates;

    friend class State;
    friend class PseudoState;
  };
}

#endif

