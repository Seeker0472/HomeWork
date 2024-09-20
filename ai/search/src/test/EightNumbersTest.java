package test;

import static org.junit.jupiter.api.Assertions.*;

import java.util.ArrayList;
import java.util.List;

import edu.ecut.cs.ai.status.Travel;
import org.junit.jupiter.api.Test;

import edu.ecut.cs.ai.search.AStar;
import edu.ecut.cs.ai.search.SearchAlgorithm;
import edu.ecut.cs.ai.status.EightNumbers;
import edu.ecut.cs.ai.status.Status;


class EightNumbersTest {
	@Test
	void test_getChildren_1() {
		int initStatus[][] = {{6,3,7},{8,5,0},{2,1,4}};
		EightNumbers input =  new EightNumbers();
		input.setStatus(initStatus);
		
		EightNumbers obj;
		List<Status> output = new ArrayList<Status>();
		int s1[][] = {{6,3,0},{8,5,7},{2,1,4}};
		obj = new EightNumbers();
		obj.setStatus(s1);
		output.add(obj);
		
		int s2[][] = {{6,3,7},{8,5,4},{2,1,0}};
		obj = new EightNumbers();
		obj.setStatus(s2);
		output.add(obj);

		int s3[][] = {{6,3,7},{8,0,5},{2,1,4}};
		obj = new EightNumbers();
		obj.setStatus(s3);
		output.add(obj);
		
		List<Status> output1 = input.getChildren();
		
		boolean same;
		assertEquals(output1.size(), 3);
		for (int i = 0; i < 3; i++) {
			same = false;
			for (int j = 0; j < 3; j++) {
				if (output.get(i).sameAs(output1.get(j)) && output1.get(j).getParent().sameAs(input)) {
					same = true;
					break;
				}
			}
			assertEquals(same, true);
		}
	}
	
	@Test
	void test_getChildren_2() {		
		int initStatus[][] = {{6,3,7},{8,0,5},{2,1,4}};
		EightNumbers input =  new EightNumbers();
		input.setStatus(initStatus);
		
		EightNumbers obj;
		List<Status> output = new ArrayList<Status>();
		int s1[][] = {{6,0,7},{8,3,5},{2,1,4}};
		obj = new EightNumbers();
		obj.setStatus(s1);
		output.add(obj);
		
		int s2[][] = {{6,3,7},{8,1,5},{2,0,4}};
		obj = new EightNumbers();
		obj.setStatus(s2);
		output.add(obj);

		int s3[][] = {{6,3,7},{0,8,5},{2,1,4}};
		obj = new EightNumbers();
		obj.setStatus(s3);
		output.add(obj);

		int s4[][] = {{6,3,7},{8,5,0},{2,1,4}};
		obj = new EightNumbers();
		obj.setStatus(s4);
		output.add(obj);
		
		List<Status> output1 = input.getChildren();
		
		boolean same;
		assertEquals(output1.size(), 4);
		for (int i = 0; i < 4; i++) {
			same = false;
			for (int j = 0; j < 4; j++) {
				if (output.get(i).sameAs(output1.get(j)) && output1.get(j).getParent().sameAs(input)) {
					same = true;
					break;
				}
			}
			assertEquals(same, true);
		}
	}
	
