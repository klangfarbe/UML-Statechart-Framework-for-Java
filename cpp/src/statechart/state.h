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
#ifndef STATE_H
#define STATE_H

#include <vector>
#include <string>

#include "action.h"
#include "event.h"
#include "metadata.h"
#include "parameter.h"
#include "transition.h"
#include "../util/smartpointer.h"

namespace statechart {

  class Context;

/**
 * Represents a simple state of the statechart. For debugging purposes each
 * state can have a name assigned for identification.
 */
  class State {
//---------------------------------------------------------------------------
// Methods
//---------------------------------------------------------------------------
  public:
    /**
     * Creates a state with the given actions. The action objects will be
     * destroyed when the state ist deleted.
     */
    State(Context* parent, Action* entryAction = 0, Action* doAction = 0, 
          Action* exitAction = 0, char* name = 0);

    /**
     * The destructor deletes all transition and action objects.
     */
    virtual ~State();

    /**
     * Gets the context
     */
    Context* getContext() { 
      return context;
    };

    /**
     * Sets the entry action. If there is already an action given,
     * it will be destroyed first.
     */
    void setEntryAction(Action* action);

    /**
     * Sets the do action. If there is already an action given,
     * it will be destroyed first.
     */
    void setDoAction(Action* action);

    /**
     * Sets the exit action. If there is already an action given,
     * it will be destroyed first.
     */
    void setExitAction(Action* action);

    /**
     * Sets the name of the state used for debugging purposes.
     */
    void setName(char* name);

    /**
     * return the given name
     */
    const char* toString() { return name ? name : "";};

  protected:
    /**
     * Adds a transition to the list.
     */
    void addTransition(Transition* transition);

    /**
     * Activates the state.
     */
    virtual bool activate(util::SmartPointer<Metadata>& data, 
                          util::SmartPointer<Parameter>& parameter);

    /**
     * Deactivates the state.
     */
    virtual void deactivate(util::SmartPointer<Metadata>& data, 
                            util::SmartPointer<Parameter>& parameter);

    /**
     * Dispatches the given event.
     */
    virtual bool dispatch(util::SmartPointer<Metadata>& data, 
                          util::SmartPointer<Event>& event, 
                          util::SmartPointer<Parameter>& parameter);

//---------------------------------------------------------------------------
// Attributes
//---------------------------------------------------------------------------
  protected:
    /// The entry action to execute.
    Action* entryAction;

    /// The do action to execute.
    Action* doAction;

    /// The exit action to execute.
    Action* exitAction;

    /// List of the associated transition objects.
    std::vector<Transition*> transitions;

    /// The context of this state. Must be set using setContext().
    Context* context;

    /// Pointer to the statechart the state belongs to
    Statechart* statechart;

  private:
    /// The name of the state (char* for easier debugging).
    char* name;

    friend class Transition;
    friend class Statechart;
    friend class ConcurrentState;
    friend class HierarchicalState;
    friend class PseudoState;
  };

}

#endif
