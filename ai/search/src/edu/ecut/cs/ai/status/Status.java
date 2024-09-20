package edu.ecut.cs.ai.status;

import java.util.List;

public interface Status {
	public void getInitStatus(boolean randomly);        // 获取初始状态
	public List<Status> getChildren();    //获取所有可能的子节点
 	public Status getParent();     //获取当前节点的父借点
	public boolean sameAs(Status s);     //状态s是否与当前状态相同
	public boolean isTargetStatus();     //当前状态是否为目标状态
 	public boolean betterThan(Status s); //当前状态是否比状态s更优
	public String toString();    //输出当前状态内部表示
}
