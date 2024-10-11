package cn.xcx.fopf;

import com.alibaba.fastjson.annotation.JSONField;

public class FullQuantifier extends Quantifier {
	@JSONField(name = "name", ordinal = 1)
    public String getName() {
    	return String.valueOf(Formula.Q_FULL) + getVariable().getName();
    }
	public void setName(String n) {
		Term v = new Term();
		v.setName(n.substring(1, n.length()));
		setVariable(v);
	}
}
