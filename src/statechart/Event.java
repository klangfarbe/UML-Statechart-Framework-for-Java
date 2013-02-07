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
 * Interface for an event for the statechart.
 */
public abstract class Event {
  //============================================================================
  // ATTRIBUTES
  //============================================================================
  /**
   * The ID of the event
   */
  String id = new String();
  
  //============================================================================
  // METHODS
  //============================================================================
  /**
   * Creates an event
   */
  public Event() {
  };

  //============================================================================

  /**
   * Creates an event with a given id 
   * @param id
   */
  public Event(String id) {
    this.id = id;
  };

  //============================================================================

  /**
   * Called by the transition to check if it should handle this event.
   * 
   * @param event
   *          the event to compare
   * @param data
   *          The runtime data object
   * @param param
   *          The parameter for this action
   */
  public boolean equals(Event event, Metadata data, Parameter param) {
    return event != null ? id.compareTo(event.toString()) == 0 : false;
  };

  //============================================================================

  /**
   * Returns the id
   */
  public String toString() {
    return id;
  }
}
