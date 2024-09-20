package edu.ecut.cs.ai.status;

import java.util.ArrayList;
import java.util.List;

public class Hanoi2 implements Status {
	static final int initStatus[] = {1,1};
	static final int targetStatus[] = {3, 3};
	Hanoi2 parent;
	int[] currentStatus;  //当前状态
	int g;
	int h;

	@Override
	public void getInitStatus(boolean randomly) {
		parent = null;
		if (randomly) {
			int nums[] = new int[2];
			for (int i = 0; i < 2; i++) {
				nums[i] = (int)(Math.random()*3);
			}
			currentStatus = nums;
		} else {
			currentStatus = initStatus;
		}
		g = 0;
		setHValue(); 
	}

	@Override
	public List<Status> getChildren() {
		List<Status> r = new ArrayList<Status>();
		Hanoi2 child;
		// 移动小圆盘
		for (int i = 1; i <= 3; i++) {
			if (this.currentStatus[0] != i) {
				child = new Hanoi2();
				child.parent = this;
				child.currentStatus = this.currentStatus.clone();
				child.currentStatus[0] = i;
				child.g = this.g + 1;
				child.setHValue();
				r.add(child);				
			}
		}
		
		// 移动大圆盘
		if (this.currentStatus[0] != this.currentStatus[1]) {
			for (int i = 1; i <= 3; i++) {			
				if (this.currentStatus[0] != i && this.currentStatus[1] != i) {
					child = new Hanoi2();
					child.parent = this;
					child.currentStatus = this.currentStatus.clone();
					child.currentStatus[1] = i;
					child.g = this.g + 1;
					child.setHValue();
					r.add(child);
					break;
				}
			}
		}
		
		return r;
	}

	@Override
	public Status getParent() {
		return parent;
	}

	@Override
	public boolean sameAs(Status s) {
		boolean r = true;
		
		if (s instanceof Hanoi2) {	
			Hanoi2 other = (Hanoi2)s;
			for (int i = 0; i < 2; i++) {
				if (this.currentStatus[i] != other.currentStatus[i]) {
					r = false;
					break;
				}
			}
		} else {
			r = false;
		}
		
		return r;
	}

	@Override
	public boolean isTargetStatus() {
		boolean r = true;
		
		for (int i = 0; i < 2; i++) {
			if (this.currentStatus[i] != targetStatus[i]) {
				r = false;
				break;
			}
		}
		return r;
	}

	@Override
	public String toString() {
		String s = "G="+g+", H="+h+"\n";
		s += "\t{" + currentStatus[0] + ", " + currentStatus[1] + "}\n";
		return s;
	}

	
	public boolean betterThan(Status s) {
		boolean r = false;
		if (s instanceof Hanoi2) {	
			Hanoi2 other = (Hanoi2)s;
			if (this.g + this.h < other.g + other.h) {
				r = true;
			}  
		}
		return r;
	}
	
	private void setHValue() {
		h = 0;
		for (int i = 0; i < 2; i++) {
			if (currentStatus[i] != targetStatus[i]) h++;
		}
	}
	
}
