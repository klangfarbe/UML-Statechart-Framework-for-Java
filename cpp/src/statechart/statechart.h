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
#ifndef STATECHART_H
#define STATECHART_H

// Includes are just for convenience. So just this header must be included.
#include <statechart/action.h>
#include <statechart/concurrentstate.h>
#include <statechart/context.h>
#include <statechart/event.h>
#include <statechart/eventqueue.h>
#include <statechart/finalstate.h>
#include <statechart/guard.h>
#include <statechart/hierarchicalstate.h>
#include <statechart/metadata.h>
#include <statechart/parameter.h>
#include <statechart/pseudostate.h>
#include <statechart/state.h>
#include <statechart/statechartexception.h>
#include <statechart/timeoutevent.h>
#include <statechart/timeouteventqueue.h>
#include <statechart/transition.h>

#include <statechart/util/thread.h>
#include <statechart/util/smartpointer.h>

namespace statechart {
/**
 * The main entry point for using the statechart framework. Contains all
 * necessary methods for delegating incoming events to the substates.
 * When deleting the statechart all substates, actions, events, guards and
 * transition will be deleted automatically.
 */
  class Statechart : public Context {
//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
  public:
    /**
     * Creates the Statechart
     */
    Statechart(unsigned int numberOfWorkerThreads = 5, 
               unsigned int numberOfTimeoutWorkerThreads = 5);

    /**
     * Synchronizes the queue threads
     */
    virtual ~Statechart();

    /**
     * Initializes the Statechart in the runtime data. Sets the start state.
     */
    bool start(util::SmartPointer<Metadata> data);

    /**
     * Initializes the Statechart in the runtime data. Sets the start state.
     */
    bool start(util::SmartPointer<Metadata> data,
               util::SmartPointer<Parameter> parameter);

    /**
     * Overrides the dispatch method from the state and takes care of delegating
     * the incoming event to the current state.
     */
    bool dispatch(util::SmartPointer<Metadata> data,
                  util::SmartPointer<Event> event); 

    /**
     * Overrides the dispatch method from the state and takes care of delegating
     * the incoming event to the current state.
     */
    bool dispatch(util::SmartPointer<Metadata> data, 
                  util::SmartPointer<Event> event, 
                  util::SmartPointer<Parameter> parameter);

    /**
     * Initializes the Statechart in the runtime data. Sets the start state 
     * and triggers an initial asynchronous dispatch.
     */
    void startAsynchron(util::SmartPointer<Metadata> data);

    /**   
     * Initializes the Statechart in the runtime data. Sets the start state 
     * and triggers an initial asynchronous dispatch.
     */
    void startAsynchron(util::SmartPointer<Metadata> data, 
                        util::SmartPointer<Parameter> parameter);

    /**
     * Adds an event to the event queue.
     */  
    void dispatchAsynchron(util::SmartPointer<Metadata> data, 
                           util::SmartPointer<Event> event); 

    /**
     * Adds an event to the event queue.
     */  
    void dispatchAsynchron(util::SmartPointer<Metadata> data, 
                           util::SmartPointer<Event> event, 
                           util::SmartPointer<Parameter> parameter);


//------------------------------------------------------------------------------
// Attributes
//------------------------------------------------------------------------------
  private:
    /// The event queue
    EventQueue eventQueue;

    /// The event queue for timeout events. Is used only internally
    TimeoutEventQueue timeoutEventQueue;

    friend class State;
  };
}

#endif




