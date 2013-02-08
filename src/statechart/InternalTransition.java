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
 * Describes a transition which does not change the state when triggered,
 * but only executes the associated action. This kind of transition must
 * have an event as its trigger. In contrast to the normal transition 
 * the states entry and exit actions are not triggered.
 */
public class InternalTransition extends Transition {
  //============================================================================
  // METHODS
  //============================================================================
  public InternalTransition(State state, Event event, Action action) {
    super(state, state, event, action);
    deactivate.clear();
    activate.clear();
  }
  
  //============================================================================

  public InternalTransition(State state, Event event, Guard guard, Action action) {
    super(state, state, event, guard, action);
    deactivate.clear();
    activate.clear();
  }

  //============================================================================
  
  @Override
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

    if (action != null) {
      action.execute(data, parameter);
    }
    return true;
  }
  
  //============================================================================
  
  @Override
  boolean allowed(Metadata data, Parameter parameter) {
    if (guard != null && !guard.check(data, parameter)) {
      return false;
    }
    return true;
  }
}
