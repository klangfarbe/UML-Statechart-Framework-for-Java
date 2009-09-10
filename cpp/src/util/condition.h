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
#ifndef CONDITION_H
#define CONDITION_H

#include <pthread.h>

#include "mutex.h"

namespace statechart {
  namespace util {

/**
 * Represents a simple thread.
 */
    class Condition {
//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
    public:
      /**
       * Default constructor
       */
      Condition(Mutex& _mutex);

      /**
       * Default destructor
       */
      virtual ~Condition();

      /**
       * Wait until a signal is send or the thread received a shutdown
       */
      void wait();

      /**
       * Wait until a signal is send or the thread received a shutdown
       * or the amount of milliseconds timed out.
       * 
       * @return bool true if an timeout occurs otherwise false;
       */
      bool wait(long milliseconds);

      /**
       * Signals the condition
       */
      void signal();

//------------------------------------------------------------------------------
// Attributes
//------------------------------------------------------------------------------
    private:
      /// The mutex to wait on
      Mutex& mutex;

      /// The condition
      pthread_cond_t condition;
    };
  }
}

#endif

