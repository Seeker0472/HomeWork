package cn.xcx.fopf;

import java.util.ArrayList;
import java.util.List;

import com.alibaba.fastjson.annotation.JSONField;

import cn.xcx.fopf.exception.UnexpectedCharacterException;

public class AtomicFormula extends Formula {
    private String name;    
    private List<Term> terms;    
    
    @JSONField(name = "name", ordinal = 1)
    public String getName() {
    	return name;
    }
    public void setName(String s){
    	name = s;
    }
    
    @JSONField(name = "children", ordinal = 2)
    public List<Object> getChildren() {
    	List<Object> ls = new ArrayList<Object>();
    	
    	List<Quantifier> qs = this.getQuantifiers();
    	if (qs != null) ls.addAll(qs);
    	
    	ls.addAll(terms);
    	
    	return ls;
    }
    public void setChildren(List<Object> children) {
    	for (Object obj: children) {
    		if (obj instanceof Quantifier) {
    			appendQuantifier((Quantifier) obj);
    		} else {
    			if (terms == null) terms = new ArrayList<Term>();
    			terms.add((Term) obj);
    		}
    	}
    }
    
    @JSONField(name = "collapsed", ordinal = 3)
    public boolean getCollapsed() {
    	return true;
    }
    
    @JSONField(serialize=false)
    public List<Term> getTerms() {
    	return terms;
    }
    public void setTerms(List<Term> l) {
    	terms = l;
    }
    
    //设置公式中同名的项为变元，调用时，公式的项列表应该已生成
    protected void setVariable(Term v) {
    	//assertEquals(terms == null, false, "Terms of the AtomicFormula are not set before setVariable()");
    	for (Term t: terms) {
    		t.setVariable(v);
    	}
    }
    
    public AtomicFormula clone() {
    	AtomicFormula r = new AtomicFormula();
    	
    	r.setName(name);
    	r.cloneQuantifiersFrom(this);
    	
    	List<Term> ts = new ArrayList<Term>();
    	for (Term t: terms) {
    		ts.add(t.clone());
    	}
    	r.setTerms(ts);
    	
    	return r;
    }
    
    public boolean equals(AtomicFormula a) {
    	boolean r = true;
    	
    	if (name.equals(a.getName())) {
    		List<Term> ts = a.getTerms();
    		if (terms.size() == ts.size()) {
		    	for (int i = 0; i < terms.size(); i++) {
		    		if (!terms.get(i).equals(ts.get(i))) {
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
    
    static Object[] parse(String s, int from) throws UnexpectedCharacterException {
    	//assertEquals(startsAName(s.charAt(from)), true, "Unexpected character at (" + s.substring(from) + "). English alphabet is expected");
    	
    	AtomicFormula f = new AtomicFormula();
    	
    	int status = 3;   	
    	char c;
    	StringBuffer name = new StringBuffer();
    	name.append(s.charAt(from));
    	int i = from+1;
    	boolean done = false;
    	while (i < s.length() && !done) {
    		c = s.charAt(i);
    		switch (status) {
    		case 3:
    			if (c == C_SPACE) {
    				f.setName(name.toString());
    				status = 4;
    			} else if (c == '(') {
    				f.setName(name.toString());
        			Object[] rs = Term.parseTerms(s, i); // 解析项列表
        			f.setTerms((List<Term>) rs[0]);
        			i = (Integer) rs[1]; 
        			status = 9;
        			done = true;
    			} else if (insideAName(c)) {
    				name.append(c);
    			} else {
    				throw new UnexpectedCharacterException(s, i, status);
    			}
    			break;
    		case 4:
    			if (c == C_SPACE) {
    				f.setName(name.toString());
    			} else if (c == '(') {
        			Object[] rs = Term.parseTerms(s, i);
        			f.setTerms((List<Term>) rs[0]);
        			i = (Integer) rs[1]; 
        			status = 9;
        			done = true;
    			} else {
    				throw new UnexpectedCharacterException(s, i, status);
    			}
    			break;
    		default:
    			break;
    		}
    		i++; 
    	}
    	
    	Object[] r = new Object[2];
    	r[0] = f;
    	r[1] = i-1;
    	return r;
    }

}
