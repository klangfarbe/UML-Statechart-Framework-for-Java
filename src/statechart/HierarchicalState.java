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

/**
 * The implementation of the OR composite state.
 */
public class HierarchicalState extends Context {
  //============================================================================
  // ATTRIBUTES
  //============================================================================
  PseudoState history = null;
  
  //============================================================================
  // METHODS
  //============================================================================
  /**
   * Creates a simple OR-composite-state with the given actions.
   * @throws StatechartException 
   */
  public HierarchicalState(String name, Context parent, Action entryAction, Action doAction, Action exitAction) throws StatechartException {
    super(name, parent, entryAction, doAction, exitAction);
    
    if(parent instanceof ConcurrentState) {
      ((ConcurrentState)parent).addRegion(this);
    }
  }

  //============================================================================

  /**
   * Deactivates the state and informs the substates that they deactivate too.
   */
  void deactivate(Metadata data, Parameter parameter) {
    if(!data.isActive(this)) {
      return;
    }

    StateRuntimedata statedata = data.getData(this);

    if(statedata == null) {
      super.deactivate(data, parameter);
      return;
    }

    // save the history
    if(history != null
      && statedata.currentState != startState
      && statedata.currentState != history
      && !(statedata.currentState instanceof FinalState)) {
      history.storeHistory(data);
    }

    /*
     * deactivate substate
     */
    if(statedata.currentState != null) {
      statedata.currentState.deactivate(data, parameter);
    }

    statedata.currentState = null;

    super.deactivate(data, parameter);
  }

  //============================================================================

  /**
   * Overrides the dispatch method from the state and takes care of delegating
   * the incoming event to the current state.
   */
  boolean dispatch(Metadata data, Event event, Parameter parameter) {
    if(!data.isActive(this)) {
      return false;
    }

    StateRuntimedata statedata = data.getData(this);

    // Use startstate on activation if available
    if(statedata.currentState == null && startState != null) {
      data.activate(startState);
      statedata.currentState.activate(data, parameter);
    }

    // delegate event to substate.
    boolean rc = false;

    if(statedata.currentState != null) {
      rc = statedata.currentState.dispatch(data, event, parameter);
    }

    /*
     * If the substate dispatched the event and we reached the finalstate or this
     * state is no longar active, trigger a new dispatch for the end transition, 
     * otherwise return.
     */
    if((rc && !(statedata.currentState instanceof FinalState)) || !data.isActive(this)) {
      return rc;
    }

    /*
     * If no substate can handle the event try to find a transition on this state
     * which can. There are 3 possibilities:
     * - The endstate is reached: Call the finishing transition
     * - Handle the event with a transition from this state
     * - Handle the event with a transition inherited from the context
     */
    for(int i = 0; i < transitions.size(); i++) {
      Transition t = transitions.get(i);

      // Filter all finishing transitions if endstate is not active
      if(!(statedata.currentState instanceof FinalState) && !t.hasEvent()) {
        continue;
      }

      /*
       * If the endstate is active and the transition is a finishing transition 
       * (it has not event), trigger the transition without the event.
       * 
       * Otherwise try to trigger the transition with the event.
       */
      if(statedata.currentState instanceof FinalState && !t.hasEvent()) {
        if(t.execute(null, data, parameter)) {
          return true;
        }
      }

      // try to fire the transision
      if(t.execute(event, data, parameter)) {
        return true;
      }
    }
    return false;
  }
}
