package edu.ecut.cs.ai.search;

import edu.ecut.cs.ai.status.*;
import java.util.*;

public class AStar extends SearchAlgorithm {
	public AStar() {
		MaxSteps = 362880;
	}



	@Override
	public void updateTree(Status child) {	
		// done
		// use sameAs to compare whether the same
		// if already in unextended Table - record the better way
		// if already in extended Table - update to best way
		// the description on the textbook is a dum shit, check these for a better approach
		// https://www.geeksforgeeks.org/a-search-algorithm/
		// https://en.wikipedia.org/wiki/A*_search_algorithm/

		for (Status s : extendedStatuses) {
			if(s.sameAs(child)){
				if(child.betterThan(s)){
					//child is better than s,add  the node to the open list
					unextendedStatuses.add(child);
				}else{
					//s is better than child,skip this child
				}
				return;
			}
		}
		for (Status s : unextendedStatuses) {
			if(s.sameAs(child)){
				if(child.betterThan(s)){
					unextendedStatuses.remove(s);
					unextendedStatuses.add(child);
				}
				return;
			}
		}
		unextendedStatuses.add(child);
	}
}
