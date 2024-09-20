package edu.ecut.cs.ai.status;

import java.util.ArrayList;
import java.util.List;

public class EightNumbers implements Status {
	public static final int initStatus[][] = { { 6, 3, 7 }, { 8, 5, 0 }, { 2, 1, 4 } };
	public static final int targetStatus[][] = { { 1, 2, 3 }, { 8, 0, 4 }, { 7, 6, 5 } };
	private EightNumbers parent;
	private int[][] currentStatus;
	private int[] spaceAt;

	private int g;
	private int h;

	double lambda;

	public EightNumbers() {
		currentStatus = new int[3][3];
		spaceAt = new int[2];
		lambda = 0.5;
	}

	public EightNumbers(double v) {
		currentStatus = new int[3][3];
		spaceAt = new int[2];
		lambda = v;
	}

	@Override
	public void getInitStatus(boolean randomly) {
		parent = null;
		if (randomly) {
			int[][] status = new int[3][3];
			int nums[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
			int n;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					do {
						n = (int) (Math.random() * 9);
					} while (nums[n] == -1); // 找到一个未放入9宫格的数
					nums[n] = n;
					status[i][j] = n;
				}
			}
			setStatus(status);
		} else {
			setStatus(initStatus);
		}
		g = 0;
	}

	public void setStatus(int[][] nums) {
		currentStatus = nums;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (nums[i][j] == 0) {
					spaceAt[0] = i;
					spaceAt[1] = j;
				}
			}
		}
		setHValue();

	}

	@Override
	public Status getParent() {
		return parent;
	}

	@Override
	public boolean sameAs(Status s) {
		boolean r = true;
		if (s instanceof EightNumbers) {
			EightNumbers other = (EightNumbers) s;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (this.currentStatus[i][j] != other.currentStatus[i][j]) {
						r = false;
						break;
					}
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
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (this.currentStatus[i][j] != targetStatus[i][j]) {
					r = false;
					break;
				}
			}
		}
		return r;
	}

	@Override
	public String toString() {
		String s = "G=" + g + ", H=" + h + "\n";
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (j == 0) {
					s += "\t|";
				}
				if (currentStatus[i][j] == 0) {
					s += " ";
				} else {
					s += String.valueOf(currentStatus[i][j]);
				}
				if (j == 0 || j == 1) {
					s += "\t";
				}
				if (j == 2) {
					s += "|\n";
				}
			}
		}
		return s;
	}

	public boolean betterThan(Status s) {
		boolean r = false;
		if (s instanceof EightNumbers) {
			EightNumbers other = (EightNumbers) s;
			if (lambda * this.g + (1 - lambda) * this.h < lambda * other.g + (1 - lambda) * other.h) {
				r = true;
			}
		}
		return r;
	}
	//Helper Function to deep clone int[][] Objects
	private int[][] deepClone(int[][] source) {
		int[][] copy = new int[source.length][];
		for (int i = 0; i < source.length; i++) {
			copy[i] = new int[source[i].length];
			for (int j = 0; j < source[i].length; j++) {
				copy[i][j] = source[i][j];
			}
		}
		return copy;
	}
	// Helper Function to generate child state
	private boolean swapStatus(int[][] source, int i, int j) {
		if (i < 0 || i > 2 || j < 0 || j > 2) {
			return true;
		}
		int temp = source[i][j];
		source[i][j] = source[spaceAt[0]][spaceAt[1]];
		source[spaceAt[0]][spaceAt[1]] = temp;
		return false;
	}

	@Override
	public List<Status> getChildren() {
		List<Status> ls = new ArrayList<Status>();
		// done
		// have 2/3/4 children,then set Parent
		for (int i = 0; i < 4; i++) {
			EightNumbers st = new EightNumbers();
			int[][] status = deepClone(currentStatus);
			switch (i) {
				case 0:
					if(swapStatus(status, spaceAt[0] + 1, spaceAt[1]))
						continue;
					break;
				case 1:
					if(swapStatus(status, spaceAt[0] - 1, spaceAt[1]))
						continue;
					break;
				case 2:
					if(swapStatus(status, spaceAt[0], spaceAt[1] + 1))
						continue;
					break;
				case 3:
					if(swapStatus(status, spaceAt[0], spaceAt[1] - 1))
						continue;
					break;
			}
			st.setStatus(status);
			st.parent = this;
			st.setHValue();
			st.g=this.g+1;
			ls.add(st);
		}
		return ls;
	}

	public void setHValue() {
		// done
		h = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (currentStatus[i][j] != targetStatus[i][j]) h++;
			}
		}
	}

	public int getHValue() {
		return h;
	}
}
