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
#include <statechart/metadata.h>
#include <statechart/state.h>
#include <statechart/context.h>
#include <statechart/statechart.h>
#include <algorithm>

#ifdef STATECHART_DEBUG
#include <iostream>
#include <time.h>
#endif

using namespace std;
using namespace statechart;
using namespace statechart::util;

//------------------------------------------------------------------------------

Transition::Transition(State* start, State* end) {
  init(start, end, 0, 0, 0);
}

//------------------------------------------------------------------------------

Transition::Transition(State* start, State* end, Event* event) {
  init(start, end, event, 0, 0);
}

//------------------------------------------------------------------------------

Transition::Transition(State* start, State* end, Guard* guard) {
  init(start, end, 0, guard, 0);
}

//------------------------------------------------------------------------------

Transition::Transition(State* start, State* end, Action* action) {
  init(start, end, 0, 0, action);
}

//------------------------------------------------------------------------------

Transition::Transition(State* start, State* end, Event* event, Guard* guard) {
  init(start, end, event, guard, 0);
}

//------------------------------------------------------------------------------

Transition::Transition(State* start, State* end, Event* event, Action* action) {
  init(start, end, event, 0, action);
}

//------------------------------------------------------------------------------

Transition::Transition(State* start, State* end, Guard* guard, Action* action) {
  init(start, end, 0, guard, action);
}

//------------------------------------------------------------------------------

Transition::Transition(State* start, State* end, Event* event, Guard* guard, Action* action) {
  init(start, end, event, guard, action);
}

//------------------------------------------------------------------------------

Transition::~Transition() {
  delete event;
  delete guard;
  delete action;
  deactivate.clear();
  activate.clear();
}

//------------------------------------------------------------------------------

void Transition::init(State* start, State* end, Event* event, Guard* guard, Action* action) {
  this->event = event;
  this->guard = guard;
  this->action = action;
  Transition::calculateStateSet(start, end, deactivate, activate);
  start->addTransition(this);

  // for handling join states correctly, we need to know the incoming transition
  PseudoState* p = dynamic_cast<PseudoState*>(end);
  if(p && p->getType() == pseudostate_join) {
    p->addIncomingTransition(this);
  }
}

//------------------------------------------------------------------------------

bool Transition::execute(SmartPointer<Event>& event, 
                         SmartPointer<Metadata>& data, 
                         SmartPointer<Parameter>& parameter) {
  // check if the event can be handled  
  if(this->event && !this->event->equals(event, data, parameter)) {
    return false;
  }

  if(!this->event && event != 0) {
    return false;
  }

  if(!allowed(data, parameter)) {
    return false;
  }

  // deactivate all states
  for(unsigned int i = 0; i < deactivate.size(); i++) {
    deactivate[i]->deactivate(data, parameter);
  }

  // Execute exit-action
  if(action) {
    action->execute(data, parameter);
  }

  // Activate all new states.
  vector<State*>::iterator p;
  for(unsigned int i = 0; i < activate.size(); i++) {
    /* check if we activate an concurrent state imlicit and if so
     * make sure adding the correct region to the list of regions to
     * ignore on activation. It is activated by this transition.
     */
    if(i + 1 < activate.size() && dynamic_cast<ConcurrentState*>(activate[i])) {
      ConcurrentState* s = dynamic_cast<ConcurrentState*>(activate[i]);
      StateRuntimedata* cd = data->createRuntimedata(s);

      p = find(cd->stateset.begin(), cd->stateset.end(), activate[i + 1]);
      if(p == cd->stateset.end()) {
        cd->stateset.push_back(activate[i + 1]);
      }
    }

    activate[i]->activate(data, parameter);
  }

#ifdef STATECHART_DEBUG
  time_t rawtime;  
  struct tm* timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);

  cout  << timeinfo->tm_mday << "." << timeinfo->tm_mon << "." << timeinfo->tm_year + 1900 << " "
        << timeinfo->tm_hour << ":" << timeinfo->tm_min << ":" << timeinfo->tm_sec << " -"
        << " [Thread " << util::Thread::getId() << "] "
        << " Metadata " << spGetImpl(data) << ": "        
        << deactivate[0]->toString() << " (" << &deactivate[0] << ")"
        << " -> "
        << activate[activate.size() -1]->toString() << " (" << &activate[activate.size() -1] << ")\n";
#endif

  return true;
}

//------------------------------------------------------------------------------

bool Transition::allowed(SmartPointer<Metadata>& data, 
                         SmartPointer<Parameter>& parameter) {
  if(guard && !guard->check(data, parameter)) {
    return false;
  }

  /*
   * if target is a pseudostate, call lookup to check if we do not stay in this
   * state. So get the last state in the activate list. end() is behind the last
   * element so we have to select the prior element by using *--.
   */
  State* target = *--activate.end();
  PseudoState* ps = dynamic_cast<PseudoState*>(target);
  if(ps) {
    return ps->lookup(data, parameter);
  }

  return true;
}

//------------------------------------------------------------------------------

void Transition::calculateStateSet(State* start,
                                   State* end,
                                   vector<State*>& deactivate,
                                   vector<State*>& activate) {

  // temp vectors for calculating the LCA (least common ancestor)
  vector<State*> a;
  vector<State*> d;

  // get all states for possible deactivation
  State* s = start;
  while(s) {
    d.insert(d.begin(), s);
    Context* context = s->getContext();

    // If context is hierarchical or concurrent state, get it as parent.
    if(context && !dynamic_cast<Statechart*>(context)) {
      s = dynamic_cast<State*>(context);
    } else {
      s = 0;
    }
  }

  // get all states for possible activation
  State* e = end;
  while(e) {
    a.insert(a.begin(), e);
    Context* context = e->getContext();

    // If context is hierarchical or concurrent state, get it as parent.
    if(context && !dynamic_cast<Statechart*>(context)) {
      e = dynamic_cast<State*>(context);
    } else {
      e = 0;
    }
  }

  /*
   * Get LCA number. It is min-1 by default. Therefore we make sure that if start
   * equals end, we do not get the whole path up to the root node if the state
   * is a substate.
   */
  int min = a.size() < d.size() ? a.size() : d.size();
  int lca = min -1;

  // get the LCA-State
  if(start != end) {
    // if the first entry is not equal we got the LCA
    for(lca = 0; lca < min; lca++) {
      if(a[lca] != d[lca]) {
        break;
      }
    }
  }

  // Fill the given vectors for the transition
  for(unsigned int j = lca; j < d.size(); j++) {
    deactivate.insert(deactivate.begin(), d[j]);
  }

  for(unsigned int j = lca; j < a.size(); j++) {
    activate.push_back(a[j]);
  }
}

