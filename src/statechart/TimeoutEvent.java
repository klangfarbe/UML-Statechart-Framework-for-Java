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

import java.util.UUID;

/**
 * Timeout event for a transition
 */
public class TimeoutEvent extends Event {
  //============================================================================
  // ATTRIBUTES
  //============================================================================
  /**
   * The timeout value in milliseconds
   */
  private long timeout;
  private UUID uuid = UUID.randomUUID();
  
  //============================================================================
  // METHODS
  //============================================================================
  /**
   * Creates a timeout event.
   * @param timeout The timeout value in milliseconds
   */
  public TimeoutEvent(long timeout) {
    super("TimeoutEvent");
    this.timeout = timeout;
  }

  //============================================================================

  /**
   * Checks if the timevalue in the given parameter is larger than the one
   * stored in this event.
   *
   * @param event the event to compare
   * @param data The runtime data object
   * @param param The parameter for this event. Must be a TimeoutParameter type.
   */
  public boolean equals(Event event, Metadata data, Parameter param) {
    if(event instanceof TimeoutEvent && ((TimeoutEvent)event).uuid == this.uuid) {
      return true;
    }
    return false;
  }

  //============================================================================

  /**
   * Gets the timeout of the event.
   */
  public long getTimout() {
    return timeout;
  }
  
  //============================================================================

  /**
   * Sets the current timeout
   */
  public void setTimeout(long timeout) {
    this.timeout = timeout;
  }
}
