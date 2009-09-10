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
#ifndef THREAD
#define THREAD

#include <pthread.h>

#include <statechart/util/mutex.h>
#include <statechart/util/scopemutex.h>
#include <statechart/util/condition.h>

namespace statechart {
  namespace util {

/**
 * Represents a simple thread.
 */
    class Thread {
//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
    public:
      /**
       * Default constructor
       */
      Thread();

      /**
       * Default destructor
       */
      virtual ~Thread();

      /**
       * Starts the thread
       */
      void start();

      /**
       * Stop the thread
       */
      void shutdown();

      /**
       * Is the thread running?
       */
      bool isRunning() {
        return running;
      };

      static void sleep(long milliseconds);

      /**
       * Returns the thread id
       */
      static unsigned long int getId() {
        return pthread_self();
      };

      /**
       * Yields the thread
       */
      static void yield() {
        sched_yield();
      }

    protected:
      /**
       * The method the thread starts
       */
      virtual void run() = 0;

    private:
      /**
       * Method for the pthread_create to start the threads
       */
      static void* createThread(void* instance);

//------------------------------------------------------------------------------
// Attributes
//------------------------------------------------------------------------------
    private:
      /// is the thread running?
      bool running;

      /// The thread
      pthread_t thread;

      /// The thread attributes
      pthread_attr_t attr;

      /// Mutex for the running variable
      Mutex mutex;
    };
  }
}

#endif

