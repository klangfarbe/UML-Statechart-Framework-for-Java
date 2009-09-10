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
#ifndef TESTEVENT_H
#define TESTEVENT_H

#include "../src/statechart/event.h"
#include "testmetadata.h"

using namespace std;
using namespace statechart;

/**
 * Test Event
 */
class TestEvent : public Event {
public:
	TestEvent(int i) {
		this->i = i;
	}

	virtual bool equals(util::SmartPointer<Event>& event, 
                        util::SmartPointer<Metadata>& data, 
                        util::SmartPointer<Parameter>& param) {
		TestEvent* e = dynamic_cast<TestEvent*>(util::spGetImpl(event));
		if(e) {
			return e->getNumber() == i;
		}
		return false;
	};

	int getNumber() {
		return i;
	};

private:
	int i;
};

#endif

