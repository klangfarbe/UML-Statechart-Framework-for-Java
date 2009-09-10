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
#ifndef TIME
#define TIME

#include <sys/time.h>
#include <pthread.h>

namespace statechart {
  namespace util {

/**
 * Helper class to get the current system time in milliseconds. 
 */
    class Time {
//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
    public:
      /**
       * Returns the current system time in milliseconds
       */
      static unsigned long now() {
        struct timespec ts;
        struct timeval  tp;

        gettimeofday(&tp, 0);
        ts.tv_sec  = tp.tv_sec;
        ts.tv_nsec = tp.tv_usec * 1000; 
        return tp.tv_sec * 1000 + tp.tv_usec / 1000;
      };

      /**
       * Returns the current system time as a timespec structure
       */
      static timespec getTimespec() {
        struct timespec ts;
        struct timeval  tp;

        gettimeofday(&tp, 0);
        ts.tv_sec  = tp.tv_sec;
        ts.tv_nsec = tp.tv_usec * 1000; 
        return ts;
      };
    };
  }
}

#endif

