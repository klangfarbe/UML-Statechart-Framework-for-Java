/*
 * UML statechart framework (http://github.com/klangfarbe/UML-Statechart-Framework-for-Java)
 *
 * Copyright (C) 2006-2013 Christian Mocek (christian.mocek@gmail.com)
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
package statechart;

import java.util.Vector;

/**
 * Describes a Transition between states.
 */
public class Transition {
  //============================================================================
  // ATTRIBUTES
  //============================================================================
  // The triggering event or 0 if no event is used.
  Event event = null;

  // The guard watching if the transition can trigger.
  Guard guard = null;

  // The action to execute when the transition triggers.
  Action action = null;

  // List of all states which must be deactivated when triggering.
  Vector<State> deactivate = new Vector<State>();

  // List of all states which must be activated when triggering.
  Vector<State> activate = new Vector<State>();
  
  //============================================================================
  // METHODS
  //============================================================================
  public Transition(State start, State end) {
    init(start, end, null, null, null);
  }

  //============================================================================

  public Transition(State start, State end, Event event) {
    init(start, end, event, null, null);
  }

  //============================================================================

  public Transition(State start, State end, Guard guard) {
    init(start, end, null, guard, null);
  }

  //============================================================================

  public Transition(State start, State end, Action action) {
    init(start, end, null, null, action);
  }

  //============================================================================

  public Transition(State start, State end, Event event, Guard guard) {
    init(start, end, event, guard, null);
  }

  //============================================================================

  public Transition(State start, State end, Event event, Action action) {
    init(start, end, event, null, action);
  }

  //============================================================================

  public Transition(State start, State end, Guard guard, Action action) {
    init(start, end, null, guard, action);
  }

  //============================================================================

  public Transition(State start, State end, Event event, Guard guard,
      Action action) {
    init(start, end, event, guard, action);
  }

  //============================================================================

  /**
   * Checks wether an event is associated with this transition or not.
   */
  boolean hasEvent() {
    return event != null ? true : false;
  };

  //============================================================================

  /**
   * Checks wether a guard is associated with this transition or not.
   */
  boolean hasGuard() {
    return guard != null ? true : false;
  };

  //============================================================================

  /**
   * Checks wether an action is associated with this transition or not.
   */
  boolean hasAction() {
    return action != null ? true : false;
  };

  //============================================================================

  /**
   * Executes the transition and triggers the new step.
   */
  boolean execute(Event event, Metadata data, Parameter parameter) {
    // check if the event can be handled
    if (this.event != null && !this.event.equals(event, data, parameter)) {
      return false;
    }

    if (this.event != null && event == null) {
      return false;
    }

    if (!allowed(data, parameter)) {
      return false;
    }

    // deactivate all states
    for (int i = 0; i < deactivate.size(); i++) {
      deactivate.get(i).deactivate(data, parameter);
    }

    // Execute exit-action
    if (action != null) {
      action.execute(data, parameter);
    }

    // Activate all new states.
    for (int i = 0; i < activate.size(); i++) {
      /*
       * check if we activate an concurrent state imlicit and if so make sure
       * adding the correct region to the list of regions to ignore on
       * activation. It is activated by this transition.
       */
      if (i + 1 < activate.size() && activate.get(i) instanceof ConcurrentState) {
        ConcurrentState s = (ConcurrentState) activate.get(i);
        StateRuntimedata cd = data.createRuntimedata(s);

        if (!cd.stateset.contains(activate.get(i + 1))) {
          cd.stateset.add(activate.get(i + 1));
        }
      }
      activate.get(i).activate(data, parameter);
    }
    return true;
  }

  //============================================================================

  /**
   * Checks if all constraints are fullfilled. To do this the whole path up to
   * the next real state is checked.
   */
  boolean allowed(Metadata data, Parameter parameter) {
    if (guard != null && !guard.check(data, parameter)) {
      return false;
    }

    /*
     * if target is a pseudostate, call lookup to check if we do not stay in
     * this state. So get the last state in the activate list. end() is behind
     * the last element so we have to select the prior element by using *--.
     */
    State target = (State) activate.lastElement();
    if (target instanceof PseudoState) {
      return ((PseudoState) target).lookup(data, parameter);
    }

    return true;
  }

  //============================================================================

  /**
   * Initializes the Transition
   */
  private void init(State start, State end, Event event, Guard guard,
      Action action) {
    this.event = event;
    this.guard = guard;
    this.action = action;
    Transition.calculateStateSet(start, end, deactivate, activate);
    start.addTransition(this);

    // for handling join states correctly, we need to know the incoming
    // transition
    if (end instanceof PseudoState
        && ((PseudoState) end).getType() == PseudoState.pseudostate_join) {
      ((PseudoState) end).addIncomingTransition(this);
    }
  }

  //============================================================================

  /**
   * Calculates all the states which must be deactivated and then activated when
   * triggering the transition.
   */
  private static void calculateStateSet(State start, State end,
      Vector<State> deactivate, Vector<State> activate) {
    // temp vectors for calculating the LCA (least common ancestor)
    Vector<State> a = new Vector<State>();
    Vector<State> d = new Vector<State>();

    // get all states for possible deactivation
    State s = start;
    while (s != null) {
      d.add(0, s);
      Context context = s.context;

      // If context is hierarchical or concurrent state, get it as parent.
      if (context != null && !(context instanceof Statechart)) {
        s = context;
      } else {
        s = null;
      }
    }

    // get all states for possible activation
    State e = end;
    while (e != null) {
      a.add(0, e);
      Context context = e.context;

      // If context is hierarchical or concurrent state, get it as parent.
      if (context != null && !(context instanceof Statechart)) {
        e = context;
      } else {
        e = null;
      }
    }

    /*
     * Get LCA number. It is min-1 by default. Therefore we make sure that if
     * start equals end, we do not get the whole path up to the root node if the
     * state is a substate.
     */
    int min = a.size() < d.size() ? a.size() : d.size();
    int lca = min - 1;

    // get the LCA-State
    if (start != end) {
      // if the first entry is not equal we got the LCA
      for (lca = 0; lca < min; lca++) {
        if (a.get(lca) != d.get(lca)) {
          break;
        }
      }
    }

    // Fill the given vectors for the transition
    for (int j = lca; j < d.size(); j++) {
      deactivate.add(0, d.get(j));
    }

    for (int j = lca; j < a.size(); j++) {
      activate.add(a.get(j));
    }
  }
}
