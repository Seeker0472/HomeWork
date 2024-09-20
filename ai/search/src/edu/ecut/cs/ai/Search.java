package edu.ecut.cs.ai;

import java.io.IOException;
import java.lang.reflect.Constructor;

import edu.ecut.cs.ai.search.SearchAlgorithm;
import edu.ecut.cs.ai.search.SearchFactory;
import edu.ecut.cs.ai.status.Status;

public class Search {
	public static void main(String[] args) {
		// 检查参数
		if (args.length != 3) {
			System.out.println("Only three parameters are permitted, a search alorithm, a class which implements the inteface of edu.ecut.cs.ai.status.Status, and a file which is used to output the solution found.");
			System.exit(-1);
		}
		
		//获取实现搜索算法的对象
		SearchAlgorithm sa = SearchFactory.getSearchAlgorithm(args[0]);
		if (sa == null) {
			System.out.println("Search alogithm not found: " + args[0]);
			System.exit(-1);			
		}
		
		// 初始化实现status接口的问题状态空间表示对象
		Status s = getStatus(args[1]);
		
		//初始化搜索树
		sa.init(s);
		
		//搜索
		sa.search();
		
		//输出搜索结果
		try {
			sa.outputSolution(args[2]);
		} catch (IOException e) {
			e.printStackTrace();
			System.exit(-1);
		}
	}
	
	private static Status getStatus(String className) {
		Status s = null;
		Class<?> statusClass = null;
		try {
			statusClass = Class.forName(className);
		} catch (ClassNotFoundException e) {
			System.out.println("Class not found: " + className);
			System.exit(-1);
		}		
		try {
			Constructor<?> statusConstructor = statusClass.getConstructor();
			s = (Status)statusConstructor.newInstance();
		} catch (Exception e) {
			e.printStackTrace();
			System.exit(-1);
		}
		
		return s;
	}
}
