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
#ifndef GUARD_H
#define GUARD_H

#include <statechart/metadata.h>
#include <statechart/parameter.h>
#include <statechart/util/smartpointer.h>

namespace statechart {

/**
 * Interface for a guard which can be called by a transition.
 */
  class Guard : public util::IntrusiveSmartPointer {
  public:
    /**
     * Destructor, does nothing
     */
    virtual ~Guard() {};

    /**
     * Called by the transition.
     * @param data The runtime data object
     * @param param The parameter for this action
     */
    virtual bool check(util::SmartPointer<Metadata>& data, 
                       util::SmartPointer<Parameter>& param) = 0;
  };
}

#endif
