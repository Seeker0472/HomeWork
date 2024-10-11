package cn.xcx.fopf;

import java.util.ArrayList;
import java.util.List;

import com.alibaba.fastjson.annotation.JSONField;

public class Function extends Term {
    private List<Term> terms;    
    @JSONField(serialize=false)
    public List<Term> getTerms() {
    	return terms;
    }
    public void setTerms(List<Term> l){
    	terms = l;
    }

    @JSONField(name = "children", ordinal = 2)
    public List<Term> getChildren() {
    	return terms;
    }
    public void setChildren(List<Term> l){
    	terms = l;
    }
    
    void setVariable(Term v) {
    	for (Term t: terms) {
    		t.setVariable(v);
    	}
    }
    
    public Function() {
    	setType(T_FUNCTION);
    }
    
    public boolean equals(Term f) {
    	boolean r = true;
    	
    	if (f instanceof Function && getName().equals(f.getName())) {
    		List<Term> ts = getTerms();
    		if (ts.size() == ((Function)f).getTerms().size()) {
    			for (int i = 0; i < ts.size(); i++) {
    				if (!ts.get(i).equals(((Function)f).getTerms().get(i))) {
    					r = false;
    					break;
    				}
    			}
    		} else {
    			r = false;
    		}
    	} else {
    		r = false;
    	}
    	
    	return r;
    }
    
    public Function clone() {
    	Function r = new Function();
    	
    	r.setName(this.getName());
    	r.setType(this.getType());
    	
    	List<Term> ts = new ArrayList<Term>();
    	for (Term t: terms) {
    		ts.add(t.clone());
    	}
    	r.setTerms(ts);
    	
    	return r;
    }
    
    public void addPrefixToVariables(String prefix) {
    	for (Term t: this.getTerms()) {
	    	if (t.getType() == T_VARIABLE) {
	    		t.setName(prefix + this.getName());
			} else if (t.getType() == T_FUNCTION) {
				((Function)t).addPrefixToVariables(prefix);
			}
    	}
    }
}
