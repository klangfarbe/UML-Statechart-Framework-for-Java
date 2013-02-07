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
 * This class describes some common exception which can occur when working
 * with the statechart.
 */
public class StatechartException extends Exception {
  //============================================================================
  // ATTRIBUTES
  //============================================================================
  private static final long serialVersionUID = 1L;

  //============================================================================
  // METHODS
  //============================================================================
  public StatechartException() {
    super();
  }
  
  //============================================================================

  public StatechartException(String reason) {
    super(reason);
  }

  //============================================================================

  public StatechartException(String reason, Throwable cause) {
    super(reason, cause);
  }

  //============================================================================

  public StatechartException(Throwable cause) {
    super(cause);
  }
}
