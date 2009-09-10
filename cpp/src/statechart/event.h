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
#ifndef EVENT_H
#define EVENT_H

#include <statechart/metadata.h>
#include <statechart/parameter.h>
#include <statechart/util/smartpointer.h>

#include <string>

namespace statechart {

#define EVENT(ID) class ID : public statechart::Event { \
public: \
  ID() : statechart::Event(#ID) {}; \
};

  class Event;

/**
 * Interface for an event for the statechart.
 */
  class Event : public util::IntrusiveSmartPointer {
  private:
    /// ID
    const std::string id;

  public:
    /**
     * Creates a simple event having no name. Warning, the
     * default equals implementation will not work without an
     * id. Use the EVENT macro instead.
     */
    Event() {
    };

    /**
     * Create an event with a name
     */
    Event(const char* id_) :id(id_) {
    };

    /**
     * Destructor, does nothing
     */
    virtual ~Event() {
    };

    /**
     * Called by the transition to check if it should handle this event.
     * @param event the event to compare
     * @param data The runtime data object
     * @param param The parameter for this action
     */
    virtual bool equals(util::SmartPointer<Event>& event, 
                        util::SmartPointer<Metadata>& data, 
                        util::SmartPointer<Parameter>& param) {
      return !event ? false : id == event->toString();
    };

    const std::string& toString() const {
      return id;
    };
  };
}

#endif
