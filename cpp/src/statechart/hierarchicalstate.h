/*
 * UML statechart framework (http://uml-statecharts.sourceforge.net)
 *
 * Copyright (C) 2006-2007 Christian Mocek (klangfarbe@users.sourceforge.net)
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
#ifndef HIERARCHICAL_STATE_H
#define HIERARCHICAL_STATE_H

#include <statechart/context.h>

namespace statechart {

/**
 * The implementation of the OR composite state.
 */
  class HierarchicalState : public Context {
//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
  public:
    /**
     * Creates a simple OR-composite-state with the given actions.
     */
    HierarchicalState(Context* parent, Action* entryAction = 0, 
                      Action* doAction = 0, Action* exitAction = 0, 
                      const char* name = 0);

  protected:
    /**
     * Deactivates the state and informs the substates that they deactivate too.
     */
    void deactivate(util::SmartPointer<Metadata>& data, 
                    util::SmartPointer<Parameter>& parameter);

    /**
     * Overrides the dispatch method from the state and takes care of delegating
     * the incoming event to the current state.
     */
    bool dispatch(util::SmartPointer<Metadata>& data, 
                  util::SmartPointer<Event>& event, 
                  util::SmartPointer<Parameter>& parameter);

//------------------------------------------------------------------------------
// Attributes
//------------------------------------------------------------------------------
  private:
    /// The history state
    PseudoState* history;

    friend class ConcurrentState;
    friend class PseudoState;
  };
}

#endif




