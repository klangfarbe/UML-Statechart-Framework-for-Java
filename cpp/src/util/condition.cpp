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
#include "condition.h"
#include "time.h"

#include <errno.h>

using namespace std;
using namespace statechart;
using namespace statechart::util;

//-----------------------------------------------------------------------------

Condition::Condition(Mutex& _mutex) : mutex(_mutex) {
  pthread_cond_init(&condition, NULL);
}

//-----------------------------------------------------------------------------

Condition::~Condition() {
  pthread_cond_destroy(&condition);
}

//-----------------------------------------------------------------------------

void Condition::wait() {
  pthread_cond_wait(&condition, &mutex.mutex);
}

//------------------------------------------------------------------------------

bool Condition::wait(long milliseconds) {
  if(milliseconds < 0) {
    return true;
  }

  Time t;
  struct timespec ts = t.getTimespec();
  ts.tv_sec += milliseconds / 1000;
  unsigned long ms = ((milliseconds % 1000) * 1000000) + ts.tv_nsec;
  ts.tv_nsec = ms % 1000000000;
  ts.tv_sec += ms / 1000000000;

  int rc = pthread_cond_timedwait(&condition, &mutex.mutex, &ts);
  if(rc == ETIMEDOUT) {
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------

void Condition::signal() {
  pthread_cond_signal(&condition);
}
