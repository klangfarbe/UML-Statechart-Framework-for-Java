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
#include <statechart/timeoutevent.h>

using namespace statechart;
using namespace statechart::util;

//------------------------------------------------------------------------------

TimeoutEvent::TimeoutEvent(unsigned long _timeout) :
  timeout(_timeout) {
}

//------------------------------------------------------------------------------

bool TimeoutEvent::equals(SmartPointer<Event>& event, 
                          SmartPointer<Metadata>& data, 
                          SmartPointer<Parameter>& parameter) {
  TimeoutEvent* e = dynamic_cast<TimeoutEvent*>(spGetImpl(event));
  if(e) {
    return true;
  }
  return false;
}
