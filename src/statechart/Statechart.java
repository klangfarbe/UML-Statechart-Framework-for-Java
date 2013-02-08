/*
 * UML statechart framework (http://github.com/klangfarbe/UML-Statechart-Framework-for-Java)
 *
 * Copyright (C) 2006-2013 Christian Mocek (christian.mocek@gmail.com)
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

import java.util.HashMap;
import java.util.Vector;
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

    //==========================================================================

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
  public static final String VERSION = "1.2.0";
  private ExecutorService threadpool = null;
  DelayQueue<EventQueueEntry> timeoutEventQueue = new DelayQueue<EventQueueEntry>();
  HashMap<String, State> states = new HashMap<String, State>();

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
    super(name, null, null, null, null);
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
   * Sets a new data object to continue from the given state. This is usually
   * needed for persistance to restore the state after a restart of the JVM
   * or if the data object was deleted sometime ago.
   */
  public boolean restoreState(State state, Metadata data, Parameter parameter) {
    if(data.isActive(this)) {
      return false;
    }
    data.reset();
    // get the path from the state to the root
    Vector<State> path = new Vector<State>();
    State parent = state;
    do {
      path.add(0, parent);
      parent = parent.context;
    } while(parent != null);

    for (State s : path) {
      s.activate(data, parameter);
    }
    return true;
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

  /**
   * Since every state must have a unique name, it is possible to get the
   * state object by name.
   */
  public final State getStateByName(String string) {
    return states.get(string);
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
