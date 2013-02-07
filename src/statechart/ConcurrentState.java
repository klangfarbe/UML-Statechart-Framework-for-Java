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
 * The implementation of the AND composite state.
 */
public class ConcurrentState extends Context {
  //============================================================================
  // ATTRIBUTES
  //============================================================================
  /**
   * The regions of the AND-composition.
   */
  private Vector<HierarchicalState> regions = new Vector<HierarchicalState>();
  
  //============================================================================
  // METHODS
  //============================================================================
  /**
   * Creates a simple OR-composite-state with the given actions.
   * @throws StatechartException 
   */
  public ConcurrentState(String name, Context parent, Action entryAction, Action doAction, Action exitAction) throws StatechartException {
    super(name, parent, entryAction, doAction, exitAction);
  }  

  //============================================================================

  /**
   * Adds a new region
   * 
   * @throws StatechartException 
   */
  void addRegion(HierarchicalState region) {
    regions.add(region);
  }

  //============================================================================
  
  /**
   * Activates the state.
   */
  boolean activate(Metadata data, Parameter parameter) {
    if (super.activate(data, parameter)) {
      StateRuntimedata statedata = data.getData(this);
      for (int i = 0; i < regions.size(); i++) {
        // check if the region is activated implicit via a incoming transition
        if (!statedata.stateset.contains(regions.get(i))) {
          HierarchicalState h = regions.get(i);
          if (h.activate(data, parameter)) {
            h.dispatch(data, null, parameter);
          }
        }
      }
      return true;
    }
    return false;
  }

  //============================================================================

  /**
   * Deactivates the state and informs the substates that they deactivate too.
   */
  void deactivate(Metadata data, Parameter parameter) {
    data.getData(this).stateset.clear();
    for (int i = 0; i < regions.size(); i++) {
      HierarchicalState h = regions.get(i);
      h.deactivate(data, parameter);
    }
    super.deactivate(data, parameter);
  }

  //============================================================================

  /**
   * Overrides the dispatch method from the state and takes care of delegating
   * the incoming event to the current state.
   */
  boolean dispatch(Metadata data, Event event, Parameter parameter) {
    // at least one region must dispatch the event
    boolean dispatched = false;

    StateRuntimedata statedata = data.getData(this);

    /*
     * Dispatch the event in all regions as long as this state is active. If we
     * don not check this, an implicit exist would be ignored by this code.
     */
    for (int i = 0; i < regions.size() && statedata.active; i++) {
      HierarchicalState h = regions.get(i);
      if (h.dispatch(data, event, parameter)) {
        dispatched = true;
      }
    }

    if (dispatched) {
      return true;
    }

    /*
     * Dispatch the event on this state. but make sure that all regions are
     * finished before we can leave this state with the final-transition.
     */
    for (int i = 0; i < transitions.size() && statedata.active; i++) {
      Transition t = transitions.get(i);

      // If not finished, ignore all transitions without an event
      if (t.event == null && !finished(data)) {
        continue;
      }

      if (t.execute(event, data, parameter)) {
        return true;
      }
    }
    return false;
  }

  //============================================================================

  /**
   * Check if all the regions reached their end-states.
   */
  private boolean finished(Metadata data) {
    for (int i = 0; i < regions.size(); i++) {
      HierarchicalState h = regions.get(i);
      if (!(data.getData(h).currentState instanceof FinalState)) {
        return false;
      }
    }
    return true;
  }
}