	@Test
	void test_getChildren_3() {		
		int initStatus[][] = {{6,3,0},{8,5,7},{2,1,4}};
		EightNumbers input =  new EightNumbers();
		input.setStatus(initStatus);
		
		EightNumbers obj;
		List<Status> output = new ArrayList<Status>();
		int s1[][] = {{6,3,7},{8,5,0},{2,1,4}};
		obj = new EightNumbers();
		obj.setStatus(s1);
		output.add(obj);
		
		int s2[][] = {{6,0,3},{8,5,7},{2,1,4}};
		obj = new EightNumbers();
		obj.setStatus(s2);
		output.add(obj);
		
		List<Status> output1 = input.getChildren();
		
		boolean same;
		assertEquals(output1.size(), 2);
		for (int i = 0; i < 2; i++) {
			same = false;
			for (int j = 0; j < 2; j++) {
				if (output.get(i).sameAs(output1.get(j)) && output1.get(j).getParent().sameAs(input)) {
					same = true;
					break;
				}
			}
			assertEquals(same, true);
		}
	}
	

	
	@Test
	void test_setHValue() {		
		int initStatus[][] = {{6,3,0},{8,5,7},{2,1,4}};
		EightNumbers input =  new EightNumbers();
		input.setStatus(initStatus);
		input.setHValue();
		
		assertEquals(input.getHValue(), 8);
	}
	
	@Test
	void test_search_1() {
		int initStatus[][] = {{6,3,7},{8,5,0},{2,1,4}};
		EightNumbers input =  new EightNumbers();
		input.setStatus(initStatus);
		
		SearchAlgorithm sa = new AStar();
		sa.init(new EightNumbers());
		sa.getUnextendedStatuses().remove(0);
		sa.getUnextendedStatuses().add(input);		
		sa.search();
		
		assertEquals(sa.getSearchResult(), SearchAlgorithm.Succeed);
		assertEquals(sa.getSteps(), 5616);
	}

	
	@Test
	void test_search_2() {		
		int initStatus[][] = {{5, 1, 7},{6, 2, 3},{0 , 4, 8}};
		EightNumbers input =  new EightNumbers();
		input.setStatus(initStatus);	
		
		SearchAlgorithm sa = new AStar();
		sa.init(new EightNumbers());
		sa.getUnextendedStatuses().remove(0);
		sa.getUnextendedStatuses().add(input);		
		sa.search();
		
		assertEquals(sa.getSearchResult(), SearchAlgorithm.Succeed);
		assertEquals(sa.getSteps(), 7576);
	}

	@Test
	void test_search_X() {
		int initStatus[][] = {{1,2,3},{8,4,5},{7,6,0}};
		EightNumbers input =  new EightNumbers();
		input.setStatus(initStatus);

		SearchAlgorithm sa = new AStar();
		sa.init(new EightNumbers());
		sa.getUnextendedStatuses().remove(0);
		sa.getUnextendedStatuses().add(input);
		sa.search();

		assertEquals(sa.getSearchResult(), SearchAlgorithm.Succeed);
		assertEquals(sa.getSteps(), 2);
	}

	@Test
	void test_search_Travel() {
		Travel input =  new Travel();
		input.getInitStatus(false);

		SearchAlgorithm sa = new AStar();
		sa.init(new Travel());
		sa.getUnextendedStatuses().remove(0);
		sa.getUnextendedStatuses().add(input);
		sa.search();

		assertEquals(sa.getSearchResult(), SearchAlgorithm.Succeed);
		assertEquals(sa.getSteps(), 7);
	}
	@Test
	void test_search_Travel_1() {
		//WTF? not started with zero????
		int initStatus[] = {1,1};
		Travel input =  new Travel();
		input.setStatus(initStatus);

		SearchAlgorithm sa = new AStar();
		sa.init(new Travel());
		sa.getUnextendedStatuses().remove(0);
		sa.getUnextendedStatuses().add(input);
		sa.search();

		assertEquals(sa.getSearchResult(), SearchAlgorithm.Succeed);
		assertEquals(sa.getSteps(), 6);
	}
	@Test
	void test_search_Travel_Wrong_Status() {
		//WTF? not started with zero????
		int initStatus[] = {0,0};
		Travel input =  new Travel();
		input.setStatus(initStatus);

		SearchAlgorithm sa = new AStar();
		sa.init(new Travel());
		sa.getUnextendedStatuses().remove(0);
		sa.getUnextendedStatuses().add(input);
		sa.search();

		assertEquals(sa.getSearchResult(), SearchAlgorithm.No_Status_To_Extend);
	}
}
