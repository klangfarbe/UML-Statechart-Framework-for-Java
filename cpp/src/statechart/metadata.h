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
#ifndef METADATA_H
#define METADATA_H

#include <map>
#include <vector>
#include <statechart/util/time.h>
#include <statechart/util/mutex.h>
#include <statechart/util/smartpointer.h>

namespace statechart {

  class State;
  class Statechart;

/**
 * Holds the runtime specific data for a state in the statechart.
 */
  typedef struct StateRuntimedata {
    /**
     * Initializes all the data
     */
    StateRuntimedata() : timestamp() {
      currentState = 0;
      active = false;
    }

    /// flag if the state is active or not
    bool active;

    /// timestamp when the state has been activated
    unsigned long timestamp;

    /// The current active substate
    State* currentState;

    /// a generic set of states, needed e.g. for history storage or region activation
    std::vector<State*> stateset;
  } StateRuntimedata;

/**
 * Describes runtime specific data of the statechart. The main data is the
 * currently active state, or in general all actives when using hierarchy.
 * For every active state a StateMetadata-Object is created which stores
 * runtime specifiv data for the state (e.g. the time since entering the state).
 * This object is allocated only when the state is active, otherwise it is
 * deleted.
 */
  class Metadata : public util::IntrusiveSmartPointer {
//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
  public:
    /**
     * Creates a Metadata object.
     */
    Metadata();

    /**
     * Releases all allocated memory. Is called when the statechart reached the finalstate.
     */
    virtual ~Metadata();

    /**
     * Checks wether the given state is active or not.
     */
    bool isActive(State* state) {
      std::map<State*, StateRuntimedata*>::iterator it = activeStates.find(state);
      if(it != activeStates.end()) {
        return(*it).second->active;
      }
      return false;
    };

    /**
     * Gets the runtime specific data of the state.
     * @return The data or NULL if the state is not active
     */
    StateRuntimedata* getData(State* state) {
      std::map<State*, StateRuntimedata*>::iterator it = activeStates.find(state);
      if(it != activeStates.end()) {
        return(*it).second;
      }
      return 0;
    };

    /**
     * Activates a state for this Metadata. If the state is not in the Hashmap it
     * will be added and a new StateRuntimeData is created.
     */
    void activate(State* state);

    /**
     * Deactivates the state and frees the allocated resources.
     */
    void deactivate(State* state);

    /**
     * Resets the object for reuse.
     */
    void reset();

    /**
     * Locks the metadata for concurrent access.
     */
    void lock() {
      mutex.lock();
    }

    /**
     * Unlocks the metadata for concurrent access.
     */
    void unlock() {
      mutex.unlock();
    }

    /**
     * Checks if the data is locked
     */
    bool isLocked() {
      return mutex.locked();
    }

  private:
    /**
     * Gets the runtime data for a state. The difference to the normal
     * getData method is, that a new StateRuntimedata is created if
     * it don't exists in the hashmap. If the data already exists,
     * it is returned instead.
     */
    StateRuntimedata* createRuntimedata(State* s);

//------------------------------------------------------------------------------
// Attributes
//------------------------------------------------------------------------------
  private:
    /// Keymap which holds the StateRuntimedata of a state
    std::map<State*, StateRuntimedata*> activeStates;

    /// The mutex to lock and unlock the data object
    util::Mutex mutex;

    friend class PseudoState;
    friend class Statechart;
    friend class Transition;
  };
}

#endif
