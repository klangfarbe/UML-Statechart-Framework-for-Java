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
#ifndef METADATATEST_H_INCLUDED
#define METADATATEST_H_INCLUDED

#include <cxxtest/TestSuite.h>

#include "../src/statechart/statechart.h"
#include "../src/util/thread.h"
#include "../src/util/smartpointer.h"
#include "testcharts.h"

using namespace statechart;
using namespace statechart::util;

class Unittests : public CxxTest::TestSuite {

public:
  void smart1(SmartPointer<Metadata> data) {
    TS_ASSERT_EQUALS(data->getReferenceCount(), 2);
  }

  void smart2(SmartPointer<Metadata>& data) {
    TS_ASSERT_EQUALS(data->getReferenceCount(), 1);
  }

  void testSmartPointer() {
    SmartPointer<Metadata> p(new Metadata());

    TS_ASSERT_EQUALS(p->getReferenceCount(), 1);
    smart1(p);
    smart2(p);

    TS_ASSERT_EQUALS(p->getReferenceCount(), 1);
    SmartPointer<Metadata> q = p;
    TS_ASSERT_EQUALS(p->getReferenceCount(), 2);
    TS_ASSERT_EQUALS(q->getReferenceCount(), 2);


    SmartPointer<TestMetadata> p2(new TestMetadata());
    TS_ASSERT_EQUALS(p2->getReferenceCount(), 1);
    smart1(p2);
    TS_ASSERT_EQUALS(p2->getReferenceCount(), 1);
    SmartPointer<TestMetadata> p3(p2);
    TS_ASSERT_EQUALS(p2->getReferenceCount(), 2);
    SmartPointer<Metadata> p4(p2);
    TS_ASSERT_EQUALS(p2->getReferenceCount(), 3);
  };

  void testMetadata() {
    /*
    Statechart* s = new Statechart();
    State* a = new State(s);
    State* b = new State(s);
    PseudoState* start = new PseudoState(s, pseudostate_start);

    TS_ASSERT(s);
    TS_ASSERT(a);
    TS_ASSERT(b);
    TS_ASSERT(start);

    TS_ASSERT_EQUALS(a->getContext(), s);
    TS_ASSERT_EQUALS(b->getContext(), s);

    TestMetadata* data = new TestMetadata();
    s->start(data);

    TS_ASSERT(data);
    TS_ASSERT_EQUALS(data->isActive(s), true);

    data->activate(a);

    TS_ASSERT_EQUALS(data->isActive(a), true);
    TS_ASSERT_EQUALS(data->isActive(b), false);

    data->deactivate(a);
    data->activate(b);

    TS_ASSERT_EQUALS(data->isActive(b), true);
    TS_ASSERT_EQUALS(data->isActive(a), false);

    delete data;
    delete s;
    */
  }

  void testEventQueue() {
    Statechart* chart = Testcharts::t2();
    SmartPointer<Metadata> data(new TestMetadata());
    SmartPointer<Event> event1(new TestEvent(1));
    SmartPointer<Event> event2(new TestEvent(2));

    chart->startAsynchron(data);
    chart->dispatchAsynchron(data, event1);
    chart->dispatchAsynchron(data, event2);

    // Wait until the statechart reached its final state
    State* current = 0; 
    while(!current || dynamic_cast<FinalState*>(current) == 0) {      
      data->lock();
      current = data->getData(chart)->currentState;
      data->unlock();
      Thread::sleep(100);
    }
   
    TS_ASSERT_EQUALS("D:start A:a D:a A:a D:a A:end", ((TestMetadata*)spGetImpl(data))->path);
    Thread::sleep(5000);
    delete chart;
  }

  void testSemantics1() {
    Statechart* chart = Testcharts::t1();

    SmartPointer<Metadata> data(new TestMetadata());

    TS_ASSERT(chart->start(data));
    TS_ASSERT_EQUALS("D:start A:a D:a A:b D:b A:end", ((TestMetadata*)spGetImpl(data))->path);

    delete chart;
  }

