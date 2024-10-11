package cn.xcx.fopf;

import com.alibaba.fastjson.annotation.JSONField;

public class DisjunctiveFormula extends BinaryFormula {
	public DisjunctiveFormula() {
		super();
	}
	
	public DisjunctiveFormula(Formula l, Formula r) {
		super(l, r);
	}

	@JSONField(name = "name", ordinal = 1)
    public String getName() {
    	return String.valueOf(Formula.OP_DISJ);
    }
	
	public DisjunctiveFormula clone() {
		DisjunctiveFormula r = new DisjunctiveFormula(getLeftChild().clone(), getRightChild().clone());
    	
    	r.cloneQuantifiersFrom(this);
    	
    	return r;
    }
}
