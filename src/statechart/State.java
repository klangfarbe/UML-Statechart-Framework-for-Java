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
 * Represents a simple state of the statechart. For debugging purposes each
 * state can have a name assigned for identification.
 */
public class State {
  //============================================================================
  // ATTRIBUTES
  //============================================================================
  // The entry action to execute.
  protected Action entryAction= null;

  // The do action to execute.
  protected Action doAction = null;

  // The exit action to execute.
  protected Action exitAction = null;

  // List of the associated transition objects.
  protected Vector<Transition> transitions = new Vector<Transition>();

  // The context of this state.
  Context context = null;
  
  // The statechart this state belongs to.
  Statechart statechart = null;

  // The name of the state (for easier debugging).
  protected String name = null;
  
  //============================================================================
  // METHODS
  //============================================================================
  /**
   * Creates a state with the given actions. 
   * @throws StatechartException 
   */
  public State(String name, Context parent, Action entryAction, Action doAction, Action exitAction) throws StatechartException {
    if(parent == null && !(this instanceof Statechart)) {
      throw new StatechartException("Parameter parent cannot be null");
    }
    if(name == null) {
      throw new StatechartException("Parameter name cannot be null");
    }
    
    this.context = parent;
    
    // get the statechart, we need it for the event queues
    if(parent != null) {
      while(parent.context != null) {
        parent = parent.context;
      }
      if(parent instanceof Statechart) {
        this.statechart = (Statechart)parent;
        if(this.statechart.states.containsKey(name)) {
          throw new StatechartException("State name <" + name + "> already used! Please define a unique name.");
        }
        this.statechart.states.put(name, this);
      } else {
        throw new StatechartException("Cannot determine path to the statechart. Check the hierarchy.");
      }
    }

    this.name = name;
    this.entryAction = entryAction;
    this.doAction = doAction;
    this.exitAction = exitAction;    
  }

  //============================================================================

  /**
   * Gets the context
   */
  public Context getContext() { 
    return context;
  };

  //============================================================================

  /**
   * Sets the entry action. If there is already an action given,
   * it will be destroyed first.
   */
  public void setEntryAction(Action action) {
    this.entryAction = action;
  }

  //============================================================================

  /**
   * Sets the do action. If there is already an action given,
   * it will be destroyed first.
   */
  public void setDoAction(Action action) {
    this.doAction = action;
  }

  //============================================================================

  /**
   * Sets the exit action. If there is already an action given,
   * it will be destroyed first.
   */
  public void setExitAction(Action action) {
    this.exitAction = action;    
  }

  //============================================================================

  /**
   * Sets the name of the state used for debugging purposes.
   */
  public void setName(String name) {
    this.name = name;
  }

  //============================================================================

  /**
   * return the given name
   */
  public String toString() { 
    return name == null ? "" : name;
  }

  //============================================================================

  /**
   * Adds a transition to the list.
   */
  void addTransition(Transition transition) {
    // make sure transition with guards are checked first!
    if(transition.hasGuard()) {
      transitions.add(0, transition);
    } else {
      transitions.add(transition);
    }
  }

  //============================================================================

  /**
   * Activates the state.
   */
  boolean activate(Metadata data, Parameter parameter) {
    if(!data.isActive(this)) {
      data.activate(this);
      
      // trigger the timout events if available
      for(int i = 0; i < transitions.size(); i++)  {
        Transition t = transitions.get(i);
        if(t.event != null && t.event instanceof TimeoutEvent) {
          TimeoutEvent event = (TimeoutEvent)t.event;          
          EventQueueEntry entry = new EventQueueEntry(statechart, 
                                                      this, data, event, parameter,
                                                      ((TimeoutEvent)event).getTimout());
          StateRuntimedata runtimedata = data.getData(this);
          runtimedata.timeoutEvents.add(entry);
          statechart.timeoutEventQueue.add(entry);
        }
      }
      
      if(entryAction != null) {
        entryAction.execute(data, parameter);
      }

      if(doAction != null) {
        doAction.execute(data, parameter);
      }
      return true;
    }
    return false;
  }

  //============================================================================

  /**
   * Deactivates the state.
   */
  void deactivate(Metadata data, Parameter parameter) {
    if(data.isActive(this)) {
      // Mark events as invalid
      Vector<EventQueueEntry> timeoutEvents = data.getData(this).timeoutEvents;
      if(timeoutEvents != null) {
        for (EventQueueEntry event : timeoutEvents) {
          event.invalid = true;
          /*
           * @FIXME
           * @See Java-Bug ID: 6268068. Proposed fix is to use:
           * removeAll(Collections.singletonList(event)).  
           *
           * Problem: Leads to ConcurrentModificationException. Final fix is in JDK6
           */
          statechart.timeoutEventQueue.remove(event);
        }
      }
      data.deactivate(this);
      if(exitAction != null) {
        exitAction.execute(data, parameter);
      }      
    }
  }

  //============================================================================

  /**
   * Dispatches the given event.
   */
  boolean dispatch(Metadata data, Event event, Parameter parameter) {
    for(int i = 0; i < transitions.size(); i++)  {
      if(transitions.get(i).execute(event, data, parameter)) {
        return true;
      }
    }
    return false;
  }
}