  void testSemantics2() {
    Statechart* chart = Testcharts::t2();
    SmartPointer<Metadata> data(new TestMetadata());
    SmartPointer<Event> event1(new TestEvent(1));
    SmartPointer<Event> event2(new TestEvent(2));
    SmartPointer<Event> event3(new TestEvent(3));
    SmartPointer<Event> event4(new TestEvent(4));

    TS_ASSERT(chart->start(data));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT(chart->dispatch(data, event2));
    TS_ASSERT_EQUALS("D:start A:a D:a A:a D:a A:end", ((TestMetadata*)spGetImpl(data))->path);

    // check if more than one signals create a longer way
    ((TestMetadata*)spGetImpl(data))->path.clear();

    TS_ASSERT(chart->start(data)) ;
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT(chart->dispatch(data, event2));
    TS_ASSERT_EQUALS("D:start A:a D:a A:a D:a A:a D:a A:end", ((TestMetadata*)spGetImpl(data))->path);

    delete chart;
  }

  void testSemantics3() {
    Statechart* chart = Testcharts::t3();
    SmartPointer<Metadata> data(new TestMetadata());
    SmartPointer<Event> event1(new TestEvent(1));

    TS_ASSERT(chart->start(data));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT_EQUALS("D:start A:a D:a A:b D:b A:end", ((TestMetadata*)spGetImpl(data))->path);

    delete chart;
  }

  void testSemantics4() {
    Statechart* chart = Testcharts::t3();

    SmartPointer<Metadata> data(new TestMetadata());

    TS_ASSERT(chart->start(data));

    // Wait until the statechart reached its final state
    State* current = 0; 
    while(!current || dynamic_cast<FinalState*>(current) == 0) {      
      data->lock();
      current = data->getData(chart)->currentState;
      data->unlock();
      Thread::sleep(100);
    }
    TS_ASSERT_EQUALS("D:start A:a D:a A:end", ((TestMetadata*)spGetImpl(data))->path);

    delete chart;
  }

  void testSemantics5() {
    Statechart* chart = Testcharts::t4();

    SmartPointer<Metadata> data(new TestMetadata());
    ((TestMetadata*)spGetImpl(data))->guardvalue = 0;

    TS_ASSERT(chart->start(data));
    TS_ASSERT_EQUALS("D:start A:a D:a A:end", ((TestMetadata*)spGetImpl(data))->path);

    delete chart;
  }

  void testSemantics6() {
    Statechart* chart = Testcharts::t4();
    SmartPointer<Metadata> data(new TestMetadata());
    ((TestMetadata*)spGetImpl(data))->guardvalue = 1;

    TS_ASSERT(chart->start(data));
    TS_ASSERT_EQUALS("D:start A:a D:a A:j1 D:j1 E:a1 A:b D:b A:end", ((TestMetadata*)spGetImpl(data))->path);

    delete chart;
  }

  void testSemantics7() {
    Statechart* chart = Testcharts::t4();

    SmartPointer<Metadata> data(new TestMetadata());
    ((TestMetadata*)spGetImpl(data))->guardvalue = 2;

    TS_ASSERT(chart->start(data));
    TS_ASSERT_EQUALS("D:start A:a D:a A:j1 D:j1 E:a2 A:c D:c A:j2 D:j2 E:a3 A:j3 D:j3 E:a4 A:end", ((TestMetadata*)spGetImpl(data))->path);

    delete chart;
  }

  void testSemantics8() {
    Statechart* chart = Testcharts::h1();

    SmartPointer<Metadata> data(new TestMetadata());

    TS_ASSERT(chart->start(data));
    TS_ASSERT_EQUALS("D:start A:p A:start p D:start p A:a D:a A:b D:b A:end p D:end p D:p A:end", ((TestMetadata*)spGetImpl(data))->path);

    delete chart;
  }

