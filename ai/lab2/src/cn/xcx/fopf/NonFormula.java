package cn.xcx.fopf;

import java.util.ArrayList;
import java.util.List;

import com.alibaba.fastjson.annotation.JSONField;

public class NonFormula extends Formula {
    private Formula child;
    @JSONField(serialize=false)
    public Formula getChild(){
    	return child;
    }
    public void setChild(Formula c){
    	child = c;
    }
    
    
    
    @JSONField(name = "name", ordinal = 1)
    public String getName() {
    	return String.valueOf(Formula.OP_NOT);
    }
    
    @JSONField(name = "children", ordinal = 2)
    public List<Object> getChildren() {
    	List<Object> ls = new ArrayList<Object>();
    	
    	List<Quantifier> qs = this.getQuantifiers();
    	if (qs != null) ls.addAll(qs);
    	
    	ls.add(child);
    	
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
    	setChild((Formula) children.get(children.size()-1));
    }
    
    public NonFormula() {
    	super();
    }
    public NonFormula(Formula c) {
    	child = c;
    }
    
    public NonFormula clone() {
    	NonFormula r = new NonFormula(child.clone());
    	
    	r.cloneQuantifiersFrom(this);
    	
    	return r;
    }
}
