/*
 * UML statechart framework (http://uml-statecharts.sourceforge.net)
 *
 * Copyright (C) 2006-2009 Christian Mocek (klangfarbe@users.sourceforge.net)
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
package statechart;

import java.util.concurrent.DelayQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.TimeUnit;

/**
 * The main entry point for using the statechart framework. Contains all
 * necessary methods for delegating incoming events to the substates. When
 * deleting the statechart all substates, actions, events, guards and transition
 * will be deleted automatically.
 */
public class Statechart extends Context implements Runnable {
  /**
   * Creates the threads for the ThreadPoolExecutor. These threads may be daemon
   * or non-daemon threads.
   */
  class StatechartThreadFactory implements ThreadFactory {
    //============================================================================
    // ATTRIBUTES
    //============================================================================
    private String id;
    private int threadCount = 0;
    private boolean makeDaemonThreads = false;
    private ThreadGroup threadGroup = null;
    
    //============================================================================
    // METHODS
    //============================================================================
    public StatechartThreadFactory(String id, boolean makeDaemonThreads) {
      this.makeDaemonThreads = makeDaemonThreads;
      this.id = id;
      threadGroup = new ThreadGroup(id);
    }
    
    // =========================================================================
    
    public Thread newThread(Runnable r) {
      String name = null;
      synchronized (this) {
        ++threadCount;
        name = id + "-thread-" + threadCount;
      }
      Thread t = new Thread(threadGroup, r, name);
      t.setDaemon(makeDaemonThreads);
      return t;
    }
  }
  
  //============================================================================
  // ATTRIBUTES
  //============================================================================
  public static final String VERSION = "1.0.0";
  private ExecutorService threadpool = null;
  DelayQueue<EventQueueEntry> timeoutEventQueue = new DelayQueue<EventQueueEntry>();

  //============================================================================
  // METHODS
  //============================================================================
  /**
   * Creates the Statechart with the given default and maximum number of
   * threads.
   * 
   * @param name The name of the statechart. This must be unique for all
   *          statecharts in the running JVM.
   * @param threads The maximum number of threads available in the
   *          threadpool.
   * @param makeDaemonThreads Specifies if the created threads should be daemon
   *          or non-daemon threads.
   * @throws StatechartException
   */
  public Statechart(String name, int threads, boolean makeDaemonThreads) throws StatechartException {
    super(null, null, null, null);
    this.name = name;
    // we need at least two threads for asynchronous and timeout events
    if(threads < 2) {
      threads = 2; 
    }
    threadpool = Executors.newFixedThreadPool(threads, new StatechartThreadFactory(name, makeDaemonThreads));
    threadpool.execute(this);
  }
  
  //============================================================================

  /**
   * Shutdown of the threadpool. The pool waits 60 seconds at most before 
   * shutting down hard.
   * 
   * @throws InterruptedException 
   */
  public synchronized void shutdown() {
    threadpool.shutdown();    
    timeoutEventQueue.clear();
    // send an empty event to the queue to get back from the delay queues take method
    timeoutEventQueue.add(new EventQueueEntry(this, this, null, null, null, 0));
    try {
      threadpool.awaitTermination(60, TimeUnit.SECONDS);
    } catch (InterruptedException e) {
      // ignore and just exit the method
    }
  }

  //============================================================================

  /**
   * Initializes the Statechart in the runtime data. Sets the start state.
   */
  public boolean start(Metadata data) {
    Parameter parameter = new Parameter();
    return start(data, parameter);
  }

  //============================================================================

  /**
   * Initializes the Statechart in the runtime data. Sets the start state.
   */
  public boolean start(Metadata data, Parameter parameter) {
    data.reset();
    data.activate(this);
    data.activate(startState);
    return dispatch(data, null, parameter);
  }

  //============================================================================

  /**
   * Calls the dispatch method on the current state.
   */
  public boolean dispatch(Metadata data, Event event) {
    Parameter parameter = new Parameter();
    return dispatch(data, event, parameter);
  }

  //============================================================================

  /**
   * Overrides the dispatch method from the state and takes care of delegating
   * the incoming event to the current state.
   */
  public boolean dispatch(Metadata data, Event event, Parameter parameter) {
    boolean rc = false;
    synchronized (data) {
      State currentState = data.getData(this).currentState;
      rc = currentState.dispatch(data, event, parameter);

      // call dispatch as long as we hit states with end transitions
      do {
        currentState = data.getData(this).currentState;
      } while (currentState != null && currentState.dispatch(data, null, parameter));
    }
    return rc;
  }

  //============================================================================

  /**
   * Initializes the Statechart in the runtime data. Sets the start state and
   * triggers an initial asynchronous dispatch.
   */
  public void startAsynchron(Metadata data) {
    Parameter parameter = new Parameter();
    startAsynchron(data, parameter);
  }

  //============================================================================

  /**
   * Initializes the Statechart in the runtime data. Sets the start state and
   * triggers an initial asynchronous dispatch.
   */
  public void startAsynchron(Metadata data, Parameter parameter) {
    data.reset();
    data.activate(this);
    data.activate(startState);
    dispatchAsynchron(data, null, parameter);
  }

  //============================================================================

  /**
   * Adds an event to the event queue.
   */
  public void dispatchAsynchron(Metadata data, Event event) {
    Parameter parameter = new Parameter();
    dispatchAsynchron(data, event, parameter);
  }

  //============================================================================

  /**
   * Adds an event to the event queue.
   */
  public void dispatchAsynchron(Metadata data, Event event, Parameter parameter) {
    if (!threadpool.isShutdown()) {
      if (event instanceof TimeoutEvent) {
        timeoutEventQueue.add(new EventQueueEntry(this, this, data, event, parameter,
                                                  ((TimeoutEvent)event).getTimout()));
      } else {
        threadpool.execute(new EventQueueEntry(this, this, data, event, parameter, 0));
      }
    }
  }

  //============================================================================
  // Inherited by Runnable
  //============================================================================
  /**
   * Dequeues elements from the timeout queue and dispatches them
   */
  public void run() {
    while (!threadpool.isShutdown()) {
      try {
        EventQueueEntry event = timeoutEventQueue.take();
        threadpool.execute(event);
      } catch (InterruptedException e) {
        // ignore the exception. Just run the next loop and take method if
        // necessary
      } catch (RejectedExecutionException e) {
        // Normally this means that the threadpool has been shutted down  
      }      
    }
  }
}