  void testSemantics9() {
    Statechart* chart = Testcharts::h2();
    SmartPointer<Metadata> data(new TestMetadata());
    SmartPointer<Event> event1(new TestEvent(1));

    TS_ASSERT(chart->start(data));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT_EQUALS("D:start A:p A:start p D:start p A:a D:a A:b D:b A:end p D:end p D:p A:end", ((TestMetadata*)spGetImpl(data))->path);

    delete chart;
  }

  void testSemantics10() {
    Statechart* chart = Testcharts::h3();
    SmartPointer<Metadata> data(new TestMetadata());
    SmartPointer<Event> event1(new TestEvent(1));
    
    TS_ASSERT(chart->start(data));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT_EQUALS("D:start A:p A:start p D:start p A:a D:a A:b D:b A:end p D:end p D:p A:end", ((TestMetadata*)spGetImpl(data))->path);

    delete chart;
  }

  void testSemantics11() {
    Statechart* chart = Testcharts::h3();
    SmartPointer<Metadata> data(new TestMetadata());
    SmartPointer<Event> event2(new TestEvent(2));

    TS_ASSERT(chart->start(data));
    TS_ASSERT(chart->dispatch(data, event2));
    TS_ASSERT_EQUALS("D:start A:p A:start p D:start p A:a D:a D:p A:end", ((TestMetadata*)spGetImpl(data))->path);

    delete chart;
  }

  void testSemantics12() {
    Statechart* chart = Testcharts::h4();
    SmartPointer<Metadata> data(new TestMetadata());
    SmartPointer<Event> event1(new TestEvent(1));
    SmartPointer<Event> event2(new TestEvent(2));
    SmartPointer<Event> event3(new TestEvent(3));

    TS_ASSERT(chart->start(data));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT(chart->dispatch(data, event2));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT(chart->dispatch(data, event2));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT(chart->dispatch(data, event2));
    TS_ASSERT(chart->dispatch(data, event3));

    string result = "D:start A:p A:start p D:start p U:history p A:a D:a ";
    result.append("D:p A:p A:start p D:start p A:a D:a A:b D:b D:p A:p ");
    result.append("A:start p D:start p A:b D:b A:a D:a D:p A:p A:start p ");
    result.append("D:start p A:a D:a A:b D:b A:end p D:end p D:p A:end");

    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);

