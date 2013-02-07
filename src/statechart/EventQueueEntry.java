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

import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.Delayed;
import java.util.concurrent.TimeUnit;

/**
 * The entry for an asynchronous dispatched event. If necessary the execution 
 * of the event can be delayed for a specific amount of milliseconds.
 *
 * @Note This class has a natrual ordering that is inconsistant with equals.
 */
public class EventQueueEntry implements Runnable, Delayed {
  //============================================================================
  // ATTRIBUTES
  //============================================================================
  private static AtomicInteger idCounter = new AtomicInteger(1);
  private Statechart statechart = null;
  private State state = null;
  private Metadata data = null;
  private Event event = null;
  private Parameter parameter = null;
  private long relativeTimeout = 0;
  private long absoluteTimeout = 0;
  private long added = System.currentTimeMillis();
  private Integer id;
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
    this.relativeTimeout = timeout;
    this.absoluteTimeout = added + relativeTimeout;
    this.id = idCounter.getAndIncrement();
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
    long currentTime = System.currentTimeMillis();
    long duration = absoluteTimeout - currentTime;
    return duration <= 0 ? 0 : sourceUnit.convert(duration, TimeUnit.MILLISECONDS);
  }

  // ============================================================================

  /*
   * FIXME The behaviour of this Method is a bit strange in order to handle a bug 
   * in JDK 5. If used in JDK 6 or newer this method can be changed so that it only
   * compares the absoluteTimeout.
   */
  public int compareTo(Delayed d) {
    if (d instanceof EventQueueEntry) {
      EventQueueEntry entry = (EventQueueEntry)d;
      if (this == entry) {
        return 0;
      } else if (this.absoluteTimeout < entry.absoluteTimeout) {
        return -1;
      } else if (this.absoluteTimeout > entry.absoluteTimeout) {
        return 1;
      } else {
        return this.id.compareTo(entry.id);
      }
    } else {
      throw new ClassCastException("Cannot compare an object of " 
          + this.getClass() + " to an object of " + d.getClass());
    }
  }

  // ============================================================================
  // Inherited by Object
  // ============================================================================
  @Override
  public String toString() {
    StringBuilder builder = new StringBuilder();
    builder.append("EventQueueEntry [id=");
    builder.append(id);
    builder.append(", added=");
    builder.append(added);
    builder.append(", data=(");
    builder.append(data);
    builder.append("), event=");
    builder.append(event);
    builder.append(", invalid=");
    builder.append(invalid);
    builder.append(", parameter=");
    builder.append(parameter);
    builder.append(", state=");
    builder.append(state);
    builder.append(", statechart=");
    builder.append(statechart);
    builder.append(", relativeTimeout=");
    builder.append(relativeTimeout);
    builder.append(", absoluteTimeout=");
    builder.append(absoluteTimeout);
    builder.append("]");
    return builder.toString();
  }
}
