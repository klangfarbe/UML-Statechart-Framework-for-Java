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
#include "thread.h"

using namespace std;
using namespace statechart;
using namespace statechart::util;

//-----------------------------------------------------------------------------

Thread::Thread() : running(false), thread(0) {
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
}

//-----------------------------------------------------------------------------

Thread::~Thread() {
}

//-----------------------------------------------------------------------------

void Thread::start() {  
  ScopeMutex g(mutex);
  if(running) {
    return;
  }
  running = true;
  pthread_create(&thread, &attr, &Thread::createThread, this); 
}

//------------------------------------------------------------------------------

void Thread::shutdown() {
  ScopeMutex g(mutex);
  if(!running) {
    return;
  }
  running = false;
  pthread_attr_destroy(&attr);
  pthread_cancel(thread);
  pthread_join(thread, 0);
}

//------------------------------------------------------------------------------

void* Thread::createThread(void* instance) {
  ((Thread*)instance)->run();
  pthread_exit(0);
  return 0;
}

//------------------------------------------------------------------------------

void Thread::sleep(long milliseconds) {
  #ifdef WIN32
    Sleep(milliseconds);
  #else
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, 0);
  #endif
}
