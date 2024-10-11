package cn.xcx.fopf;

import com.alibaba.fastjson.annotation.JSONField;

public class EquivalentFormula extends BinaryFormula {
	public EquivalentFormula() {
		super();
	}
	public EquivalentFormula(Formula l, Formula r) {
		super(l, r);
	}

	@JSONField(name = "name", ordinal = 1)
    public String getName() {
    	return String.valueOf(Formula.OP_EQ);
    }
	
	public EquivalentFormula clone() {
		EquivalentFormula r = new EquivalentFormula(getLeftChild().clone(), getRightChild().clone());
    	
    	r.cloneQuantifiersFrom(this);
    	
    	return r;
    }
}
