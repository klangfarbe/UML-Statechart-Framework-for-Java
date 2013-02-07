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

import java.util.HashMap;
import java.util.Map;

/**
 * Describes runtime specific data of the statechart. The main data is the
 * currently active state, or in general all actives when using hierarchy. For
 * every active state a StateMetadata-Object is created which stores runtime
 * specifiv data for the state (e.g. the time since entering the state). This
 * object is allocated only when the state is active, otherwise it is deleted.
 */
public class Metadata {
  //============================================================================
  // ATTRIBUTES
  //============================================================================
  /** Keymap which holds the StateRuntimedata of a state */
  private Map<State, StateRuntimedata> activeStates = new HashMap<State, StateRuntimedata>();
  
  //============================================================================
  // METHODS
  //============================================================================
  /**
   * Creates a Metadata object.
   */
  public Metadata() {
  }

  //============================================================================

  /**
   * Checks wether the given state is active or not.
   */
  public boolean isActive(State state) {
    if (activeStates.containsKey(state)) {
      return getData(state).active;
    }
    return false;
  }

  //============================================================================

  /**
   * Gets the runtime specific data of the state.
   * 
   * @return The data or NULL if the state is not active
   */
  public StateRuntimedata getData(State state) {
    return activeStates.get(state);
  }

  //============================================================================

  /**
   * Activates a state for this Metadata. If the state is not in the Hashmap it
   * will be added and a new StateRuntimeData is created.
   */
  void activate(State state) {
    StateRuntimedata data = getData(state);
    if (data == null) {
      data = new StateRuntimedata();
      activeStates.put(state, data);
    }

    data.active = true;
    data.currentTime = System.currentTimeMillis();
    data.currentState = null;

    // update the context. if context is null we are at top level
    if (state.context != null) {
      data = activeStates.get(state.context);
      data.currentState = state;
    }
  }

  //============================================================================

  /**
   * Deactivates the state and frees the allocated resources.
   */
  void deactivate(State state) {
    if (activeStates.containsKey(state)) {
      StateRuntimedata data = getData(state);

      // If we store the history of a hierarchical state, keep it
      if (state instanceof PseudoState 
          && (((PseudoState)state).type == PseudoState.pseudostate_deep_history
          || ((PseudoState)state).type == PseudoState.pseudostate_history)) {
        data.active = false;
        return;
      }
            
      data.timeoutEvents.clear();
      data.currentState = null;
      data = null;
      activeStates.remove(state);
    }
  }

  //============================================================================

  /**
   * Gets the runtime data for a state. The difference to the normal getData
   * method is, that a new StateRuntimedata is created if it don't exists in the
   * hashmap. If the data already exists, it is returned instead.
   */
  StateRuntimedata createRuntimedata(State s) {
    StateRuntimedata data = activeStates.get(s);
    if (data == null) {
      data = new StateRuntimedata();
      activeStates.put(s, data);
    }
    return data;
  }

  //============================================================================

  /**
   * Resets the metadata object for reuse 
   */
  public void reset() {
    activeStates.clear();
  }
}
