/*
 * UML statechart framework (http://uml-statecharts.sourceforge.net)
 *
 * Copyright (C) 2006-2009 Christian Mocek (klangfarbe@users.sourceforge.net)
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

import java.util.concurrent.Delayed;
import java.util.concurrent.TimeUnit;

/**
 * The entry for an asynchronous dispatched event. If necessary the execution 
 * of the event can be delayed for a specific amount of milliseconds.
 */
public class EventQueueEntry implements Runnable, Delayed {
  //============================================================================
  // ATTRIBUTES
  //============================================================================
  private Statechart statechart = null;
  private State state = null;
  private Metadata data = null;
  private Event event = null;
  private Parameter parameter = null;
  private long timeout = 0;
  private long added = System.currentTimeMillis();
  volatile boolean invalid = false;

  //============================================================================
  // METHODS
  //============================================================================  
  public EventQueueEntry(Statechart statechart, State state, Metadata data, Event event, Parameter parameter, long timeout) {
    this.statechart = statechart;
    this.state = state;
    this.data = data;
    this.event = event;
    this.parameter = parameter;
    this.timeout = timeout; 
  }

  //============================================================================
  // Inherited by Runnable
  //============================================================================
  public void run() {
    // if the state this event belongs to is not active anymore ignore it
    if(!invalid && data.isActive(state)) {
      statechart.dispatch(data, event, parameter);
    }
    statechart = null;
    state = null;
    data = null;
    event = null;
    parameter = null;
  }

  // ============================================================================
  // Inherited by Delayed
  // ============================================================================
  public long getDelay(TimeUnit sourceUnit) {
    long ms = System.currentTimeMillis();
    long duration = (timeout + added) - ms;
    return duration <= 0 ? 0 : sourceUnit.convert(duration, TimeUnit.MILLISECONDS);
  }

  // ============================================================================

  public int compareTo(Delayed d) {
    if (d instanceof EventQueueEntry) {
      EventQueueEntry entry = (EventQueueEntry)d;
      if (this.equals(entry)) {
        return 0;
      } else if (added + timeout < entry.added + entry.timeout) {
        return -1;
      } else if (added + timeout > entry.added + entry.timeout) {
        return 1;
      }
    }
    return 0;
  }

  // ============================================================================
  // Inherited by Object
  // ============================================================================
  @Override
  /**
   * Just checks if the state and data is equal. This is used by the delayQueue
   * to check if the event must be removed
   */
  public boolean equals(Object obj) {
    if ((obj == null) || (obj.getClass() != this.getClass())) {
      return false;
    }
    if (obj == this) {
      return true;
    }
    EventQueueEntry entry = (EventQueueEntry) obj;
    if (entry.statechart == statechart 
      && entry.data == data
      && entry.state == state 
      && entry.event == event 
      && entry.parameter == parameter 
      && entry.added == added
      && entry.timeout == timeout) {
      return true;
    }
    return false;
  }
}
