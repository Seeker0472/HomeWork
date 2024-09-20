package edu.ecut.cs.ai.search;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Stack;

import edu.ecut.cs.ai.status.Status;

public abstract class SearchAlgorithm {
	// searchResult       0： 搜索步数超过最大阈值；-1：所有节点已扩展，仍未找到目标节点；1：搜索成功
	public static final int Succeed = 1;
	public static final int No_Status_To_Extend = -1;
	public static final int No_More_Step = 0;
	
	int MaxSteps;
	int steps;
	int searchResult;   
	Status currentStatus;

	List<Status> extendedStatuses;
	List<Status> unextendedStatuses;

	public Status getCurrentStatus() {
		return currentStatus;
	}
	public void setCurrentStatus(Status s) {
		currentStatus = s;
	}
	
	public List<Status> getExtendedStatuses() {
		return extendedStatuses;
	}
	public void setExtendedStatuses(List<Status> ss) {
		extendedStatuses = ss;
	}
	
	public List<Status> getUnextendedStatuses() {
		return unextendedStatuses;
	}
	public void setUnextendedStatuses(List<Status> ss) {
		unextendedStatuses = ss;
	}
	
	public int getSearchResult() {
		return searchResult;
	}
	
	public int getSteps() {
		return steps;
	}
	public abstract void updateTree(Status child);
	
	public void search() {
		if(unextendedStatuses==null&&currentStatus==null){
			this.searchResult=No_Status_To_Extend;
			return;
		}
		//if null,get one
		if(currentStatus==null){
			currentStatus=unextendedStatuses.get(0);
			unextendedStatuses.remove(0);
		}
		// done
		while(true) {
			//in this Loop:get children then undate tree?
			if(steps>MaxSteps){
				//Failed to fetch an Answer while Exceed MaxSteps
				this.searchResult=No_More_Step;
				return;
			}
			steps++;
			List<Status> childs=currentStatus.getChildren();
			for(Status child:childs) {
				updateTree(child);
			}
			//Why not Employ PriorityQueue?
			unextendedStatuses.sort(new Comparator<Status>() {
				public int compare(Status s1, Status s2) {
					return s1.betterThan(s2) ? -1:1;
				}
			});
			extendedStatuses.add(currentStatus);
			if(unextendedStatuses.isEmpty()&&!currentStatus.isTargetStatus()) {
				//No status to extend , problem unsolvable
				this.searchResult=No_Status_To_Extend;
				return;
			}
			//get next status
			currentStatus=unextendedStatuses.getFirst();
			if(currentStatus.isTargetStatus()) {
				//Reach The target Status,Exiting
				this.searchResult=Succeed;
				break;
			}
			//remove from unexpected
			unextendedStatuses.remove(currentStatus);
		}

	}
	
	public void init(Status s) {
		extendedStatuses = new ArrayList<Status>();
		unextendedStatuses = new ArrayList<Status>();	
		s.getInitStatus(false);
		unextendedStatuses.add(s);
	}
	
	public void outputSolution(String fileName) throws IOException {
		Stack<Status> solution = new Stack<Status>();
		Status parent = currentStatus;
		while (parent != null) {
			solution.push(parent);
			parent = parent.getParent();
		}

		String answer = null;
		File f = new File(fileName);
		Writer w = new FileWriter(f);
		switch (searchResult) {
		case Succeed:
			int len = solution.size() - 1;
			answer = "Task is done in " + steps + " steps. Length of the solution is " + len + ".\n";
			break;
		case No_Status_To_Extend:
			answer = "Task is unresolved. Search space is exhausted in " + steps + " steps.\n";
			break;
		case No_More_Step:
			answer = "Task is unresolved in " + MaxSteps + " steps.\n";
			break;
		}
		int i = 0;
		Status s;
		while (!solution.empty()) {
			s = solution.pop();
			answer += "S_"+i+": " + s.toString();
			i++;
		}
		w.write(answer);
		w.close();
		
		System.out.println(answer);
	}
	
	// 在状态列表中查找特定状态，返回找到状态的序号，未找到返回-1
	protected static int findStatus(Status s, List<Status> l) {
		int r = -1;
		Status x;
		for (int i = 0; i < l.size(); i++) {
			x = l.get(i);
			if (s.sameAs(x)) {
				r = i;
				break;
			} 
		}
		return r;
	}	
}
