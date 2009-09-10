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
#ifndef TRANSITION_H
#define TRANSITION_H

#include <vector>

#include "event.h"
#include "guard.h"
#include "action.h"

namespace statechart {

  class State;
  class PseudoState;

/**
 * Describes a Transition between states.
 */
  class Transition {
//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
  public:
    Transition(State* start, State* end);
    Transition(State* start, State* end, Event* event);
    Transition(State* start, State* end, Guard* guard);
    Transition(State* start, State* end, Action* action);
    Transition(State* start, State* end, Event* event, Guard* guard);
    Transition(State* start, State* end, Event* event, Action* action);
    Transition(State* start, State* end, Guard* guard, Action* action);
    Transition(State* start, State* end, Event* event, Guard* guard, Action* action);
    ~Transition();

    /**
     * Checks wether an event is associated with this transition or not.
     */
    bool hasEvent() { return event ? true : false;};

    /**
     * Checks wether a guard is associated with this transition or not.
     */
    bool hasGuard() { return guard ? true : false;};

    /**
     * Checks wether an action is associated with this transition or not.
     */
    bool hasAction() { return action ? true : false;};

  private:
    /**
     * Initializes the Transition
     */
    void init(State* start, State* end, Event* event, Guard* guard, Action* action);

    /**
     * Executes the transition and triggers the new step.
     */
    bool execute(util::SmartPointer<Event>& event, 
                 util::SmartPointer<Metadata>& data, 
                 util::SmartPointer<Parameter>& parameter);

    /**
     * Checks if all constraints are fullfilled. To do this the whole path up to
     * the next real state is checked.
     */
    bool allowed(util::SmartPointer<Metadata>& data, 
                 util::SmartPointer<Parameter>& parameter);

  private:
    /**
     * Calculates all the states which must be deactivated and then activated
     * when triggering the transition.
     */
    static void calculateStateSet(State* start, State* end,
                                  std::vector<State*>& deactivate,
                                  std::vector<State*>& activate);

//------------------------------------------------------------------------------
// Attributes
//------------------------------------------------------------------------------
  private:
    /// The triggering event or 0 if no event is used.
    Event* event;

    /// The guard watching if the transition can trigger.
    Guard* guard;

    /// The action to execute when the transition triggers.
    Action* action;

    /// List of all states which must be deactivated when triggering.
    std::vector<State*> deactivate;

    /// List of all states which must be activated when triggering.
    std::vector<State*> activate;

#ifdef STATECHART_DEBUG
    //util::Mutex writeMutex;
#endif

    // Fork and join needs access to the action, guard and the statelists
    friend class State;
    friend class PseudoState;
    friend class ConcurrentState;
    friend class HierarchicalState;
  };
}

#endif


