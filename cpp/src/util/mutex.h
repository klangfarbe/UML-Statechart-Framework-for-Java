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
#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

namespace statechart {
  namespace util {

/**
 * A Mutex to prevent threads from accessing shared resources
 * concurrently.
 */
    class Mutex {
//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
    public:
      /**
       * Default constructor
       */
      Mutex() : isLocked(false) {
        pthread_mutex_init(&mutex, 0);
      };

      /**
       * Default destructor
       */
      virtual ~Mutex() {
        pthread_mutex_destroy(&mutex);
      };

      /**
       * Locks the mutex
       */
      void lock() {
        pthread_mutex_lock(&mutex);
        isLocked = true;
      };

      /**
       * Stop the thread
       */
      void unlock() {
        isLocked = false;
        pthread_mutex_unlock(&mutex);
      };

      /**
       * Tries to lock the mutex and returns false if the lock could
       * not be accomplished.
       */
      bool tryLock() {
        return pthread_mutex_trylock(&mutex) == 0;
      };

      /**
       * Is the mutex locked?
       */
      bool locked() {
        return isLocked;
      }

//------------------------------------------------------------------------------
// Attributes
//------------------------------------------------------------------------------
    private:
      /// Mutex for the running variable
      pthread_mutex_t mutex;

      /// Flag if the mutex is locked
      bool isLocked;

      friend class Condition;
    };
  }
}

#endif

