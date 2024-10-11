package cn.xcx.fopf;

import com.alibaba.fastjson.annotation.JSONField;

public class ImplicationFormula extends BinaryFormula {
	public ImplicationFormula() {
		super();
	}
	public ImplicationFormula(Formula l, Formula r) {
		super(l, r);
	}

	@JSONField(name = "name", ordinal = 1)
    public String getName() {
    	return String.valueOf(Formula.OP_IMPL);
    }
	
	public ImplicationFormula clone() {
		ImplicationFormula r = new ImplicationFormula(getLeftChild().clone(), getRightChild().clone());
    	
    	r.cloneQuantifiersFrom(this);
    	
    	return r;
    }
}