    delete chart;
  }

  void testSemantics13() {
    Statechart* chart = Testcharts::h5();
    SmartPointer<Metadata> data(new TestMetadata());
    SmartPointer<Event> event1(new TestEvent(1));
    SmartPointer<Event> event2(new TestEvent(2));
    SmartPointer<Event> event3(new TestEvent(3));
    SmartPointer<Event> event4(new TestEvent(4));

    TS_ASSERT(chart->start(data));
    TS_ASSERT(chart->dispatch(data, event2));
    TS_ASSERT(chart->dispatch(data, event4));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT(chart->dispatch(data, event2));
    TS_ASSERT(chart->dispatch(data, event2));
    TS_ASSERT(chart->dispatch(data, event3));

    string result = "D:start A:p A:start p D:start p U:history p A:a D:a ";
    result.append("A:q A:start q D:start q A:b D:b A:c D:c D:q D:p A:p ");
    result.append("A:start p D:start p A:q A:c D:c D:q A:a D:a A:q ");
    result.append("A:start q D:start q A:b D:b D:q A:end p D:end p D:p A:end");

    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);

    delete chart;
  }

  void testSemantics14() {
    Statechart* chart = Testcharts::h6();

    SmartPointer<Metadata> data(new TestMetadata());

    TS_ASSERT(chart->start(data));
    TS_ASSERT_EQUALS("D:start A:p A:q A:r D:r D:q D:p A:x A:y D:y D:x A:end", ((TestMetadata*)spGetImpl(data))->path);

    delete chart;
  }

  void testSemantics15() {
    Statechart* chart = Testcharts::c1();
    SmartPointer<Metadata> data(new TestMetadata());

    TS_ASSERT(chart->start(data));

    string result = "D:start ";
    result.append("A:p ");
    result.append("A:p-r1 ");
    result.append("A:start p-r1 ");
    result.append("D:start p-r1 ");
    result.append("A:a ");
    result.append("A:p-r2 ");
    result.append("A:start p-r2 ");
    result.append("D:start p-r2 ");
    result.append("A:b ");
    result.append("D:a ");
    result.append("A:end p-r1 ");
    result.append("D:b ");
    result.append("A:end p-r2 ");
    result.append("D:end p-r1 ");
    result.append("D:p-r1 ");
    result.append("D:end p-r2 ");
    result.append("D:p-r2 ");
    result.append("D:p ");
    result.append("A:end");

    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);
    delete chart;
  }

  void testSemantics16() {
    Statechart* chart = Testcharts::c2();
    SmartPointer<Metadata> data(new TestMetadata());
    SmartPointer<Event> event1(new TestEvent(1));
    SmartPointer<Event> event2(new TestEvent(2));

    TS_ASSERT(chart->start(data));
    TS_ASSERT(chart->dispatch(data, event2));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT(chart->dispatch(data, event2));

    string result = "D:start ";
    result.append("A:p ");
    result.append("A:p-r1 ");
    result.append("A:start p-r1 ");
    result.append("D:start p-r1 ");
    result.append("A:a ");
    result.append("A:p-r2 ");
    result.append("A:start p-r2 ");
    result.append("D:start p-r2 ");
    result.append("A:c ");
    result.append("D:a ");
    result.append("A:b ");
    result.append("D:b ");
    result.append("A:end p-r1 ");
    result.append("D:c ");
    result.append("A:d ");
    result.append("D:d ");
    result.append("A:e ");
    result.append("D:e ");
    result.append("A:end p-r2 ");
    result.append("D:end p-r1 ");
    result.append("D:p-r1 ");
    result.append("D:end p-r2 ");
    result.append("D:p-r2 ");
    result.append("D:p ");
    result.append("A:end");

    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);
    delete chart;
  }

  void testSemantics17() {
    Statechart* chart = Testcharts::c2();
    SmartPointer<Metadata> data(new TestMetadata());
    SmartPointer<Event> event1(new TestEvent(1));
    SmartPointer<Event> event2(new TestEvent(2));
    TS_ASSERT(chart->start(data));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT(chart->dispatch(data, event2));

    string result = "D:start ";

    result.append("A:p ");
    // aktivieren der Regionen
    result.append("A:p-r1 ");
    result.append("A:start p-r1 ");
    result.append("D:start p-r1 ");
    result.append("A:a ");
    result.append("A:p-r2 ");
    result.append("A:start p-r2 ");
    result.append("D:start p-r2 ");
    result.append("A:c ");
    // c -> d
    result.append("D:c ");
    result.append("A:d ");
    // a -> b
    result.append("D:a ");
    result.append("A:b ");
    // d -> e
    result.append("D:d ");
    result.append("A:e ");
    // b -> end p-r2
    result.append("D:b ");
    result.append("A:end p-r1 ");
    // e -> end p-r2
    result.append("D:e ");
    result.append("A:end p-r2 ");
    // Endtransitionen
    result.append("D:end p-r1 ");
    result.append("D:p-r1 ");
    result.append("D:end p-r2 ");
    result.append("D:p-r2 ");
    result.append("D:p ");
    result.append("A:end");

    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);
    delete chart;
  }

  void testSemantics18() {
    Statechart* chart = Testcharts::c2();
    SmartPointer<Metadata> data(new TestMetadata());
    SmartPointer<Event> event1(new TestEvent(1));
    SmartPointer<Event> event3(new TestEvent(3));
    TS_ASSERT(chart->start(data));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT(chart->dispatch(data, event3));

    string result = "D:start ";
    result.append("A:p ");
    // aktivieren der Regionen
    result.append("A:p-r1 ");
    result.append("A:start p-r1 ");
    result.append("D:start p-r1 ");
    result.append("A:a ");
    result.append("A:p-r2 ");
    result.append("A:start p-r2 ");
    result.append("D:start p-r2 ");
    result.append("A:c ");
    // c -> d
    result.append("D:c ");
    result.append("A:d ");
    // S3 Transition
    result.append("D:a ");
    result.append("D:p-r1 ");
    result.append("D:d ");
    result.append("D:p-r2 ");
    result.append("D:p ");
    result.append("A:end");

    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);
    delete chart;
  }

  void testSemantics19() {
    Statechart* chart = Testcharts::c3();
    SmartPointer<Metadata> data(new TestMetadata());

    TS_ASSERT(chart->start(data));

    string result = "D:start ";
    result.append("A:p ");
    // aktivieren der Regionen
    result.append("A:p-r2 ");
    result.append("A:start p-r2 ");
    result.append("D:start p-r2 ");
    result.append("A:b ");
    result.append("A:p-r1 ");
    result.append("A:a ");
    // a -> end p-r1
    result.append("D:a ");
    result.append("A:end p-r1 ");
    // b -> end p-r2
    result.append("D:b ");
    result.append("A:end p-r2 ");
    // Endtransitionen
    result.append("D:end p-r1 ");
    result.append("D:p-r1 ");
    result.append("D:end p-r2 ");
    result.append("D:p-r2 ");
    result.append("D:p ");
    result.append("A:end");

    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);
    delete chart;
  }

  void testSemantics20() {
    Statechart* chart = Testcharts::c4();
    SmartPointer<Metadata> data(new TestMetadata());

    TS_ASSERT(chart->start(data));

    string result = "D:start ";
    result.append("A:p ");
    // aktivieren der Regionen
    result.append("A:p-r1 ");
    result.append("A:start p-r1 ");
    result.append("D:start p-r1 ");
    result.append("A:a ");
    result.append("A:p-r2 ");
    result.append("A:start p-r2 ");
    result.append("D:start p-r2 ");
    result.append("A:b ");
    // a -> end
    result.append("D:a ");
    result.append("D:p-r1 ");
    result.append("D:b ");
    result.append("D:p-r2 ");
    result.append("D:p ");
    result.append("A:end");


    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);
    delete chart;
  }

  void testSemantics21() {
    Statechart* chart = Testcharts::c5();
    SmartPointer<Metadata> data(new TestMetadata());
    SmartPointer<Event> event1(new TestEvent(1));
    SmartPointer<Event> event2(new TestEvent(2));
    TS_ASSERT(chart->start(data));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT(!chart->dispatch(data, event2));

    string result = "D:start ";
    result.append("A:p ");
    // aktivieren der Regionen
    result.append("A:p-r1 ");
    result.append("A:start p-r1 ");
    result.append("D:start p-r1 ");
    result.append("A:a ");
    result.append("A:p-r2 ");
    result.append("A:start p-r2 ");
    result.append("D:start p-r2 ");
    result.append("A:b ");
    // a -> end
    result.append("D:a ");
    result.append("D:p-r1 ");
    result.append("D:b ");
    result.append("D:p-r2 ");
    result.append("D:p ");
    result.append("A:end");


    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);
    delete chart;
  }

  void testSemantics22() {
    Statechart* chart = Testcharts::c5();
    SmartPointer<Metadata> data(new TestMetadata());
    SmartPointer<Event> event1(new TestEvent(1));
    SmartPointer<Event> event2(new TestEvent(2));
    TS_ASSERT(chart->start(data));
    TS_ASSERT(chart->dispatch(data, event2));
    TS_ASSERT(chart->dispatch(data, event1));

    string result = "D:start ";
    result.append("A:p ");
    // aktivieren der Regionen
    result.append("A:p-r1 ");
    result.append("A:start p-r1 ");
    result.append("D:start p-r1 ");
    result.append("A:a ");
    result.append("A:p-r2 ");
    result.append("A:start p-r2 ");
    result.append("D:start p-r2 ");
    result.append("A:b ");
    // b -> c
    result.append("D:b ");
    result.append("A:c ");
    // a -> end
    result.append("D:a ");
    result.append("D:p-r1 ");
    result.append("D:c ");
    result.append("D:p-r2 ");
    result.append("D:p ");
    result.append("A:end");


    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);
    delete chart;
  }

  void testSemantics23() {
    Statechart* chart = Testcharts::c6();
    SmartPointer<Metadata> data(new TestMetadata());

    TS_ASSERT(chart->start(data));

    string result = "D:start ";
    result.append("A:fork ");
    result.append("D:fork ");
    // aktivieren der Regionen
    result.append("A:p ");
    result.append("A:p-r1 ");
    result.append("A:a ");
    result.append("A:p-r2 ");
    result.append("A:b ");
    result.append("D:a ");
    result.append("A:end p-r1 ");
    result.append("D:b ");
    result.append("A:end p-r2 ");
    result.append("D:end p-r1 ");
    result.append("D:p-r1 ");
    result.append("D:end p-r2 ");
    result.append("D:p-r2 ");
    result.append("D:p ");
    result.append("A:end");


    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);
    delete chart;
  }

  void testSemantics24() {
    Statechart* chart = Testcharts::c7();
    SmartPointer<Metadata> data(new TestMetadata());
    ((TestMetadata*)spGetImpl(data))->guardvalue = 1;

    TS_ASSERT(chart->start(data));

    string result = "D:start ";
    result.append("A:fork ");
    result.append("D:fork ");
    result.append("A:p ");
    // aktivieren der Regionen
    result.append("A:p-r3 ");
    result.append("A:c ");
    result.append("A:p-r1 ");
    result.append("A:a ");
    result.append("A:p-r2 ");
    result.append("A:b ");
    result.append("D:a ");
    result.append("A:end p-r1 ");
    result.append("D:b ");
    result.append("A:end p-r2 ");
    result.append("D:c ");
    result.append("A:end p-r3 ");
    result.append("D:end p-r1 ");
    result.append("D:p-r1 ");
    result.append("D:end p-r2 ");
    result.append("D:p-r2 ");
    result.append("D:end p-r3 ");
    result.append("D:p-r3 ");
    result.append("D:p ");
    result.append("A:end");

    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);
    delete chart;
  }

  void testSemantics25() {
    Statechart* chart = Testcharts::c7();
    SmartPointer<Metadata> data(new TestMetadata());
    ((TestMetadata*)spGetImpl(data))->guardvalue = 0;

    TS_ASSERT(chart->start(data));

    string result = "D:start ";
    result.append("A:fork ");
    result.append("D:fork ");
    result.append("A:p ");
    // aktivieren der Regionen
    result.append("A:p-r3 ");
    result.append("A:start p-r3 ");
    result.append("D:start p-r3 ");
    result.append("A:d ");
    result.append("A:p-r1 ");
    result.append("A:a ");
    result.append("A:p-r2 ");
    result.append("A:b ");
    result.append("D:a ");
    result.append("A:end p-r1 ");
    result.append("D:b ");
    result.append("A:end p-r2 ");
    result.append("D:d ");
    result.append("A:end p-r3 ");
    result.append("D:end p-r1 ");
    result.append("D:p-r1 ");
    result.append("D:end p-r2 ");
    result.append("D:p-r2 ");
    result.append("D:end p-r3 ");
    result.append("D:p-r3 ");
    result.append("D:p ");
    result.append("A:end");

    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);
    delete chart;
  }

  void testSemantics26() {
    Statechart* chart = Testcharts::c8();
    SmartPointer<Metadata> data(new TestMetadata());    

    TS_ASSERT(chart->start(data));

    string result = "D:start ";
    result.append("A:p ");
    // aktivieren der Regionen
    result.append("A:p-r1 ");
    result.append("A:start p-r1 ");
    result.append("D:start p-r1 ");
    result.append("A:a ");
    result.append("A:p-r2 ");
    result.append("A:start p-r2 ");
    result.append("D:start p-r2 ");
    result.append("A:b ");
    result.append("D:b ");
    result.append("A:c ");
    // Endtransitionen
    result.append("D:a ");
    result.append("D:p-r1 ");
    result.append("D:c ");
    result.append("D:p-r2 ");
    result.append("D:p ");
    result.append("A:join ");
    result.append("D:join ");
    result.append("A:end");

    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);
    delete chart;
  }

  void testSemantics27() {
    Statechart* chart = Testcharts::c9();
    SmartPointer<Metadata> data(new TestMetadata());

    TS_ASSERT(chart->start(data));

    // Wait until the statechart reached its final state
    State* current = 0; 
    while(!current || dynamic_cast<FinalState*>(current) == 0) {      
      data->lock();
      current = data->getData(chart)->currentState;
      data->unlock();
      Thread::sleep(100);    
    }

    string result = "D:start ";
    result.append("A:p ");
    // aktivieren der Regionen
    result.append("A:p-r1 ");
    result.append("A:start p-r1 ");
    result.append("D:start p-r1 ");
    result.append("A:a ");
    result.append("A:p-r2 ");
    result.append("A:start p-r2 ");
    result.append("D:start p-r2 ");
    result.append("A:b ");
    result.append("A:p-r3 ");
    result.append("A:start p-r3 ");
    result.append("D:start p-r3 ");
    result.append("A:d ");
    result.append("D:b ");
    result.append("A:c ");
    // Endtransitionen
    result.append("D:a ");
    result.append("D:p-r1 ");
    result.append("D:c ");
    result.append("D:p-r2 ");
    result.append("D:d ");
    result.append("D:p-r3 ");
    result.append("D:p ");
    result.append("A:join ");
    result.append("D:join ");
    result.append("A:end");

    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);
    delete chart;
  }

  void testSemantics28() {
    Statechart* chart = Testcharts::c10();
    SmartPointer<Metadata> data(new TestMetadata());
    SmartPointer<Event> event1(new TestEvent(1));
    SmartPointer<Event> event2(new TestEvent(2));
    TS_ASSERT(chart->start(data));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT(chart->dispatch(data, event2));
    TS_ASSERT(chart->dispatch(data, event1));
    TS_ASSERT(chart->dispatch(data, event2));
    TS_ASSERT(chart->dispatch(data, event2));

    string result = "D:start ";
    result.append("A:c ");
    result.append("D:c ");
    // S1
    result.append("A:p ");
    result.append("A:p-r1 ");
    result.append("A:start p-r1 ");
    result.append("D:start p-r1 ");
    result.append("A:end p-r1 ");
    result.append("A:p-r2 ");
    result.append("A:start p-r2 ");
    result.append("D:start p-r2 ");
    result.append("E:history ");
    result.append("A:a ");
    // S1
    result.append("D:a ");
    result.append("A:b ");
    // S2
    result.append("D:b ");
    result.append("D:p-r2 ");
    result.append("D:end p-r1 ");
    result.append("D:p-r1 ");
    result.append("D:p ");
    result.append("A:c ");
    // S1
    result.append("D:c ");
    result.append("A:p ");
    result.append("A:p-r1 ");
    result.append("A:start p-r1 ");
    result.append("D:start p-r1 ");
    result.append("A:end p-r1 ");
    result.append("A:p-r2 ");
    result.append("A:start p-r2 ");
    result.append("D:start p-r2 ");
    result.append("A:b ");
    // S2
    result.append("D:b ");
    result.append("D:p-r2 ");
    result.append("D:end p-r1 ");
    result.append("D:p-r1 ");
    result.append("D:p ");
    result.append("A:c ");
    // S2
    result.append("D:c ");
    result.append("A:end");

    TS_ASSERT_EQUALS(result, ((TestMetadata*)spGetImpl(data))->path);
    delete chart;
  }
};

#endif
