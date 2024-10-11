package cn.xcx.fopf;

import com.alibaba.fastjson.annotation.JSONField;

public class ConjunctionFormula extends BinaryFormula {
	public ConjunctionFormula() {
		super();
	}
	public ConjunctionFormula(Formula l, Formula r) {
		super(l, r);
		// TODO Auto-generated constructor stub
	}

	@JSONField(name = "name", ordinal = 1)
    public String getName() {
    	return String.valueOf(Formula.OP_CONJ);
    }
	
	public ConjunctionFormula clone() {
		ConjunctionFormula r = new ConjunctionFormula(getLeftChild().clone(), getRightChild().clone());
    	
    	r.cloneQuantifiersFrom(this);
    	
    	return r;
    }
}
