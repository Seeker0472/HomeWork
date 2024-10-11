package cn.xcx.fopf;

import com.alibaba.fastjson.annotation.JSONField;

public abstract class Quantifier {
    private Term variable;
    @JSONField(serialize=false)
    public Term getVariable() {
    	return variable;
    }
    public void setVariable(Term v){
    	v.setType(Term.T_VARIABLE);
    	variable = v;
    }
    
    public Quantifier clone() {
    	Quantifier r = null;
    	
    	if (this instanceof FullQuantifier) {
    		r = new FullQuantifier();    		
    	} else {
    		r = new ExistentialQuantifier();
    	}
    	
    	r.setVariable(variable.clone());
    	
    	return r;
    }
}
