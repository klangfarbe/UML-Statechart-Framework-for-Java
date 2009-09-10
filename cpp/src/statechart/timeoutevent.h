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
#ifndef TIMEOUT_EVENT_H
#define TIMEOUT_EVENT_H

#include "event.h"
#include "metadata.h"
#include "parameter.h"

namespace statechart {

/**
 * Timeout event for a transition
 */
  class TimeoutEvent : public Event {
//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
  public:
    /**
     * Creates a timeout event.
     * @param timeout The timeout value in milliseconds
     */
    TimeoutEvent(unsigned long timeout);

    /**
     * Checks if the timevalue in the given parameter is larger than the one
     * stored in this event.
     *
     * @param event the event to compare
     * @param data The runtime data object
     * @param param The parameter for this event. Must be a TimeoutParameter type.
     */
    virtual bool equals(util::SmartPointer<Event>& event, 
                        util::SmartPointer<Metadata>& data, 
                        util::SmartPointer<Parameter>& parameter);

    /**
     * Returns the timeout
     */
    unsigned long getTimeout() {
      return timeout;
    }

//------------------------------------------------------------------------------
// Attributes
//------------------------------------------------------------------------------
  private:
    /// The timeout value in milliseconds
    unsigned long timeout;
  };
}

#endif // EVENT_H

