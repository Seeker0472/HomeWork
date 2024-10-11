package cn.xcx.reasoning;

import cn.xcx.fopf.AtomicFormula;
import cn.xcx.fopf.Formula;
import cn.xcx.fopf.NonFormula;
import cn.xcx.fopf.Term;
import cn.xcx.fopf.exception.ParseException;

import java.util.List;

import com.alibaba.fastjson.annotation.JSONField;

public class Literal {
    @JSONField(serialize=false)
	public static final String ANSWER_NAME = "Answer";
    
	private boolean nonLiteral;
    
	private AtomicFormula atom;
	
	private String name;
	@JSONField(name = "name")
	public String getName() {
		return this.toString();
	}
	public void setName(String r) throws ParseException {
		String s = r;
		if (r.charAt(0) == Formula.OP_NOT) {
			this.nonLiteral = true;
			s = r.substring(1);
		}
		atom = (AtomicFormula) Formula.parse(s);
	}

	
	public Literal() {
		
	}
	
	
	public String toString() {
		String r = "";
		if (nonLiteral) {
			r += Formula.OP_NOT;
		} 
		
		r += atom.getName();
		r += "(";
		List<Term> ts = atom.getTerms();		
		for (Term t: ts) {
			r += t.toString();
			r += ", ";
		}
		r = r.substring(0, r.length()-2);
		r += ")";
		
		return r;
	}
	
	public Literal(Formula f) {
		//assertEquals(f instanceof NonFormula || f instanceof AtomicFormula, true, "Unexpected formula for literals.");
		if (f instanceof AtomicFormula) {
			nonLiteral = false;
			atom = (AtomicFormula) f;
		} else {
			nonLiteral = true;
			//assertEquals(((NonFormula)f).getChild() instanceof AtomicFormula, true, "Unexpected formula for literals.");
			atom = (AtomicFormula) ((NonFormula)f).getChild();
		} 
	}

	// 当前文字是否包含逻辑非
    @JSONField(serialize=false)
	public boolean isNonLiteral() {
		return nonLiteral;
	}

    @JSONField(serialize=false)
	public AtomicFormula getAtom() {
		return atom;
	}
	
	@Override
	public boolean equals(Object l) {
		return (l != null && l instanceof Literal && 
				nonLiteral == ((Literal)l).isNonLiteral()) && 
				atom.equals(((Literal)l).getAtom());
	}
	
	@Override
	public int hashCode() {
		return this.toString().hashCode();
	}
	
	// 当前文字是否为Answer,且不包含逻辑非
    @JSONField(serialize=false)
	public boolean isAnswer() {		
		return !nonLiteral && atom.getName().equals(ANSWER_NAME);
	}
	
	// 当前文字是否为最终答案，即为Answer,且不包含逻辑非，且所有项都为常量
    @JSONField(serialize=false)
	public boolean isFinalAnswer() {
		boolean r = true;
		
		if (!nonLiteral && atom.getName().equals(ANSWER_NAME)) {
			for (Term t: atom.getTerms()) {
				if (t.getType() != Term.T_CONSTANT) {
					r = false;
					break;
				}
			}
		} else {
			r = false;
		}
		
		return r;
	}
	
	// 判断当前文字是否可能与文字to合一，即两个文字是否包含逻辑非的情况一致，且原子谓词名称相同，项的数量一致
	public boolean canMerge(Literal to) {
		return nonLiteral == to.isNonLiteral() && 
				getAtom().getName().equals(to.getAtom().getName()) &&
				getAtom().getTerms().size() == to.getAtom().getTerms().size();
	}

	// 判断当前文字是否可能与文字to进行归结，即两个文字是否包含逻辑非的情况相反，且原子谓词名称相同，项的数量一致
	public boolean canResolve(Literal to) {
		return nonLiteral != to.isNonLiteral() && 
				getAtom().getName().equals(to.getAtom().getName()) &&
				getAtom().getTerms().size() == to.getAtom().getTerms().size();
	}
	
	public Literal clone() {
		Formula f;
		if (nonLiteral) f = new NonFormula(atom.clone());
		else f = atom.clone();
		Literal r = new Literal(f);
		return r;
	}
}
