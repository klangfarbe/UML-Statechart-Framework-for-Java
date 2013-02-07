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
 * Domain of the state. Needed for setting up the hierarchy. This class must
 * never be instantiated directly.
 */
public abstract class Context extends State {
  //============================================================================
  // ATTRIBUTES
  //============================================================================
  protected PseudoState startState = null;
  
  //============================================================================
  // METHODS
  //============================================================================
  public Context(String name, Context parent, Action entryAction, Action doAction, Action exitAction) throws StatechartException {
    super(name, parent, entryAction, doAction, exitAction);
  }  
}
