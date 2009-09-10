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
#include "context.h"

using namespace statechart;

//------------------------------------------------------------------------------

Context::Context(Context* parent, Action* entryAction, Action* doAction, 
                 Action* exitAction, char* name) 
  : State(parent, entryAction, doAction, exitAction, name), startState(0) {
}

//------------------------------------------------------------------------------

Context::~Context() {
  // trigger destruction of the subtree
  for(unsigned int i = 0; i < substates.size(); i++) {
    delete substates[i];
    substates[i] = 0;
  }
  substates.clear();
  startState = 0;
}

//------------------------------------------------------------------------------

void Context::addSubstate(State* state) {
  substates.push_back(state);
}
