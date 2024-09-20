package edu.ecut.cs.ai.search;

import edu.ecut.cs.ai.status.Status;

public class WidthFirst extends SearchAlgorithm {
	public WidthFirst() {
		MaxSteps = 362880;
	}

	@Override
	public void updateTree(Status child) {	
		if (findStatus(child, unextendedStatuses) == -1) {
			unextendedStatuses.add(child);						
		}
	}

}
