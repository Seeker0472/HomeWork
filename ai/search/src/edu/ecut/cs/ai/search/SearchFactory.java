package edu.ecut.cs.ai.search;

public class SearchFactory {
	public static SearchAlgorithm getSearchAlgorithm(String type) {
		SearchAlgorithm r = null;
		if ("WidthFirst".equals(type)) {
			r = new WidthFirst();
		} else if ("DepthFirst".equals(type)) {
			r = new DepthFirst();
		} else if ("AStar".equals(type)) {
			r = new AStar();
		}
		return r;
	}
}
