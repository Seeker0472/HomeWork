package cn.xcx.fopf;

import java.util.ArrayList;
import java.util.List;

import com.alibaba.fastjson.annotation.JSONField;

public abstract class BinaryFormula extends Formula {
    private Formula leftChild;
    private Formula rightChild;
    
    @JSONField(serialize=false)
    public Formula getLeftChild(){
    	return leftChild;
    }
    public void setLeftChild(Formula c){
    	leftChild = c;
    }
    
    @JSONField(serialize=false)
    public Formula getRightChild(){
    	return rightChild;
    }
    public void setRightChild(Formula c){
    	rightChild = c;
    }

    @JSONField(name = "children", ordinal = 2)
    public List<Object> getChildren() {
    	List<Object> ls = new ArrayList<Object>();    	

    	List<Quantifier> qs = this.getQuantifiers();
    	if (qs != null) ls.addAll(qs);
    	
    	ls.add(leftChild);
    	ls.add(rightChild);
    	
    	return ls;
    }
    public void setChildren(List<Object> children) {
    	for (Object obj: children) {
    		if (obj instanceof Quantifier) {
    			appendQuantifier((Quantifier) obj);
    		} else {
    			break;
    		}
    	}
    	setLeftChild((Formula) children.get(children.size()-2));
    	setRightChild((Formula) children.get(children.size()-1));
    }
    
    public BinaryFormula(Formula l, Formula r) {
    	leftChild = l;
    	rightChild = r;
    }
	public BinaryFormula() {
		super();
	}
}
