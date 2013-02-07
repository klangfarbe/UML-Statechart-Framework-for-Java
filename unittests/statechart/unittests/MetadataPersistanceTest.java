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

import org.junit.Assert;
import org.junit.Test;
import statechart.Metadata;
import statechart.State;
import statechart.Statechart;
import statechart.StatechartException;

public class MetadataPersistanceTest {

  //============================================================================
  // Test persistance capability of the Metadata object
  //============================================================================
  @Test
  public void testRestoreState() throws StatechartException {
    Statechart chart = TestCharts.h5();
    State s3 = chart.getStateByName("c");
    State h1 = chart.getStateByName("p");
    State h2 = chart.getStateByName("q");

    TestParameter parameter = new TestParameter();
    Metadata data = new Metadata();

    chart.restoreState(s3, data, parameter);
    Assert.assertTrue(data.isActive(s3));
    Assert.assertTrue(data.isActive(h2));
    Assert.assertTrue(data.isActive(h1));
    Assert.assertTrue(data.isActive(chart));

    String result = "A:p A:q A:c";
    Assert.assertEquals(result, parameter.path);

    // check that the restore only works one time
    Assert.assertEquals(false, chart.restoreState(s3, data, parameter));

    chart.shutdown();
  }
}
