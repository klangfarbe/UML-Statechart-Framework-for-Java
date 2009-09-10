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
#ifndef PSEUDOSTATE_H
#define PSEUDOSTATE_H

#include "state.h"

namespace statechart {

/**
 * The possible types of pseudo states.
 */
  enum PseudoStateType {
    /// The start state of a context
    pseudostate_start,
    /// The junction point for segmented transitions
    pseudostate_junction,
    /// The fork state
    pseudostate_fork,
    /// The join state
    pseudostate_join,
    /// The shallow-history of an or-state
    pseudostate_history,
    /// The deep-history of an or-state
    pseudostate_deep_history
  };

/**
 * Implements the Pseudostates. Note that it is not possible to stay in this
 * kind of state.
 */
  class PseudoState : public State {
//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
  public:
    /**
     * Creates a pseudo state with the given type.
     */
    PseudoState(Context* parent, PseudoStateType type, char* name = 0);

    /**
     * Does a lookup if this pseudo state can be activated. This is true when
     * there exists a path to the next real state.
     */
    bool lookup(util::SmartPointer<Metadata>& data, 
                util::SmartPointer<Parameter>& parameter);

    /**
     * Returns the type of this pseudo state.
     */
    const PseudoStateType getType() { return type;};

  protected:
    /**
     * Activates the state and executes the next nessecary steps depending on the
     * type. Overwrites the method from class State.
     */
    virtual bool activate(util::SmartPointer<Metadata>& data, 
                          util::SmartPointer<Parameter>& parameter);

    /**
     * If a history type is given and the history is stored the event will be
     * ignored because on activation the substates are in the context the current
     * active state and not the history pseudostate anymore. for other states
     * simply call the base method.
     */
    bool dispatch(util::SmartPointer<Metadata>& data, 
                  util::SmartPointer<Event>& event, 
                  util::SmartPointer<Parameter>& parameter);

    /**
     * Stores the active states for a history state.
     */
    void storeHistory(util::SmartPointer<Metadata>& data);

  private:
    /**
     * Adds an incoming transition to the state. is called by the
     * transition constructor.
     */
    void addIncomingTransition(Transition* t);

    /**
     * Does a depth-first-search to get all active nodes in the hierarchy tree.
     */
    static void calculate(std::vector<State*>& history, State* s, 
                          util::SmartPointer<Metadata>& data, 
                          PseudoStateType type);

//------------------------------------------------------------------------------
// Attributes
//------------------------------------------------------------------------------
  private:
    /// The type of this state
    PseudoStateType type;

    /// Incoming transitions (needed for join)
    std::vector<Transition*> incoming;

    friend class Transition;
    friend class HierarchicalState;
  };
}

#endif


