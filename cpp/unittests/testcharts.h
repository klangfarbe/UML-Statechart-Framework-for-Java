/*
 * UML statechart framework (http://uml-statecharts->sourceforge->net)
 *
 * Copyright (C) 2006-2007 Christian Mocek (klangfarbe@users->sourceforge->net)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2->1 of the License, or (at your option) any later version->
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE->  See the GNU
 * Lesser General Public License for more details->
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc->, 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */
#ifndef TESTCHARTS_H
#define TESTCHARTS_H

#include "../src/statechart/statechart.h"

#include "testaction.h"
#include "testevent.h"
#include "testmetadata.h"
#include "testguard.h"

using namespace statechart;

/**
 * Testcharts (see master thesis for details)
 */
class Testcharts {
public:
  static Statechart* t1() {
    Statechart* chart = new Statechart();
    State* s1 = new State(chart, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(chart, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    new Transition(p1, s1);
    new Transition(s1, s2);
    new Transition(s2, p2);
    return chart;
  }

  static Statechart* t2() {
    Statechart* chart = new Statechart();
    State* s1 = new State(chart, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    new Transition(p1, s1);
    new Transition(s1, s1, new TestEvent(1));
    new Transition(s1, p2, new TestEvent(2));
    return chart;
  }

  static Statechart* t3() {
    Statechart* chart = new Statechart();
    State* s1 = new State(chart, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(chart, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    new Transition(p1, s1);
    new Transition(s1, s2, new TestEvent(1));
    new Transition(s1, p2, new TimeoutEvent(5000));
    new Transition(s2, p2);
    return chart;
  }

  static Statechart* t4() {
    Statechart* chart = new Statechart();
    State* s1 = new State(chart, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(chart, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* s3 = new State(chart, new TestAction("c", "A"), NULL, new TestAction("c", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);
    PseudoState* j1 = new PseudoState(chart, pseudostate_junction);
    PseudoState* j2 = new PseudoState(chart, pseudostate_junction);
    PseudoState* j3 = new PseudoState(chart, pseudostate_junction);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    j1->setEntryAction(new TestAction("j1", "A"));
    j1->setExitAction(new TestAction("j1", "D"));
    j2->setEntryAction(new TestAction("j2", "A"));
    j2->setExitAction(new TestAction("j2", "D"));
    j3->setEntryAction(new TestAction("j3", "A"));
    j3->setExitAction(new TestAction("j3", "D"));
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

  static Statechart* h1() {
    Statechart* chart = new Statechart();
    HierarchicalState* h1 = new HierarchicalState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"));   
    State* s1 = new State(h1, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(h1, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);
    State* p3 = new PseudoState(h1, pseudostate_start);
    State* p4 = new FinalState(h1);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    p3->setEntryAction(new TestAction("start p", "A"));
    p3->setExitAction(new TestAction("start p", "D"));
    p4->setEntryAction(new TestAction("end p", "A"));
    p4->setExitAction(new TestAction("end p", "D"));
    new Transition(p1, h1);
    new Transition(h1, p2);
    new Transition(p3, s1);
    new Transition(s1, s2);
    new Transition(s2, p4);
    return chart;
  }

  static Statechart* h2() {
    Statechart* chart = new Statechart();
    HierarchicalState* h1 = new HierarchicalState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"));   
    State* s1 = new State(h1, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(h1, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);
    State* p3 = new PseudoState(h1, pseudostate_start);
    State* p4 = new FinalState(h1);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    p3->setEntryAction(new TestAction("start p", "A"));
    p3->setExitAction(new TestAction("start p", "D"));
    p4->setEntryAction(new TestAction("end p", "A"));
    p4->setExitAction(new TestAction("end p", "D"));
    new Transition(p1, h1);
    new Transition(h1, p2);
    new Transition(h1, p2, new TestEvent(1));
    new Transition(p3, s1);
    new Transition(s1, s2, new TestEvent(1));
    new Transition(s2, p4);
    return chart;
  }

  static Statechart* h3() {
    Statechart* chart = new Statechart();
    HierarchicalState* h1 = new HierarchicalState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"));   
    State* s1 = new State(h1, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(h1, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);
    State* p3 = new PseudoState(h1, pseudostate_start);
    State* p4 = new FinalState(h1);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    p3->setEntryAction(new TestAction("start p", "A"));
    p3->setExitAction(new TestAction("start p", "D"));
    p4->setEntryAction(new TestAction("end p", "A"));
    p4->setExitAction(new TestAction("end p", "D"));
    new Transition(p1, h1);
    new Transition(h1, p2);
    new Transition(h1, p2, new TestEvent(2));
    new Transition(p3, s1);
    new Transition(s1, s2, new TestEvent(1));
    new Transition(s2, p4);
    return chart;
  }

  static Statechart* h4() {
    Statechart* chart = new Statechart();
    HierarchicalState* h1 = new HierarchicalState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"));   
    State* s1 = new State(h1, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(h1, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);
    State* p3 = new PseudoState(h1, pseudostate_start);
    State* p4 = new FinalState(h1);
    State* hi = new PseudoState(h1, pseudostate_history);
    hi->setExitAction(new TestAction("history p", "U"));
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    p3->setEntryAction(new TestAction("start p", "A"));
    p3->setExitAction(new TestAction("start p", "D"));
    p4->setEntryAction(new TestAction("end p", "A"));
    p4->setExitAction(new TestAction("end p", "D"));
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

  static Statechart* h5() {
    Statechart* chart = new Statechart();
    HierarchicalState* h1 = new HierarchicalState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"));   
    HierarchicalState* h2 = new HierarchicalState(h1, new TestAction("q", "A"), NULL, new TestAction("q", "D"));
    State* s1 = new State(h1, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(h2, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* s3 = new State(h2, new TestAction("c", "A"), NULL, new TestAction("c", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);
    State* p3 = new PseudoState(h1, pseudostate_start);
    State* p4 = new FinalState(h1);
    State* hi = new PseudoState(h1, pseudostate_deep_history);
    State* p5 = new PseudoState(h2, pseudostate_start);
    hi->setExitAction(new TestAction("history p", "U"));
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    p3->setEntryAction(new TestAction("start p", "A"));
    p3->setExitAction(new TestAction("start p", "D"));
    p4->setEntryAction(new TestAction("end p", "A"));
    p4->setExitAction(new TestAction("end p", "D"));
    p5->setEntryAction(new TestAction("start q", "A"));
    p5->setExitAction(new TestAction("start q", "D"));
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
    return chart;
  }

  static Statechart* h6() {
    Statechart* chart = new Statechart();
    HierarchicalState* h1 = new HierarchicalState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"));
    HierarchicalState* h2 = new HierarchicalState(h1, new TestAction("q", "A"), NULL, new TestAction("q", "D"));
    HierarchicalState* h3 = new HierarchicalState(chart, new TestAction("x", "A"), NULL, new TestAction("x", "D"));
    State* s1 = new State(h2, new TestAction("r", "A"), NULL, new TestAction("r", "D"));
    State* s2 = new State(h3, new TestAction("y", "A"), NULL, new TestAction("y", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    new Transition(p1, s1);
    new Transition(s1, s2);
    new Transition(s2, p2);
    return chart;
  }

  static Statechart* c1() {
    Statechart* chart = new Statechart();
    ConcurrentState* n1 = new ConcurrentState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"));    
    HierarchicalState* h1 = new HierarchicalState(n1, new TestAction("p-r1", "A"), NULL, new TestAction("p-r1", "D"));
    HierarchicalState* h2 = new HierarchicalState(n1, new TestAction("p-r2", "A"), NULL, new TestAction("p-r2", "D"));
    State* s1 = new State(h1, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(h2, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);    
    State* p3 = new PseudoState(h1, pseudostate_start);
    State* p4 = new FinalState(h1);
    State* p5 = new PseudoState(h2, pseudostate_start);
    State* p6 = new FinalState(h2);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    p3->setEntryAction(new TestAction("start p-r1", "A"));
    p3->setExitAction(new TestAction("start p-r1", "D"));
    p4->setEntryAction(new TestAction("end p-r1", "A"));
    p4->setExitAction(new TestAction("end p-r1", "D"));
    p5->setEntryAction(new TestAction("start p-r2", "A"));
    p5->setExitAction(new TestAction("start p-r2", "D")); 
    p6->setEntryAction(new TestAction("end p-r2", "A"));
    p6->setExitAction(new TestAction("end p-r2", "D")); 
    new Transition(p1, n1);
    new Transition(n1, p2);
    new Transition(p3, s1);   
    new Transition(s1, p4);
    new Transition(p5, s2);   
    new Transition(s2, p6); 
    return chart;
  }

  static Statechart* c2() {
    Statechart* chart = new Statechart();
    ConcurrentState* n1 = new ConcurrentState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"));    
    HierarchicalState* h1 = new HierarchicalState(n1, new TestAction("p-r1", "A"), NULL, new TestAction("p-r1", "D"));
    HierarchicalState* h2 = new HierarchicalState(n1, new TestAction("p-r2", "A"), NULL, new TestAction("p-r2", "D"));
    State* s1 = new State(h1, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(h1, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* s3 = new State(h2, new TestAction("c", "A"), NULL, new TestAction("c", "D"));
    State* s4 = new State(h2, new TestAction("d", "A"), NULL, new TestAction("d", "D"));
    State* s5 = new State(h2, new TestAction("e", "A"), NULL, new TestAction("e", "D"));    
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);    
    State* p3 = new PseudoState(h1, pseudostate_start);
    State* p4 = new FinalState(h1);
    State* p5 = new PseudoState(h2, pseudostate_start);
    State* p6 = new FinalState(h2);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    p3->setEntryAction(new TestAction("start p-r1", "A"));
    p3->setExitAction(new TestAction("start p-r1", "D"));
    p4->setEntryAction(new TestAction("end p-r1", "A"));
    p4->setExitAction(new TestAction("end p-r1", "D"));
    p5->setEntryAction(new TestAction("start p-r2", "A"));
    p5->setExitAction(new TestAction("start p-r2", "D")); 
    p6->setEntryAction(new TestAction("end p-r2", "A"));
    p6->setExitAction(new TestAction("end p-r2", "D")); 
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

  static Statechart* c3() {
    Statechart* chart = new Statechart();
    ConcurrentState* n1 = new ConcurrentState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"));    
    HierarchicalState* h1 = new HierarchicalState(n1, new TestAction("p-r1", "A"), NULL, new TestAction("p-r1", "D"));
    HierarchicalState* h2 = new HierarchicalState(n1, new TestAction("p-r2", "A"), NULL, new TestAction("p-r2", "D"));
    State* s1 = new State(h1, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(h2, new TestAction("b", "A"), NULL, new TestAction("b", "D"));    
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);    
    State* p3 = new FinalState(h1);
    State* p4 = new PseudoState(h2, pseudostate_start);
    State* p5 = new FinalState(h2);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    p3->setEntryAction(new TestAction("end p-r1", "A"));
    p3->setExitAction(new TestAction("end p-r1", "D"));
    p4->setEntryAction(new TestAction("start p-r2", "A"));
    p4->setExitAction(new TestAction("start p-r2", "D")); 
    p5->setEntryAction(new TestAction("end p-r2", "A"));
    p5->setExitAction(new TestAction("end p-r2", "D"));
    new Transition(p1, s1);
    new Transition(n1, p2);
    new Transition(s1, p3);   
    new Transition(p4, s2);
    new Transition(s2, p5); 
    return chart;
  }

  static Statechart* c4() {
    Statechart* chart = new Statechart();
    ConcurrentState* n1 = new ConcurrentState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"), "n1");    
    HierarchicalState* h1 = new HierarchicalState(n1, new TestAction("p-r1", "A"), NULL, new TestAction("p-r1", "D"), "h1");
    HierarchicalState* h2 = new HierarchicalState(n1, new TestAction("p-r2", "A"), NULL, new TestAction("p-r2", "D"), "h2");
    State* s1 = new State(h1, new TestAction("a", "A"), NULL, new TestAction("a", "D"), "s1");
    State* s2 = new State(h2, new TestAction("b", "A"), NULL, new TestAction("b", "D"), "s2");
    State* s3 = new State(h2, new TestAction("c", "A"), NULL, new TestAction("c", "D"), "s3");
    State* p1 = new PseudoState(chart, pseudostate_start, "p1");
    State* p2 = new FinalState(chart);    
    State* p3 = new PseudoState(h1, pseudostate_start, "p3");
    State* p4 = new PseudoState(h2, pseudostate_start, "p4");
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    p3->setEntryAction(new TestAction("start p-r1", "A"));
    p3->setExitAction(new TestAction("start p-r1", "D"));
    p4->setEntryAction(new TestAction("start p-r2", "A"));
    p4->setExitAction(new TestAction("start p-r2", "D"));   
    new Transition(p1, n1);
    new Transition(p3, s1);   
    new Transition(s1, p2);
    new Transition(p4, s2);   
    new Transition(s2, s3); 
    return chart;
  }

  static Statechart* c5() {
    Statechart* chart = new Statechart();
    ConcurrentState* n1 = new ConcurrentState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"));    
    HierarchicalState* h1 = new HierarchicalState(n1, new TestAction("p-r1", "A"), NULL, new TestAction("p-r1", "D"));
    HierarchicalState* h2 = new HierarchicalState(n1, new TestAction("p-r2", "A"), NULL, new TestAction("p-r2", "D"));
    State* s1 = new State(h1, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(h2, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* s3 = new State(h2, new TestAction("c", "A"), NULL, new TestAction("c", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);    
    State* p3 = new PseudoState(h1, pseudostate_start);
    State* p4 = new PseudoState(h2, pseudostate_start);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    p3->setEntryAction(new TestAction("start p-r1", "A"));
    p3->setExitAction(new TestAction("start p-r1", "D"));
    p4->setEntryAction(new TestAction("start p-r2", "A"));
    p4->setExitAction(new TestAction("start p-r2", "D"));
    new Transition(p1, n1);
    new Transition(p3, s1);   
    new Transition(s1, p2, new TestEvent(1));
    new Transition(p4, s2);   
    new Transition(s2, s3, new TestEvent(2)); 
    return chart;
  }

  static Statechart* c6() {
    Statechart* chart = new Statechart();
    ConcurrentState* n1 = new ConcurrentState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"));    
    HierarchicalState* h1 = new HierarchicalState(n1, new TestAction("p-r1", "A"), NULL, new TestAction("p-r1", "D"));
    HierarchicalState* h2 = new HierarchicalState(n1, new TestAction("p-r2", "A"), NULL, new TestAction("p-r2", "D"));
    State* s1 = new State(h1, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(h2, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);  
    State* p3 = new FinalState(h1);
    State* p4 = new FinalState(h2);   
    State* p5 = new PseudoState(chart, pseudostate_fork);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    p3->setEntryAction(new TestAction("end p-r1", "A"));
    p3->setExitAction(new TestAction("end p-r1", "D"));   
    p4->setEntryAction(new TestAction("end p-r2", "A"));
    p4->setExitAction(new TestAction("end p-r2", "D"));
    p5->setEntryAction(new TestAction("fork", "A"));
    p5->setExitAction(new TestAction("fork", "D"));
    new Transition(p1, p5);
    new Transition(p5, s1);
    new Transition(p5, s2);
    new Transition(s1, p3);
    new Transition(s2, p4);
    new Transition(n1, p2);
    return chart;
  }

  static Statechart* c7() {
    Statechart* chart = new Statechart();
    ConcurrentState* n1 = new ConcurrentState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"));    
    HierarchicalState* h1 = new HierarchicalState(n1, new TestAction("p-r1", "A"), NULL, new TestAction("p-r1", "D"));
    HierarchicalState* h2 = new HierarchicalState(n1, new TestAction("p-r2", "A"), NULL, new TestAction("p-r2", "D"));
    HierarchicalState* h3 = new HierarchicalState(n1, new TestAction("p-r3", "A"), NULL, new TestAction("p-r3", "D"));    
    State* s1 = new State(h1, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(h2, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* s3 = new State(h3, new TestAction("c", "A"), NULL, new TestAction("c", "D"));
    State* s4 = new State(h3, new TestAction("d", "A"), NULL, new TestAction("d", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);  
    State* p3 = new FinalState(h1);
    State* p4 = new FinalState(h2);   
    State* p5 = new PseudoState(chart, pseudostate_fork);
    State* p6 = new PseudoState(h3, pseudostate_start);
    State* p7 = new FinalState(h3);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    p3->setEntryAction(new TestAction("end p-r1", "A"));
    p3->setExitAction(new TestAction("end p-r1", "D"));   
    p4->setEntryAction(new TestAction("end p-r2", "A"));
    p4->setExitAction(new TestAction("end p-r2", "D"));
    p5->setEntryAction(new TestAction("fork", "A"));
    p5->setExitAction(new TestAction("fork", "D"));   
    p6->setEntryAction(new TestAction("start p-r3", "A"));
    p6->setExitAction(new TestAction("start p-r3", "D"));
    p7->setEntryAction(new TestAction("end p-r3", "A"));
    p7->setExitAction(new TestAction("end p-r3", "D"));
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

  static Statechart* c8() {
    Statechart* chart = new Statechart();
    ConcurrentState* n1 = new ConcurrentState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"));    
    HierarchicalState* h1 = new HierarchicalState(n1, new TestAction("p-r1", "A"), NULL, new TestAction("p-r1", "D"));
    HierarchicalState* h2 = new HierarchicalState(n1, new TestAction("p-r2", "A"), NULL, new TestAction("p-r2", "D"));
    State* s1 = new State(h1, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(h2, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* s3 = new State(h2, new TestAction("c", "A"), NULL, new TestAction("c", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);  
    State* p3 = new PseudoState(h1, pseudostate_start);
    State* p4 = new PseudoState(h2, pseudostate_start);   
    PseudoState* p5 = new PseudoState(chart, pseudostate_join);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    p3->setEntryAction(new TestAction("start p-r1", "A"));
    p3->setExitAction(new TestAction("start p-r1", "D"));   
    p4->setEntryAction(new TestAction("start p-r2", "A"));
    p4->setExitAction(new TestAction("start p-r2", "D"));
    p5->setEntryAction(new TestAction("join", "A"));
    p5->setExitAction(new TestAction("join", "D"));   
    new Transition(p1, n1);
    new Transition(p3, s1);
    new Transition(p4, s2);
    new Transition(s2, s3);   
    new Transition(s1, p5);
    new Transition(s3, p5);   
    new Transition(p5, p2);
    return chart;
  }

  static Statechart* c9() {
    Statechart* chart = new Statechart();
    ConcurrentState* n1 = new ConcurrentState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"));    
    HierarchicalState* h1 = new HierarchicalState(n1, new TestAction("p-r1", "A"), NULL, new TestAction("p-r1", "D"));
    HierarchicalState* h2 = new HierarchicalState(n1, new TestAction("p-r2", "A"), NULL, new TestAction("p-r2", "D"));
    HierarchicalState* h3 = new HierarchicalState(n1, new TestAction("p-r3", "A"), NULL, new TestAction("p-r3", "D"));
    State* s1 = new State(h1, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(h2, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* s3 = new State(h2, new TestAction("c", "A"), NULL, new TestAction("c", "D"));
    State* s4 = new State(h3, new TestAction("d", "A"), NULL, new TestAction("d", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);  
    State* p3 = new PseudoState(h1, pseudostate_start);
    State* p4 = new PseudoState(h2, pseudostate_start);   
    PseudoState* p5 = new PseudoState(chart, pseudostate_join);
    State* p6 = new PseudoState(h3, pseudostate_start);
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    p3->setEntryAction(new TestAction("start p-r1", "A"));
    p3->setExitAction(new TestAction("start p-r1", "D"));   
    p4->setEntryAction(new TestAction("start p-r2", "A"));
    p4->setExitAction(new TestAction("start p-r2", "D"));
    p5->setEntryAction(new TestAction("join", "A"));
    p5->setExitAction(new TestAction("join", "D"));   
    p6->setEntryAction(new TestAction("start p-r3", "A"));
    p6->setExitAction(new TestAction("start p-r3", "D"));
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

  static Statechart* c10() {
    Statechart* chart = new Statechart();
    ConcurrentState* n1 = new ConcurrentState(chart, new TestAction("p", "A"), NULL, new TestAction("p", "D"));    
    HierarchicalState* h1 = new HierarchicalState(n1, new TestAction("p-r1", "A"), NULL, new TestAction("p-r1", "D"));
    HierarchicalState* h2 = new HierarchicalState(n1, new TestAction("p-r2", "A"), NULL, new TestAction("p-r2", "D"));
    State* s1 = new State(h2, new TestAction("a", "A"), NULL, new TestAction("a", "D"));
    State* s2 = new State(h2, new TestAction("b", "A"), NULL, new TestAction("b", "D"));
    State* s3 = new State(chart, new TestAction("c", "A"), NULL, new TestAction("c", "D"));
    State* p1 = new PseudoState(chart, pseudostate_start);
    State* p2 = new FinalState(chart);  
    State* p3 = new PseudoState(h1, pseudostate_start);
    State* p4 = new FinalState(h1);
    State* p5 = new PseudoState(h2, pseudostate_start);
    State* history = new PseudoState(h2, pseudostate_history);
    history->setExitAction(new TestAction("history", "E"));
    p1->setEntryAction(new TestAction("start", "A"));
    p1->setExitAction(new TestAction("start", "D"));
    p2->setEntryAction(new TestAction("end", "A"));
    p2->setExitAction(new TestAction("end", "D"));
    p3->setEntryAction(new TestAction("start p-r1", "A"));
    p3->setExitAction(new TestAction("start p-r1", "D"));
    p4->setEntryAction(new TestAction("end p-r1", "A"));
    p4->setExitAction(new TestAction("end p-r1", "D"));
    p5->setEntryAction(new TestAction("start p-r2", "A"));
    p5->setExitAction(new TestAction("start p-r2", "D"));
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
};

#endif
