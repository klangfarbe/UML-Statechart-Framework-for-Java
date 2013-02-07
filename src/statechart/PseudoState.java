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
 * Implements the Pseudostates. Note that it is not possible to stay in this
 * kind of state.
 */
public class PseudoState extends State {
  //============================================================================
  // ATTRIBUTES
  //============================================================================
  public static final int pseudostate_start = 1;
  public static final int pseudostate_junction = 2;
  public static final int pseudostate_fork = 3;
  public static final int pseudostate_join = 4;
  public static final int pseudostate_history = 5;
  public static final int pseudostate_deep_history = 6;

  /** The type of this state */
  int type;

  /** Incoming transitions (needed for join) */
  Vector<Transition> incoming = null;
  
  //============================================================================
  // METHODS
  //============================================================================
  /**
   * Creates a pseudo state with the given type.
   * @throws StatechartException 
   */
  public PseudoState(String name, Context parent, int type) throws StatechartException {
    super(name, parent, null, null, null);
    this.type = type;
    if(type == pseudostate_start) {
      if(parent.startState == null) {
        parent.startState = this;        
      } else {
        throw new StatechartException("Parent has already a start state!");
      }
    } else if (type == pseudostate_history || type == pseudostate_deep_history) {
      if(parent instanceof HierarchicalState) {
        HierarchicalState h = (HierarchicalState)parent;
        if(h.history == null) {
          h.history = this;
        } else {
          throw new StatechartException("Parent has already a history state!");
        }
      } else {
        throw new StatechartException("Parent is not hierarchical state!");
      }
    }
  }

  //============================================================================

  /**
   * Returns the type of this pseudo state.
   */
  public int getType() {
    return type;
  };

  //============================================================================

  /**
   * Does a lookup if this pseudo state can be activated. This is true when
   * there exists a path to the next real state.
   */
  boolean lookup(Metadata data, Parameter parameter) {
    // check if all incoming transitions can trigger
    if(type == pseudostate_join) {
      for(int i = 0; i < incoming.size(); i++) {
        Transition t = incoming.get(i);
        StateRuntimedata d = data.getData(t.deactivate.get(0));
        if(d == null || d != null && !d.active || t.hasGuard() 
           && !t.guard.check(data, parameter) ) {
          return false;
        }
      }
    }

    // check if an outgoing transition can trigger
    for(int i = 0; i < transitions.size(); i++) {
      if(transitions.get(i).allowed(data, parameter)) {
        return true;
      }
    }
    return false;
  }

  //============================================================================

  /**
   * Activates the state and executes the next nessecary steps depending on the
   * type. Overwrites the method from class State.
   */
  boolean activate(Metadata data, Parameter parameter) {
    data.activate(this);
    StateRuntimedata d = data.getData(this);

    if(entryAction != null) {
      entryAction.execute(data, parameter);
    }

    if(type == pseudostate_history || type == pseudostate_deep_history) {
      for(int i = 0; i < d.stateset.size(); i++) {
        d.stateset.get(i).activate(data, parameter);
      }
    } else if(type == pseudostate_fork) {
      // Exclude regions from automatic activation through the and-state
      for(int j = 0; j < transitions.size(); j++) {
        Transition t = transitions.get(j);
        // Activate the region at it's start state if the guard fails.
        if(t.guard != null && !t.guard.check(data, parameter)) {
          continue;
        }

        for(int i = 0; i < t.activate.size(); i++) {
          if(i + 1 < t.activate.size() && t.activate.get(i) instanceof ConcurrentState) {
            ConcurrentState s = (ConcurrentState)t.activate.get(i);
            StateRuntimedata cd = data.createRuntimedata(s);

            if (!cd.stateset.contains(t.activate.get(i + 1))) {
              cd.stateset.add(t.activate.get(i + 1));
            }
          }
        }
      }
    } else if(type == pseudostate_join) {
      // trigger every action of all incoming transitions
    }

    return true;
  }

  //============================================================================

  /**
   * Stores the active states for a history state.
   */
  void storeHistory(Metadata data) {
    StateRuntimedata d = data.getData(this);
    if(d != null) {
      d.stateset.clear();
      PseudoState.calculate(d.stateset, data.getData(context).currentState, 
          data, type);
    }
  }

  //============================================================================

  /**
   * If a history type is given and the history is stored the event will be
   * ignored because on activation the substates are in the context the current
   * active state and not the history pseudostate anymore. for other states
   * simply call the base method.
   */
  boolean dispatch(Metadata data, Event event, Parameter parameter) {
    if (type == pseudostate_history || type == pseudostate_deep_history) {
      StateRuntimedata d = data.getData(context);
      if (d != null && d.currentState != null && d.currentState != this) {
        return d.currentState.dispatch(data, event, parameter);
      }
    } else if (type == pseudostate_fork) {
      // enter the state and execute the transitions
      for (int i = 0; i < transitions.size(); i++) {
        Transition t = transitions.get(i);
        t.execute(event, data, parameter);
      }
      return true;
    }
    return super.dispatch(data, event, parameter);
  }

  //============================================================================

  /**
   * Adds an incoming transition to the state. is called by the transition
   * constructor.
   */
  void addIncomingTransition(Transition t) {
    if (incoming == null) {
      incoming = new Vector<Transition>();
    }
    incoming.add(t);
  }

  //============================================================================

  /**
   * Does a depth-first-search to get all active nodes in the hierarchy tree.
   */
  private static void calculate(Vector<State> history, State s, Metadata data, int type) {
    if (s == null) {
      return;
    }

    history.add(s);

    if (type == pseudostate_history) {
      return;
    }

    if (s instanceof HierarchicalState) {
      StateRuntimedata runtimedata = data.getData((HierarchicalState) s);
      State substate = runtimedata != null ? runtimedata.currentState : null;
      if (substate != null) {
        calculate(history, substate, data, type);
      }
    }
/*
    if (s instanceof ConcurrentState) {
      Vector regions = ((ConcurrentState) s).regions;
      for (int i = 0; i < regions.size(); i++) {
        calculate(history, (State) regions.get(i), data, type);
      }
    }
    */
  }
}
