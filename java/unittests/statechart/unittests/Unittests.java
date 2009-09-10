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
package statechart.unittests;

import org.junit.Assert;
import org.junit.Test;
import statechart.FinalState;
import statechart.Metadata;
import statechart.State;
import statechart.Statechart;
import statechart.StatechartException;

public class Unittests {
  @Test
  public void testEventQueue() throws StatechartException, InterruptedException {
    Statechart chart = TestCharts.t2();

    TestEvent s1 = new TestEvent(1);
    TestEvent s2 = new TestEvent(2);
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();
        
    chart.startAsynchron(data, parameter);    
    chart.dispatchAsynchron(data, s1, parameter);
    chart.dispatchAsynchron(data, s2, parameter); 
    
    // Wait until the statechart reached its final state
    State current = null; 
    while(current == null || !(current instanceof FinalState)) {      
      Thread.sleep(100);
      synchronized(data) {
        current = data.getData(chart).currentState;
      }
    }
    chart.shutdown();
    Assert.assertEquals("D:start A:a D:a A:a D:a A:end", parameter.path);
  }

  @Test
  public void testSemantics1() throws StatechartException {
    Statechart chart = TestCharts.t1();

    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertEquals("D:start A:a D:a A:b D:b A:end", parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics2() throws StatechartException {
    Statechart chart = TestCharts.t2();

    TestEvent s1 = new TestEvent(1);
    TestEvent s2 = new TestEvent(2);
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertTrue(chart.dispatch(data, s1, parameter));
    Assert.assertTrue(chart.dispatch(data, s2, parameter));
    Assert.assertEquals("D:start A:a D:a A:a D:a A:end", parameter.path);

    // check if more than one signals create a longer way
    parameter.path = "";

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertTrue(chart.dispatch(data, s1, parameter));
    Assert.assertTrue(chart.dispatch(data, s1, parameter));
    Assert.assertTrue(chart.dispatch(data, s2, parameter));
    Assert.assertEquals("D:start A:a D:a A:a D:a A:a D:a A:end", parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics3() throws StatechartException {
    Statechart chart = TestCharts.t3();

    TestEvent s1 = new TestEvent(1);
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertTrue(chart.dispatch(data, s1, parameter));
    Assert.assertEquals("D:start A:a D:a A:b D:b A:end", parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics4() throws StatechartException, InterruptedException {
    Statechart chart = TestCharts.t3();

    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));

    State current = null; 
    while(current == null || !(current instanceof FinalState)) {      
      Thread.sleep(100);
      synchronized(data) {
        current = data.getData(chart).currentState;
      }
    }

    Assert.assertEquals("D:start A:a D:a A:end", parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics5() throws StatechartException {
    Statechart chart = TestCharts.t4();

    TestParameter parameter = new TestParameter();
    parameter.guardvalue = 0;
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertEquals("D:start A:a D:a A:end", parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics6() throws StatechartException {
    Statechart chart = TestCharts.t4();

    TestParameter parameter = new TestParameter();
    parameter.guardvalue = 1;
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertEquals("D:start A:a D:a A:j1 D:j1 E:a1 A:b D:b A:end", parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics7() throws StatechartException {
    Statechart chart = TestCharts.t4();

    TestParameter parameter = new TestParameter();
    parameter.guardvalue = 2;
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertEquals(
        "D:start A:a D:a A:j1 D:j1 E:a2 A:c D:c A:j2 D:j2 E:a3 A:j3 D:j3 E:a4 A:end",
        parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics8() throws StatechartException {
    Statechart chart = TestCharts.h1();

    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertEquals(
        "D:start A:p A:start p D:start p A:a D:a A:b D:b A:end p D:end p D:p A:end",
        parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics9() throws StatechartException {
    Statechart chart = TestCharts.h2();

    TestEvent event = new TestEvent(1);
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertTrue(chart.dispatch(data, event, parameter));
    Assert.assertEquals(
        "D:start A:p A:start p D:start p A:a D:a A:b D:b A:end p D:end p D:p A:end",
        parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics10() throws StatechartException {
    Statechart chart = TestCharts.h3();

    TestEvent event = new TestEvent(1);
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertTrue(chart.dispatch(data, event, parameter));
    Assert.assertEquals(
        "D:start A:p A:start p D:start p A:a D:a A:b D:b A:end p D:end p D:p A:end",
        parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics11() throws StatechartException {
    Statechart chart = TestCharts.h3();

    TestEvent event = new TestEvent(2);
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertTrue(chart.dispatch(data, event, parameter));
    Assert.assertEquals("D:start A:p A:start p D:start p A:a D:a D:p A:end",
        parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics12() throws StatechartException {
    Statechart chart = TestCharts.h4();

    TestEvent event1 = new TestEvent(1);
    TestEvent event2 = new TestEvent(2);
    TestEvent event3 = new TestEvent(3);
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertTrue(chart.dispatch(data, event1, parameter));
    Assert.assertTrue(chart.dispatch(data, event2, parameter));
    Assert.assertTrue(chart.dispatch(data, event1, parameter));
    Assert.assertTrue(chart.dispatch(data, event2, parameter));
    Assert.assertTrue(chart.dispatch(data, event1, parameter));
    Assert.assertTrue(chart.dispatch(data, event2, parameter));
    Assert.assertTrue(chart.dispatch(data, event3, parameter));

    String result = "D:start A:p A:start p D:start p U:history p A:a D:a ";
    result += "D:p A:p A:start p D:start p A:a D:a A:b D:b D:p A:p ";
    result += "A:start p D:start p A:b D:b A:a D:a D:p A:p A:start p ";
    result += "D:start p A:a D:a A:b D:b A:end p D:end p D:p A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics13() throws StatechartException {
    Statechart chart = TestCharts.h5();

    TestEvent event1 = new TestEvent(1);
    TestEvent event2 = new TestEvent(2);
    TestEvent event3 = new TestEvent(3);
    TestEvent event4 = new TestEvent(4);
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertTrue(chart.dispatch(data, event2, parameter));
    Assert.assertTrue(chart.dispatch(data, event4, parameter));
    Assert.assertTrue(chart.dispatch(data, event1, parameter));
    Assert.assertTrue(chart.dispatch(data, event2, parameter));
    Assert.assertTrue(chart.dispatch(data, event2, parameter));
    Assert.assertTrue(chart.dispatch(data, event3, parameter));

    String result = "D:start A:p A:start p D:start p U:history p A:a D:a ";
    result += "A:q A:start q D:start q A:b D:b A:c D:c D:q D:p A:p ";
    result += "A:start p D:start p A:q A:c D:c D:q A:a D:a A:q ";
    result += "A:start q D:start q A:b D:b D:q A:end p D:end p D:p A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics14() throws StatechartException {
    Statechart chart = TestCharts.h6();

    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertEquals("D:start A:p A:q A:r D:r D:q D:p A:x A:y D:y D:x A:end",
        parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics15() throws StatechartException {
    Statechart chart = TestCharts.c1();
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));

    String result = "D:start ";
    result += "A:p ";
    result += "A:p-r1 ";
    result += "A:start p-r1 ";
    result += "D:start p-r1 ";
    result += "A:a ";
    result += "A:p-r2 ";
    result += "A:start p-r2 ";
    result += "D:start p-r2 ";
    result += "A:b ";
    result += "D:a ";
    result += "A:end p-r1 ";
    result += "D:b ";
    result += "A:end p-r2 ";
    result += "D:end p-r1 ";
    result += "D:p-r1 ";
    result += "D:end p-r2 ";
    result += "D:p-r2 ";
    result += "D:p ";
    result += "A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics16() throws StatechartException {
    Statechart chart = TestCharts.c2();

    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    TestEvent event1 = new TestEvent(1);
    TestEvent event2 = new TestEvent(2);

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertTrue(chart.dispatch(data, event2, parameter));
    Assert.assertTrue(chart.dispatch(data, event1, parameter));
    Assert.assertTrue(chart.dispatch(data, event2, parameter));

    String result = "D:start ";
    result += "A:p ";
    result += "A:p-r1 ";
    result += "A:start p-r1 ";
    result += "D:start p-r1 ";
    result += "A:a ";
    result += "A:p-r2 ";
    result += "A:start p-r2 ";
    result += "D:start p-r2 ";
    result += "A:c ";
    result += "D:a ";
    result += "A:b ";
    result += "D:b ";
    result += "A:end p-r1 ";
    result += "D:c ";
    result += "A:d ";
    result += "D:d ";
    result += "A:e ";
    result += "D:e ";
    result += "A:end p-r2 ";
    result += "D:end p-r1 ";
    result += "D:p-r1 ";
    result += "D:end p-r2 ";
    result += "D:p-r2 ";
    result += "D:p ";
    result += "A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics17() throws StatechartException {
    Statechart chart = TestCharts.c2();
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();
    TestEvent event1 = new TestEvent(1);
    TestEvent event2 = new TestEvent(2);

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertTrue(chart.dispatch(data, event1, parameter));
    Assert.assertTrue(chart.dispatch(data, event2, parameter));

    String result = "D:start ";

    result += "A:p "; 
    result += "A:p-r1 ";
    result += "A:start p-r1 ";
    result += "D:start p-r1 ";
    result += "A:a ";
    result += "A:p-r2 ";
    result += "A:start p-r2 ";
    result += "D:start p-r2 ";
    result += "A:c ";
    // c . d
    result += "D:c ";
    result += "A:d "; // a . b
    result += "D:a ";
    result += "A:b "; // d . e
    result += "D:d ";
    result += "A:e ";
    // b . end p-r2
    result += "D:b ";
    result += "A:end p-r1 "; // e . end p-r2
    result += "D:e ";
    result += "A:end p-r2 "; 
    result += "D:end p-r1 ";
    result += "D:p-r1 ";
    result += "D:end p-r2 ";
    result += "D:p-r2 ";
    result += "D:p ";
    result += "A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics18() throws StatechartException {
    Statechart chart = TestCharts.c2();
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();
    TestEvent event1 = new TestEvent(1);
    TestEvent event3 = new TestEvent(3);

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertTrue(chart.dispatch(data, event1, parameter));
    Assert.assertTrue(chart.dispatch(data, event3, parameter));

    String result = "D:start ";
    result += "A:p ";
    result += "A:p-r1 ";
    result += "A:start p-r1 ";
    result += "D:start p-r1 ";
    result += "A:a ";
    result += "A:p-r2 ";
    result += "A:start p-r2 ";
    result += "D:start p-r2 ";
    result += "A:c "; // c . d
    result += "D:c ";
    result += "A:d "; // S3 Transition
    result += "D:a ";
    result += "D:p-r1 ";
    result += "D:d ";
    result += "D:p-r2 ";
    result += "D:p ";
    result += "A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }

  public void testSemantics19() throws StatechartException {
    Statechart chart = TestCharts.c3();
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));

    String result = "D:start ";
    result += "A:p ";
    result += "A:p-r2 ";
    result += "A:start p-r2 ";
    result += "D:start p-r2 ";
    result += "A:b ";
    result += "A:p-r1 ";
    result += "A:a "; // a . end p-r1
    result += "D:a ";
    result += "A:end p-r1 "; // b . end p-r2
    result += "D:b ";
    result += "A:end p-r2 ";
    result += "D:end p-r1 ";
    result += "D:p-r1 ";
    result += "D:end p-r2 ";
    result += "D:p-r2 ";
    result += "D:p ";
    result += "A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }
  
  @Test
  public void testSemantics20() throws StatechartException {
    Statechart chart = TestCharts.c4();
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));

    String result = "D:start ";
    result += "A:p ";
    result += "A:p-r1 ";
    result += "A:start p-r1 ";
    result += "D:start p-r1 ";
    result += "A:a ";
    result += "A:p-r2 ";
    result += "A:start p-r2 ";
    result += "D:start p-r2 ";
    result += "A:b "; // a . end
    result += "D:a ";
    result += "D:p-r1 ";
    result += "D:b ";
    result += "D:p-r2 ";
    result += "D:p ";
    result += "A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics21() throws StatechartException {
    Statechart chart = TestCharts.c5();
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();
    TestEvent event1 = new TestEvent(1);
    TestEvent event2 = new TestEvent(2);

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertTrue(chart.dispatch(data, event1, parameter));
    Assert.assertTrue(!chart.dispatch(data, event2, parameter));

    String result = "D:start ";
    result += "A:p ";
    result += "A:p-r1 ";
    result += "A:start p-r1 ";
    result += "D:start p-r1 ";
    result += "A:a ";
    result += "A:p-r2 ";
    result += "A:start p-r2 ";
    result += "D:start p-r2 ";
    result += "A:b "; // a . end
    result += "D:a ";
    result += "D:p-r1 ";
    result += "D:b ";
    result += "D:p-r2 ";
    result += "D:p ";
    result += "A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics22() throws StatechartException {
    Statechart chart = TestCharts.c5();
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();
    TestEvent event1 = new TestEvent(1);
    TestEvent event2 = new TestEvent(2);

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertTrue(chart.dispatch(data, event2, parameter));
    Assert.assertTrue(chart.dispatch(data, event1, parameter));

    String result = "D:start ";
    result += "A:p ";
    result += "A:p-r1 ";
    result += "A:start p-r1 ";
    result += "D:start p-r1 ";
    result += "A:a ";
    result += "A:p-r2 ";
    result += "A:start p-r2 ";
    result += "D:start p-r2 ";
    result += "A:b "; // b . c
    result += "D:b ";
    result += "A:c "; // a . end
    result += "D:a ";
    result += "D:p-r1 ";
    result += "D:c ";
    result += "D:p-r2 ";
    result += "D:p ";
    result += "A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics23() throws StatechartException {
    Statechart chart = TestCharts.c6();
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));

    String result = "D:start ";
    result += "A:fork ";
    result += "D:fork ";
    result += "A:p ";
    result += "A:p-r1 ";
    result += "A:a ";
    result += "A:p-r2 ";
    result += "A:b ";
    result += "D:a ";
    result += "A:end p-r1 ";
    result += "D:b ";
    result += "A:end p-r2 ";
    result += "D:end p-r1 ";
    result += "D:p-r1 ";
    result += "D:end p-r2 ";
    result += "D:p-r2 ";
    result += "D:p ";
    result += "A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics24() throws StatechartException {
    Statechart chart = TestCharts.c7();
    TestParameter parameter = new TestParameter();
    parameter.guardvalue = 1;
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));

    String result = "D:start ";
    result += "A:fork ";
    result += "D:fork ";
    result += "A:p ";
    result += "A:p-r3 ";
    result += "A:c ";
    result += "A:p-r1 ";
    result += "A:a ";
    result += "A:p-r2 ";
    result += "A:b ";
    result += "D:a ";
    result += "A:end p-r1 ";
    result += "D:b ";
    result += "A:end p-r2 ";
    result += "D:c ";
    result += "A:end p-r3 ";
    result += "D:end p-r1 ";
    result += "D:p-r1 ";
    result += "D:end p-r2 ";
    result += "D:p-r2 ";
    result += "D:end p-r3 ";
    result += "D:p-r3 ";
    result += "D:p ";
    result += "A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics25() throws StatechartException {
    Statechart chart = TestCharts.c7();
    TestParameter parameter = new TestParameter();
    parameter.guardvalue = 0;
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));

    String result = "D:start ";
    result += "A:fork ";
    result += "D:fork ";
    result += "A:p ";
    result += "A:p-r3 ";
    result += "A:start p-r3 ";
    result += "D:start p-r3 ";
    result += "A:d ";
    result += "A:p-r1 ";
    result += "A:a ";
    result += "A:p-r2 ";
    result += "A:b ";
    result += "D:a ";
    result += "A:end p-r1 ";
    result += "D:b ";
    result += "A:end p-r2 ";
    result += "D:d ";
    result += "A:end p-r3 ";
    result += "D:end p-r1 ";
    result += "D:p-r1 ";
    result += "D:end p-r2 ";
    result += "D:p-r2 ";
    result += "D:end p-r3 ";
    result += "D:p-r3 ";
    result += "D:p ";
    result += "A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics26() throws StatechartException {
    Statechart chart = TestCharts.c8();
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));

    String result = "D:start ";
    result += "A:p ";
    result += "A:p-r1 ";
    result += "A:start p-r1 ";
    result += "D:start p-r1 ";
    result += "A:a ";
    result += "A:p-r2 ";
    result += "A:start p-r2 ";
    result += "D:start p-r2 ";
    result += "A:b ";
    result += "D:b ";
    result += "A:c "; 
    result += "D:a ";
    result += "D:p-r1 ";
    result += "D:c ";
    result += "D:p-r2 ";
    result += "D:p ";
    result += "A:join ";
    result += "D:join ";
    result += "A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics27() throws InterruptedException, StatechartException{
    Statechart chart = TestCharts.c9();
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    Assert.assertTrue(chart.start(data, parameter));
    
    State current = null; 
    while(current == null || !(current instanceof FinalState)) {      
      Thread.sleep(100);
      synchronized(data) {
        current = data.getData(chart).currentState;
      }
    }
    
    String result = "D:start ";
    result += "A:p ";
    result += "A:p-r1 ";
    result += "A:start p-r1 ";
    result += "D:start p-r1 ";
    result += "A:a ";
    result += "A:p-r2 ";
    result += "A:start p-r2 ";
    result += "D:start p-r2 ";
    result += "A:b ";
    result += "A:p-r3 ";
    result += "A:start p-r3 ";
    result += "D:start p-r3 ";
    result += "A:d ";
    result += "D:b ";
    result += "A:c ";
    result += "D:a ";
    result += "D:p-r1 ";
    result += "D:c ";
    result += "D:p-r2 ";
    result += "D:d ";
    result += "D:p-r3 ";
    result += "D:p ";
    result += "A:join ";
    result += "D:join ";
    result += "A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }

  @Test
  public void testSemantics28() throws StatechartException {
    Statechart chart = TestCharts.c10();
    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();
    TestEvent event1 = new TestEvent(1);
    TestEvent event2 = new TestEvent(2);

    Assert.assertTrue(chart.start(data, parameter));
    Assert.assertTrue(chart.dispatch(data, event1, parameter));
    Assert.assertTrue(chart.dispatch(data, event1, parameter));
    Assert.assertTrue(chart.dispatch(data, event2, parameter));
    Assert.assertTrue(chart.dispatch(data, event1, parameter));
    Assert.assertTrue(chart.dispatch(data, event2, parameter));
    Assert.assertTrue(chart.dispatch(data, event2, parameter));

    String result = "D:start ";
    result += "A:c ";
    result += "D:c ";
    // S1
    result += "A:p ";
    result += "A:p-r1 ";
    result += "A:start p-r1 ";
    result += "D:start p-r1 ";
    result += "A:end p-r1 ";
    result += "A:p-r2 ";
    result += "A:start p-r2 ";
    result += "D:start p-r2 ";
    result += "E:history ";
    result += "A:a "; // S1
    result += "D:a ";
    result += "A:b "; // S2
    result += "D:b ";
    result += "D:p-r2 ";
    result += "D:end p-r1 ";
    result += "D:p-r1 ";
    result += "D:p ";
    result += "A:c "; // S1
    result += "D:c ";
    result += "A:p ";
    result += "A:p-r1 ";
    result += "A:start p-r1 ";
    result += "D:start p-r1 ";
    result += "A:end p-r1 ";
    result += "A:p-r2 ";
    result += "A:start p-r2 ";
    result += "D:start p-r2 ";
    result += "A:b "; // S2
    result += "D:b ";
    result += "D:p-r2 ";
    result += "D:end p-r1 ";
    result += "D:p-r1 ";
    result += "D:p ";
    result += "A:c "; // S2
    result += "D:c ";
    result += "A:end";

    Assert.assertEquals(result, parameter.path);
    chart.shutdown();
  }
}
