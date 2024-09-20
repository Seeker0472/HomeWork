package edu.ecut.cs.ai.status;

import java.util.ArrayList;
import java.util.List;

public class Travel implements Status {
	static final int initStatus[] = {2,1};
	static final int targetStatus[] = {2, 4};
	Travel parent;
	int[] currentStatus;
	int g;
	double h;

	@Override
	public void getInitStatus(boolean randomly) {
		parent = null;
		if (randomly) {
			int nums[] = new int[2];
			do {
				for (int i = 0; i < 2; i++) {
					nums[i] = (int)(Math.random()*4);
				}
			} while (isForbidden(nums));
			currentStatus = nums;
		} else {
			currentStatus = initStatus;
		}
		g = 0;
		setHValue();  
	}
	//Helper Function to deep clone int[] Objects
	private int[] deepClone(int[] source) {
		int[] copy = new int[source.length];
		for (int i = 0; i < source.length; i++) {
				copy[i] = source[i];
		}
		return copy;
	}
	public void setStatus(int[] nums) {
		currentStatus = nums;
		setHValue();
	}
	@Override
	public List<Status> getChildren() {
		List<Status> r = new ArrayList<Status>();
		// done
		for (int i = 0; i < 4; i++) {
			Travel st = new Travel();
			int[] status = deepClone(currentStatus);
			switch (i) {
				case 0:
					status[0] -= 1;
					if (isForbidden(status))
						continue;
					break;
				case 1:
					status[0] += 1;
					if (isForbidden(status))
						continue;
					break;
				case 2:
					status[1] += 1;
					if (isForbidden(status))
						continue;
					break;
				case 3:
					status[1] -= 1;
					if (isForbidden(status))
						continue;
					break;
			}
			st.setStatus(status);
			st.parent = this;
			st.setHValue();
			st.g=this.g+1;
			r.add(st);
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
		if (s instanceof Travel) {	
			Travel other = (Travel)s;
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
		if (s instanceof Travel) {	
			Travel other = (Travel)s;
			if (this.g + this.h < other.g + other.h) {
				r = true;
			}  
		}
		return r;
	}
	
	private void setHValue() {
		// done
		//simply return the distance
		h=Math.abs(currentStatus[1]-targetStatus[1])+Math.abs(currentStatus[0]-targetStatus[0]);
	}
	
	private boolean isForbidden(int[] s) {
		return (s[0] < 1 || s[0] > 4) ||
				(s[1] < 1 || s[1] > 4) ||
				(s[0] == 2 || s[0] == 3) && s[1] == 2;
	}
}