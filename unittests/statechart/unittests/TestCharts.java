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
package statechart.unittests;

import statechart.ConcurrentState;
import statechart.FinalState;
import statechart.HierarchicalState;
import statechart.PseudoState;
import statechart.State;
import statechart.Statechart;
import statechart.StatechartException;
import statechart.TimeoutEvent;
import statechart.Transition;

public class TestCharts {
  static Statechart t1() throws StatechartException {
    Statechart chart = new Statechart("t1", 10, false);
    State s1 = new State("a", chart, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", chart, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    new Transition(p1, s1);
    new Transition(s1, s2);
    new Transition(s2, p2);
    return chart;
  }

  static Statechart t2() throws StatechartException {
    Statechart chart = new Statechart("t2", 10, false);
    State s1 = new State("a", chart, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    new Transition(p1, s1);
    new Transition(s1, s1, new TestEvent(1));
    new Transition(s1, p2, new TestEvent(2));

    s1.setName("s1");
    p1.setName("p1");
    p2.setName("p3");
    return chart;
  }

  static Statechart t3() throws StatechartException {
    Statechart chart = new Statechart("t3", 10, false);

    State s1 = new State("a", chart, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", chart, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    new Transition(p1, s1);
    new Transition(s1, s2, new TestEvent(1));
    new Transition(s1, p2, new TimeoutEvent(1000));
    new Transition(s2, p2);

    return chart;
  }

  static Statechart t4() throws StatechartException {
    Statechart chart = new Statechart("t4", 10, false);

    State s1 = new State("a", chart, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", chart, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State s3 = new State("c", chart, new TestAction("c", "A"), null, new TestAction("c", "D"));

    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    PseudoState j1 = new PseudoState("j1", chart, PseudoState.pseudostate_junction);
    PseudoState j2 = new PseudoState("j2", chart, PseudoState.pseudostate_junction);
    PseudoState j3 = new PseudoState("j3", chart, PseudoState.pseudostate_junction);

    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));

    j1.setEntryAction(new TestAction("j1", "A"));
    j1.setExitAction(new TestAction("j1", "D"));
    j2.setEntryAction(new TestAction("j2", "A"));
    j2.setExitAction(new TestAction("j2", "D"));
    j3.setEntryAction(new TestAction("j3", "A"));
    j3.setExitAction(new TestAction("j3", "D"));

    new Transition(p1, s1);
    new Transition(s1, p2, new TestGuard(0));
    new Transition(s1, j1);
    new Transition(j1, s2, new TestGuard(1), new TestAction("a1", "E"));
    new Transition(j1, s3, new TestGuard(2), new TestAction("a2", "E"));
    new Transition(s2, p2);
    new Transition(s3, j2);
    new Transition(j2, j3, new TestAction("a3", "E"));
    new Transition(j3, p2, new TestAction("a4", "E"));

    return chart;
  }

  static Statechart h1() throws StatechartException {
    Statechart chart = new Statechart("h1", 10, false);
    HierarchicalState h1 = new HierarchicalState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    State s1 = new State("a", h1, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", h1, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    State p3 = new PseudoState("start p", h1, PseudoState.pseudostate_start);
    State p4 = new FinalState("end p", h1);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    p3.setEntryAction(new TestAction("start p", "A"));
    p3.setExitAction(new TestAction("start p", "D"));
    p4.setEntryAction(new TestAction("end p", "A"));
    p4.setExitAction(new TestAction("end p", "D"));
    new Transition(p1, h1);
    new Transition(h1, p2);
    new Transition(p3, s1);
    new Transition(s1, s2);
    new Transition(s2, p4);

    return chart;
  }

  static Statechart h2() throws StatechartException {
    Statechart chart = new Statechart("h2", 10, false);
    HierarchicalState h1 = new HierarchicalState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    State s1 = new State("a", h1, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", h1, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    State p3 = new PseudoState("start p", h1, PseudoState.pseudostate_start);
    State p4 = new FinalState("end p", h1);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    p3.setEntryAction(new TestAction("start p", "A"));
    p3.setExitAction(new TestAction("start p", "D"));
    p4.setEntryAction(new TestAction("end p", "A"));
    p4.setExitAction(new TestAction("end p", "D"));
    new Transition(p1, h1);
    new Transition(h1, p2);
    new Transition(h1, p2, new TestEvent(1));
    new Transition(p3, s1);
    new Transition(s1, s2, new TestEvent(1));
    new Transition(s2, p4);

    return chart;
  }

  static Statechart h3() throws StatechartException {
    Statechart chart = new Statechart("h3", 10, false);
    HierarchicalState h1 = new HierarchicalState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    State s1 = new State("a", h1, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", h1, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    State p3 = new PseudoState("start p", h1, PseudoState.pseudostate_start);
    State p4 = new FinalState("end p", h1);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    p3.setEntryAction(new TestAction("start p", "A"));
    p3.setExitAction(new TestAction("start p", "D"));
    p4.setEntryAction(new TestAction("end p", "A"));
    p4.setExitAction(new TestAction("end p", "D"));
    new Transition(p1, h1);
    new Transition(h1, p2);
    new Transition(h1, p2, new TestEvent(2));
    new Transition(p3, s1);
    new Transition(s1, s2, new TestEvent(1));
    new Transition(s2, p4);

    return chart;
  }

  static Statechart h4() throws StatechartException {
    Statechart chart = new Statechart("h4", 10, false);
    HierarchicalState h1 = new HierarchicalState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    State s1 = new State("a", h1, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", h1, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    State p3 = new PseudoState("start p", h1, PseudoState.pseudostate_start);
    State p4 = new FinalState("end p", h1);
    State hi = new PseudoState("history p", h1, PseudoState.pseudostate_history);
    hi.setExitAction(new TestAction("history p", "U"));
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    p3.setEntryAction(new TestAction("start p", "A"));
    p3.setExitAction(new TestAction("start p", "D"));
    p4.setEntryAction(new TestAction("end p", "A"));
    p4.setExitAction(new TestAction("end p", "D"));
    new Transition(p1, h1);
    new Transition(h1, h1, new TestEvent(1));
    new Transition(h1, p2);
    new Transition(p3, hi);
    new Transition(hi, s1);
    new Transition(s1, s2, new TestEvent(2));
    new Transition(s2, s1, new TestEvent(2));
    new Transition(s2, p4, new TestEvent(3));
    return chart;
  }

  static Statechart h5() throws StatechartException {
    Statechart chart = new Statechart("h5", 10, false);
    HierarchicalState h1 = new HierarchicalState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    HierarchicalState h2 = new HierarchicalState("q", h1, new TestAction("q", "A"), null, new TestAction("q", "D"));
    State s1 = new State("a", h1, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", h2, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State s3 = new State("c", h2, new TestAction("c", "A"), null, new TestAction("c", "D"));
    State s4 = new State("d", chart, new TestAction("d", "A"), null, new TestAction("d", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    State p3 = new PseudoState("start p", h1, PseudoState.pseudostate_start);
    State p4 = new FinalState("end p", h1);
    State hi = new PseudoState("history p", h1, PseudoState.pseudostate_deep_history);
    State p5 = new PseudoState("start q", h2, PseudoState.pseudostate_start);
    hi.setExitAction(new TestAction("history p", "U"));
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    p3.setEntryAction(new TestAction("start p", "A"));
    p3.setExitAction(new TestAction("start p", "D"));
    p4.setEntryAction(new TestAction("end p", "A"));
    p4.setExitAction(new TestAction("end p", "D"));
    p5.setEntryAction(new TestAction("start q", "A"));
    p5.setExitAction(new TestAction("start q", "D"));
    new Transition(p1, h1);
    new Transition(h1, h1, new TestEvent(1));
    new Transition(h1, p2);
    new Transition(p3, hi);
    new Transition(hi, s1);
    new Transition(s1, h2, new TestEvent(2));
    new Transition(h2, s1, new TestEvent(2));
    new Transition(h2, p4, new TestEvent(3));
    new Transition(p5, s2);
    new Transition(s2, s3, new TestEvent(4));
    new Transition(s3, s2, new TestEvent(4));
    new Transition(s3, s4, new TestEvent(5));
    new Transition(s4, h1);

    return chart;
  }

  static Statechart h6() throws StatechartException {
    Statechart chart = new Statechart("h6", 10, false);
    HierarchicalState h1 = new HierarchicalState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    HierarchicalState h2 = new HierarchicalState("q", h1, new TestAction("q", "A"), null, new TestAction("q", "D"));
    HierarchicalState h3 = new HierarchicalState("x", chart, new TestAction("x", "A"), null, new TestAction("x", "D"));
    State s1 = new State("a", h2, new TestAction("r", "A"), null, new TestAction("r", "D"));
    State s2 = new State("b", h3, new TestAction("y", "A"), null, new TestAction("y", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    new Transition(p1, s1);
    new Transition(s1, s2);
    new Transition(s2, p2);

    return chart;
  }

  static Statechart c1() throws StatechartException
  {
    Statechart chart = new Statechart("c1", 10, false);
    ConcurrentState n1 = new ConcurrentState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    HierarchicalState h1 = new HierarchicalState("p-r1", n1, new TestAction("p-r1", "A"), null, new TestAction("p-r1", "D"));
    HierarchicalState h2 = new HierarchicalState("p-r2", n1, new TestAction("p-r2", "A"), null, new TestAction("p-r2", "D"));
    State s1 = new State("a", h1, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", h2, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    State p3 = new PseudoState("start p-r1", h1, PseudoState.pseudostate_start);
    State p4 = new FinalState("end p-r1", h1);
    State p5 = new PseudoState("start p-r2", h2, PseudoState.pseudostate_start);
    State p6 = new FinalState("end p-r2", h2);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    p3.setEntryAction(new TestAction("start p-r1", "A"));
    p3.setExitAction(new TestAction("start p-r1", "D"));
    p4.setEntryAction(new TestAction("end p-r1", "A"));
    p4.setExitAction(new TestAction("end p-r1", "D"));
    p5.setEntryAction(new TestAction("start p-r2", "A"));
    p5.setExitAction(new TestAction("start p-r2", "D"));
    p6.setEntryAction(new TestAction("end p-r2", "A"));
    p6.setExitAction(new TestAction("end p-r2", "D"));
    new Transition(p1, n1);
    new Transition(n1, p2);
    new Transition(p3, s1);
    new Transition(s1, p4);
    new Transition(p5, s2);
    new Transition(s2, p6);

    return chart;
  }

  static Statechart c2() throws StatechartException
  {
    Statechart chart = new Statechart("c2", 10, false);
    ConcurrentState n1 = new ConcurrentState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    HierarchicalState h1 = new HierarchicalState("p-r1", n1, new TestAction("p-r1", "A"), null, new TestAction("p-r1", "D"));
    HierarchicalState h2 = new HierarchicalState("p-r2", n1, new TestAction("p-r2", "A"), null, new TestAction("p-r2", "D"));
    State s1 = new State("a", h1, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", h1, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State s3 = new State("c", h2, new TestAction("c", "A"), null, new TestAction("c", "D"));
    State s4 = new State("d", h2, new TestAction("d", "A"), null, new TestAction("d", "D"));
    State s5 = new State("e", h2, new TestAction("e", "A"), null, new TestAction("e", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    State p3 = new PseudoState("start p-r1", h1, PseudoState.pseudostate_start);
    State p4 = new FinalState("end p-r1", h1);
    State p5 = new PseudoState("start p-r2", h2, PseudoState.pseudostate_start);
    State p6 = new FinalState("end p-r2", h2);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    p3.setEntryAction(new TestAction("start p-r1", "A"));
    p3.setExitAction(new TestAction("start p-r1", "D"));
    p4.setEntryAction(new TestAction("end p-r1", "A"));
    p4.setExitAction(new TestAction("end p-r1", "D"));
    p5.setEntryAction(new TestAction("start p-r2", "A"));
    p5.setExitAction(new TestAction("start p-r2", "D"));
    p6.setEntryAction(new TestAction("end p-r2", "A"));
    p6.setExitAction(new TestAction("end p-r2", "D"));
    new Transition(p1, n1);
    new Transition(n1, p2);
    new Transition(n1, p2, new TestEvent(3));
    new Transition(p3, s1);
    new Transition(s1, s2, new TestEvent(2));
    new Transition(s2, p4);
    new Transition(p5, s3);
    new Transition(s3, s4, new TestEvent(1));
    new Transition(s4, s5, new TestEvent(2));
    new Transition(s5, p6);

    return chart;
  }

  static Statechart c3() throws StatechartException
  {
    Statechart chart = new Statechart("c3", 10, false);
    ConcurrentState n1 = new ConcurrentState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    HierarchicalState h1 = new HierarchicalState("p-r1", n1, new TestAction("p-r1", "A"), null, new TestAction("p-r1", "D"));
    HierarchicalState h2 = new HierarchicalState("p-r2", n1, new TestAction("p-r2", "A"), null, new TestAction("p-r2", "D"));
    State s1 = new State("a", h1, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", h2, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    State p3 = new FinalState("end p-r1", h1);
    State p4 = new PseudoState("start p-r2", h2, PseudoState.pseudostate_start);
    State p5 = new FinalState("end p-r2", h2);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    p3.setEntryAction(new TestAction("end p-r1", "A"));
    p3.setExitAction(new TestAction("end p-r1", "D"));
    p4.setEntryAction(new TestAction("start p-r2", "A"));
    p4.setExitAction(new TestAction("start p-r2", "D"));
    p5.setEntryAction(new TestAction("end p-r2", "A"));
    p5.setExitAction(new TestAction("end p-r2", "D"));
    new Transition(p1, s1);
    new Transition(n1, p2);
    new Transition(s1, p3);
    new Transition(p4, s2);
    new Transition(s2, p5);
    return chart;
  }

  static Statechart c4() throws StatechartException
  {
    Statechart chart = new Statechart("c4", 10, false);
    ConcurrentState n1 = new ConcurrentState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    HierarchicalState h1 = new HierarchicalState("p-r1", n1, new TestAction("p-r1", "A"), null, new TestAction("p-r1", "D"));
    HierarchicalState h2 = new HierarchicalState("p-r2", n1, new TestAction("p-r2", "A"), null, new TestAction("p-r2", "D"));
    State s1 = new State("a", h1, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", h2, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State s3 = new State("c", h2, new TestAction("c", "A"), null, new TestAction("c", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    State p3 = new PseudoState("start p-r1", h1, PseudoState.pseudostate_start);
    State p4 = new PseudoState("start p-r2", h2, PseudoState.pseudostate_start);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    p3.setEntryAction(new TestAction("start p-r1", "A"));
    p3.setExitAction(new TestAction("start p-r1", "D"));
    p4.setEntryAction(new TestAction("start p-r2", "A"));
    p4.setExitAction(new TestAction("start p-r2", "D"));
    new Transition(p1, n1);
    new Transition(p3, s1);
    new Transition(s1, p2);
    new Transition(p4, s2);
    new Transition(s2, s3);
    return chart;
  }

  static Statechart c5() throws StatechartException
  {
    Statechart chart = new Statechart("c5", 10, false);
    ConcurrentState n1 = new ConcurrentState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    HierarchicalState h1 = new HierarchicalState("p-r1", n1, new TestAction("p-r1", "A"), null, new TestAction("p-r1", "D"));
    HierarchicalState h2 = new HierarchicalState("p-r2", n1, new TestAction("p-r2", "A"), null, new TestAction("p-r2", "D"));
    State s1 = new State("a", h1, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", h2, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State s3 = new State("c", h2, new TestAction("c", "A"), null, new TestAction("c", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    State p3 = new PseudoState("start p-r1", h1, PseudoState.pseudostate_start);
    State p4 = new PseudoState("start p-r2", h2, PseudoState.pseudostate_start);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    p3.setEntryAction(new TestAction("start p-r1", "A"));
    p3.setExitAction(new TestAction("start p-r1", "D"));
    p4.setEntryAction(new TestAction("start p-r2", "A"));
    p4.setExitAction(new TestAction("start p-r2", "D"));
    new Transition(p1, n1);
    new Transition(p3, s1);
    new Transition(s1, p2, new TestEvent(1));
    new Transition(p4, s2);
    new Transition(s2, s3, new TestEvent(2));
    return chart;
  }

  static Statechart c6() throws StatechartException
  {
    Statechart chart = new Statechart("c6", 10, false);
    ConcurrentState n1 = new ConcurrentState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    HierarchicalState h1 = new HierarchicalState("p-r1", n1, new TestAction("p-r1", "A"), null, new TestAction("p-r1", "D"));
    HierarchicalState h2 = new HierarchicalState("p-r2", n1, new TestAction("p-r2", "A"), null, new TestAction("p-r2", "D"));
    State s1 = new State("a", h1, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", h2, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    State p3 = new FinalState("end p-r1", h1);
    State p4 = new FinalState("end p-r2", h2);
    State p5 = new PseudoState("fork", chart, PseudoState.pseudostate_fork);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    p3.setEntryAction(new TestAction("end p-r1", "A"));
    p3.setExitAction(new TestAction("end p-r1", "D"));
    p4.setEntryAction(new TestAction("end p-r2", "A"));
    p4.setExitAction(new TestAction("end p-r2", "D"));
    p5.setEntryAction(new TestAction("fork", "A"));
    p5.setExitAction(new TestAction("fork", "D"));
    new Transition(p1, p5);
    new Transition(p5, s1);
    new Transition(p5, s2);
    new Transition(s1, p3);
    new Transition(s2, p4);
    new Transition(n1, p2);
    return chart;
  }

  static Statechart c7() throws StatechartException
  {
    Statechart chart = new Statechart("c7", 10, false);
    ConcurrentState n1 = new ConcurrentState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    HierarchicalState h1 = new HierarchicalState("p-r1", n1, new TestAction("p-r1", "A"), null, new TestAction("p-r1", "D"));
    HierarchicalState h2 = new HierarchicalState("p-r2", n1, new TestAction("p-r2", "A"), null, new TestAction("p-r2", "D"));
    HierarchicalState h3 = new HierarchicalState("p-r3", n1, new TestAction("p-r3", "A"), null, new TestAction("p-r3", "D"));
    State s1 = new State("a", h1, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", h2, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State s3 = new State("c", h3, new TestAction("c", "A"), null, new TestAction("c", "D"));
    State s4 = new State("d", h3, new TestAction("d", "A"), null, new TestAction("d", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    State p3 = new FinalState("end p-r1", h1);
    State p4 = new FinalState("end p-r2", h2);
    State p5 = new PseudoState("fork", chart, PseudoState.pseudostate_fork);
    State p6 = new PseudoState("start p-r3", h3, PseudoState.pseudostate_start);
    State p7 = new FinalState("end p-r3", h3);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    p3.setEntryAction(new TestAction("end p-r1", "A"));
    p3.setExitAction(new TestAction("end p-r1", "D"));
    p4.setEntryAction(new TestAction("end p-r2", "A"));
    p4.setExitAction(new TestAction("end p-r2", "D"));
    p5.setEntryAction(new TestAction("fork", "A"));
    p5.setExitAction(new TestAction("fork", "D"));
    p6.setEntryAction(new TestAction("start p-r3", "A"));
    p6.setExitAction(new TestAction("start p-r3", "D"));
    p7.setEntryAction(new TestAction("end p-r3", "A"));
    p7.setExitAction(new TestAction("end p-r3", "D"));
    new Transition(p1, p5);
    new Transition(p5, s1);
    new Transition(p5, s2);
    new Transition(p5, s3, new TestGuard(1));
    new Transition(s1, p3);
    new Transition(s2, p4);
    new Transition(p6, s4);
    new Transition(s3, p7);
    new Transition(s4, p7);
    new Transition(n1, p2);
    return chart;
  }

  static Statechart c8() throws StatechartException
  {
    Statechart chart = new Statechart("x8", 10, false);
    ConcurrentState n1 = new ConcurrentState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    HierarchicalState h1 = new HierarchicalState("p-r1", n1, new TestAction("p-r1", "A"), null, new TestAction("p-r1", "D"));
    HierarchicalState h2 = new HierarchicalState("p-r2", n1, new TestAction("p-r2", "A"), null, new TestAction("p-r2", "D"));
    State s1 = new State("a", h1, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", h2, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State s3 = new State("c", h2, new TestAction("c", "A"), null, new TestAction("c", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    State p3 = new PseudoState("start p-r1", h1, PseudoState.pseudostate_start);
    State p4 = new PseudoState("start p-r2", h2, PseudoState.pseudostate_start);
    PseudoState p5 = new PseudoState("join", chart, PseudoState.pseudostate_join);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    p3.setEntryAction(new TestAction("start p-r1", "A"));
    p3.setExitAction(new TestAction("start p-r1", "D"));
    p4.setEntryAction(new TestAction("start p-r2", "A"));
    p4.setExitAction(new TestAction("start p-r2", "D"));
    p5.setEntryAction(new TestAction("join", "A"));
    p5.setExitAction(new TestAction("join", "D"));
    new Transition(p1, n1);
    new Transition(p3, s1);
    new Transition(p4, s2);
    new Transition(s2, s3);
    new Transition(s1, p5);
    new Transition(s3, p5);
    new Transition(p5, p2);
    return chart;
  }

  static Statechart c9() throws StatechartException
  {
    Statechart chart = new Statechart("c9", 10, false);
    ConcurrentState n1 = new ConcurrentState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    HierarchicalState h1 = new HierarchicalState("p-r1", n1, new TestAction("p-r1", "A"), null, new TestAction("p-r1", "D"));
    HierarchicalState h2 = new HierarchicalState("p-r2", n1, new TestAction("p-r2", "A"), null, new TestAction("p-r2", "D"));
    HierarchicalState h3 = new HierarchicalState("p-r3", n1, new TestAction("p-r3", "A"), null, new TestAction("p-r3", "D"));
    State s1 = new State("a", h1, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", h2, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State s3 = new State("c", h2, new TestAction("c", "A"), null, new TestAction("c", "D"));
    State s4 = new State("d", h3, new TestAction("d", "A"), null, new TestAction("d", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    State p3 = new PseudoState("start p-r1", h1, PseudoState.pseudostate_start);
    State p4 = new PseudoState("start p-r2", h2, PseudoState.pseudostate_start);
    PseudoState p5 = new PseudoState("join", chart, PseudoState.pseudostate_join);
    State p6 = new PseudoState("start p-r3", h3, PseudoState.pseudostate_start);
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    p3.setEntryAction(new TestAction("start p-r1", "A"));
    p3.setExitAction(new TestAction("start p-r1", "D"));
    p4.setEntryAction(new TestAction("start p-r2", "A"));
    p4.setExitAction(new TestAction("start p-r2", "D"));
    p5.setEntryAction(new TestAction("join", "A"));
    p5.setExitAction(new TestAction("join", "D"));
    p6.setEntryAction(new TestAction("start p-r3", "A"));
    p6.setExitAction(new TestAction("start p-r3", "D"));
    new Transition(p1, n1);
    new Transition(p3, s1);
    new Transition(p4, s2);
    new Transition(s2, s3, new TimeoutEvent(2000));
    new Transition(s1, p5);
    new Transition(s3, p5);
    new Transition(p5, p2);
    new Transition(p6, s4);
    return chart;
  }

  static Statechart c10() throws StatechartException
  {
    Statechart chart = new Statechart("c10", 10, false);
    ConcurrentState n1 = new ConcurrentState("p", chart, new TestAction("p", "A"), null, new TestAction("p", "D"));
    HierarchicalState h1 = new HierarchicalState("p-r1", n1, new TestAction("p-r1", "A"), null, new TestAction("p-r1", "D"));
    HierarchicalState h2 = new HierarchicalState("p-r2", n1, new TestAction("p-r2", "A"), null, new TestAction("p-r2", "D"));
    State s1 = new State("a", h2, new TestAction("a", "A"), null, new TestAction("a", "D"));
    State s2 = new State("b", h2, new TestAction("b", "A"), null, new TestAction("b", "D"));
    State s3 = new State("c", chart, new TestAction("c", "A"), null, new TestAction("c", "D"));
    State p1 = new PseudoState("start", chart, PseudoState.pseudostate_start);
    State p2 = new FinalState("end", chart);
    State p3 = new PseudoState("start p-r1", h1, PseudoState.pseudostate_start);
    State p4 = new FinalState("end p-r1", h1);
    State p5 = new PseudoState("start p-r2", h2, PseudoState.pseudostate_start);
    State history = new PseudoState("history", h2, PseudoState.pseudostate_history);
    history.setExitAction(new TestAction("history", "E"));
    p1.setEntryAction(new TestAction("start", "A"));
    p1.setExitAction(new TestAction("start", "D"));
    p2.setEntryAction(new TestAction("end", "A"));
    p2.setExitAction(new TestAction("end", "D"));
    p3.setEntryAction(new TestAction("start p-r1", "A"));
    p3.setExitAction(new TestAction("start p-r1", "D"));
    p4.setEntryAction(new TestAction("end p-r1", "A"));
    p4.setExitAction(new TestAction("end p-r1", "D"));
    p5.setEntryAction(new TestAction("start p-r2", "A"));
    p5.setExitAction(new TestAction("start p-r2", "D"));
    new Transition(p1, s3);
    new Transition(s3, n1, new TestEvent(1));
    new Transition(s3, p2, new TestEvent(2));
    new Transition(p3, p4);
    new Transition(p5, history);
    new Transition(history, s1);
    new Transition(s1, s2, new TestEvent(1));
    new Transition(s1, s3, new TestEvent(2));
    new Transition(s2, s1, new TestEvent(1));
    new Transition(s2, s3, new TestEvent(2));
    return chart;
  }
}
