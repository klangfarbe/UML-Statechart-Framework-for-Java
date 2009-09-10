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
#ifndef TESTACTION_H
#define TESTACTION_H

#include <vector>
#include <string>

#include "../src/statechart/action.h"
#include "testmetadata.h"

using namespace std;
using namespace statechart;

/**
 * TestAction
 */
class TestAction : public Action {
public:
  TestAction(char* name, char* action) {
    this->name = name;
    this->action = action;
  }

  virtual void execute(util::SmartPointer<Metadata>& data, 
                       util::SmartPointer<Parameter>& param) {
    util::SmartPointer<TestMetadata> d(data);
    d->path.append((d->path.length() ? " " : "") + action + ":" + name);
  };

private:
  string name;
  string action;
};

#endif

