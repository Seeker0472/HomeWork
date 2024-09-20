package edu.ecut.cs.ai.search;

import edu.ecut.cs.ai.status.Status;

public class DepthFirst extends SearchAlgorithm {
	public DepthFirst() {
		MaxSteps = 362880;
	}

	@Override
	public void updateTree(Status child) {	
		int r = findStatus(child, unextendedStatuses);
		if (r >= 0) {
			unextendedStatuses.remove(r);						
		}
		unextendedStatuses.add(0, child);
	}

}
