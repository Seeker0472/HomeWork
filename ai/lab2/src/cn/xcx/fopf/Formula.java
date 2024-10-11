package cn.xcx.fopf;

import java.util.ArrayList;
import java.util.List;
import java.util.Stack;

import com.alibaba.fastjson.annotation.JSONField;

import cn.xcx.fopf.exception.ConflictingQuantifierException;
import cn.xcx.fopf.exception.ParseException;
import cn.xcx.fopf.exception.UnexpectedCharacterException;
import cn.xcx.fopf.exception.UnmatchedLeftBracketException;
import cn.xcx.fopf.exception.UnmatchedRightBracketException;

public abstract class Formula {
	public static final char OP_NOT = '¬'; 
	public static final char OP_CONJ = '∧';   
	public static final char OP_DISJ = '∨'; 
	public static final char OP_IMPL = '→';
	public static final char OP_EQ = '↔';	
	public static final char L_BRACKET = '(';	
	public static final char R_BRACKET = ')';
	//Bro you have a Poor English(^-^) Universal quantification Not Full quantification
	public static final char Q_FULL = '∀';		
	public static final char Q_EXISTENTIAL = '∃';		
	public static final char C_SPACE = ' ';			
	public static final char C_COMMA = ',';	

	public static final int P_NOT = 1;
	public static final int P_CONJ = 2;
	public static final int P_DISJ = 3;
	public static final int P_IMPL = 4;
	public static final int P_EQ = 5;
	public static final int P_DEFAULT = 6;
	
	static final char Q_LABEL = 'q';		//量词占位符
	
    private List<Quantifier> quantifiers;
    @JSONField(serialize=false)
    public List<Quantifier> getQuantifiers() {
    	return quantifiers;
    }
    public void setQuantifiers(List<Quantifier> qs) {
    	quantifiers = qs;
    }

    public void addQuantifier(Quantifier q) throws ConflictingQuantifierException { 
    	if (findQuantifier(q.getVariable()) == null) {
    		if (quantifiers == null) quantifiers = new ArrayList<Quantifier>();
    		quantifiers.add(0, q);
    		setVariable(q.getVariable());
    	} else {
    		throw new ConflictingQuantifierException(q.getVariable().getName());
    	}
    }

    public void appendQuantifier(Quantifier q) { 
		if (quantifiers == null) quantifiers = new ArrayList<Quantifier>();
		quantifiers.add(q);
    }
    
    public static Formula parse(String s) throws ParseException {
    	Formula f = null;
    	
    	Stack<Character> ops = new Stack<Character>();
    	Stack<Formula> fs = new Stack<Formula>();
    	Stack<Quantifier> qs = new Stack<Quantifier>();
    	//The FUCKING State Machine didn't even have a comment!!!!
		/*
		* The Following is just my guess
		* ０-Initial Status
		* １- Space And OP_NOT
		* 	    - If Left Bracket goto 2
		* 		- If Predicate ,goto 9
		*
		* 2 - Char after left bracket
		* 		- If Predicate ,goto 9
		* 		- If OP_NOT ,goto 1
		* 		- If Q_FULL/Q_EXISTENTIAL ,goto 11
		*
		* 9  - Char after Predicate
		* 10 - Space and Right Bracket
		* 		- If BinaryOp --------------- goto 1
		* 11 - After Quantification (Universal or Existential)
		* 		- if hit variable ，goto,12
		* 12 - Deals With variable Name Lager than 2
		* 13 - Str After variable
		*
		* */
    	int status = 0;
    	char c;
    	int i = 0;
    	Quantifier q = null;
    	StringBuffer name = null;
    	Term v = null;
    	while (!endOfString(s, i)) {       // 结束条件
    		c = s.charAt(i);
    		switch (status) {
    		case 0, 1:
    			if (c == C_SPACE) {
    				status = 1;
    			} else if (c == OP_NOT) {
    				status = 1;
    				ops.push(c);
    			} else if (c == L_BRACKET) {
    				status = 2;
    				ops.push(c);
    			} else if (startsAName(c)) { // 原子谓词的开头    				
    				Object[] tmp = AtomicFormula.parse(s, i); //解析原子谓词，返回一个原子谓词，和原子谓词结束的字符序号
    				fs.push((Formula) tmp[0]);
    				i = (Integer)tmp[1];
    				status = 9;
    			} else {
    				throw new UnexpectedCharacterException(s, i, status);
    			}
    			break;
    		case 2:
    			if (c == C_SPACE) {
    				status = 2;
    			} else if (c == OP_NOT) {
    				ops.push(c);
    				status = 1;
    			} else if (c == L_BRACKET) {
    				ops.push(c);
    				status = 2;
    			} else if (startsAName(c)) { // 同status == 0
    				Object[] tmp = AtomicFormula.parse(s, i);
    				fs.push((Formula) tmp[0]);
    				i = (Integer)tmp[1];
    				status = 9;
    			} else if (c == Q_FULL) {
    				q = new FullQuantifier();
    				ops.pop();
    				status = 11;
    			} else if (c == Q_EXISTENTIAL) {
    				q = new ExistentialQuantifier();
    				ops.pop();
    				status = 11;
    			} else {
    				throw new UnexpectedCharacterException(s, i, status);
    			}
    			break;
    		case 9, 10:
    			if (c == C_SPACE) {
    				status = 10;
    			} else if (c == R_BRACKET) {     // 中序转后序，弹出至相应左括号
    				if (!popToLeftBracket(ops, fs, qs)) throw new UnmatchedRightBracketException(s, i, status);
    				status = 10;
    			} else if (isBinaryOperator(c)) {    				
    				popUntilPush(ops, fs, qs, c);   // 中序转后序，弹出至出现优先级更高或相同的连接符
    				status = 1;    				
    			} else {
    				throw new UnexpectedCharacterException(s, i, status);
    			}
    			break;
    		case 11:
    			if (c == C_SPACE) {
    				status = 11;
    			} else if (startsAName(c)) {
    		    	name = new StringBuffer();
    				name.append(c);
    				status = 12;
    			} else {
    				throw new UnexpectedCharacterException(s, i, status);
    			}
    			break;
    		case 12:
    			if (c == C_SPACE) {
    				v = new Term();
    				v.setName(name.toString());
					status = 13;
				} else if (c == R_BRACKET) {
    				v = new Term();
    				v.setName(name.toString());
    				q.setVariable(v);
    				qs.push(q);
    				ops.push(Q_LABEL);
    				status = 1;
    			} else if (insideAName(c)) {
    				name.append(c);
    				status = 12;
    			} else {
    				throw new UnexpectedCharacterException(s, i, status);
    			}
    			break;
    		case 13:
    			if (c == C_SPACE) {
					status = 13;
				} else if (c == R_BRACKET) {
    				q.setVariable(v);
    				qs.push(q);
    				ops.push(Q_LABEL);
    				status = 1;
    			} else {
    				throw new UnexpectedCharacterException(s, i, status);
    			}
    			break;
    		default:
    			break;
    		}
    		
    		i++;
    	}
    	
    	popAll(ops, fs, qs);
    	f = fs.pop();

    	return f;
    }

    public static boolean isPrior(char op1, char op2){
        return getPriority(op1) < getPriority(op2);
    }

    public Quantifier findQuantifier(Term v){
    	Quantifier q = null;
    	
    	if (quantifiers != null) {    	
	    	for (Quantifier i: quantifiers) {
	    		if (i.getVariable().equals(v)) {
	    			q = i;
	    			break;
	    		}
	    	}
    	}
    	
    	if (q == null) {
    		if (this instanceof NonFormula) {
    			q = ((NonFormula)this).getChild().findQuantifier(v);
    		} else if (this instanceof BinaryFormula) {
    			q = ((BinaryFormula)this).getLeftChild().findQuantifier(v);
    			if (q == null) q = ((BinaryFormula)this).getRightChild().findQuantifier(v);
    		}
    	}
    	
        return q;
    }


	private static int getPriority(char op) {
		int priority = P_DEFAULT;
		switch (op) {
			case OP_NOT, Q_LABEL:
				priority = P_NOT;
				break;
			case OP_CONJ:
				priority = P_CONJ;
				break;
			case OP_DISJ:
				priority = P_DISJ;
				break;
			case OP_IMPL:
				priority = P_IMPL;
				break;
			case OP_EQ:
				priority = P_EQ;
				break;
		}
		return priority;
	}	

	static boolean startsAName(char c) {
		return c>='a' && c<='z' || c>='A' && c<='Z';
	}
	
	static boolean insideAName(char c) {
		return c>='a' && c<='z' || c>='A' && c<='Z' || c>='0' && c<='9' || c == '_';
	}
	
	// 弹出ops所有余下的运算符（如果公式的语法正确，调用时ops中应该已经没有括号了），同时弹出qs中的量词和fs中所有子公式，组合成最终的公式，并压入fs中（如果公式的语法正确，返回时ops和qs应为空，fs中应该只有一个公式）
	private static void popAll(Stack<Character> ops, Stack<Formula> fs, Stack<Quantifier> qs) throws UnmatchedRightBracketException, ConflictingQuantifierException, UnmatchedLeftBracketException {
		char op;
		while (!ops.empty()) {
			op = ops.pop();
	    	//assertEquals(op == R_BRACKET, false, "Unexpected '" + op + "' when pop out all operators.");
	    	if (op == L_BRACKET) throw new UnmatchedLeftBracketException();
			pushFormula(ops, fs, qs, op);
		}
    	//assertEquals(qs.empty(), true, "Unexpected quantifiers after popping out all operators.");
    	//assertEquals(fs.size(), 1, "Unexpected formulus afeter popping out all operators.");
	}
	
	//把运算符或量词与fs中的子公式组合新的公式，压入fs
	private static void pushFormula(Stack<Character> ops, Stack<Formula> fs, Stack<Quantifier> qs, char op) throws ConflictingQuantifierException {
		Formula root = null;		
		Formula left, right;
		if (op == OP_NOT) {
			root = new NonFormula(fs.pop());
			fs.push(root);
		} else if (op == Q_LABEL) {
			root = fs.pop();
			Quantifier q = qs.pop();
			root.addQuantifier(q);
			fs.push(root);
		} else {
			right = fs.pop();
			left = fs.pop();
			root = getInstance(op, left, right);
			fs.push(root);
		} 
	}
	
	//按中序转后序算法，查看并处理ops顶端的算符top，直到top为左边括号，或op的优先级高于top，然后将op压入ops
	private static void popUntilPush(Stack<Character> ops, Stack<Formula> fs, Stack<Quantifier> qs, char op) throws ConflictingQuantifierException {
		//assertEquals(op == OP_NOT || op == Q_LABEL, false, "Unexpected '" + op + "' in popUntilPush().");
		char top;
		while (!ops.empty()) {
			top = ops.peek();
	    	//assertEquals(top == R_BRACKET, false, "Unexpected ')' in popUntilPush().");
			if (top == L_BRACKET || isPrior(op, top)) {
				break;
			} else {
				top = ops.pop();
				pushFormula(ops, fs, qs, top);
			}
		}
		ops.push(op);
	}
	
	//当遇到右边括号，处理ops顶端的算符top，直到top为左边括号，成功返回true，失败返回false
	private static boolean popToLeftBracket(Stack<Character> ops, Stack<Formula> fs, Stack<Quantifier> qs) throws UnmatchedRightBracketException, ConflictingQuantifierException {
		char top;	
		boolean done = false;
		while (!ops.empty()) {
			top = ops.pop();
			if (top == L_BRACKET) {
				done = true;
				break;
			} else {
				pushFormula(ops, fs, qs, top);
			}
		} 
		return done;
	}
	
	private static boolean isBinaryOperator(char c) {
		boolean r = c == OP_CONJ || c == OP_DISJ || c == OP_IMPL || c == OP_EQ;
		return r;
	}
	
	private static boolean endOfString(String s, int i) {
		boolean r = i == s.length() || s.charAt(i) == '\r' || s.charAt(i) == '\n';
		return r;
	}
	
	private static BinaryFormula getInstance(char fn, Formula l, Formula r) {
		BinaryFormula b = null;
		switch (fn) {
		case OP_CONJ:
			b = new ConjunctionFormula(l, r);
			break;
		case OP_DISJ:
			b = new DisjunctiveFormula(l, r);
			break;
		case OP_IMPL:
			b = new ImplicationFormula(l, r);
			break;
		case OP_EQ:
			b = new EquivalentFormula(l, r);
			break;
		}		
		return b;
	}
	
	public void insertQuantifiers(List<Quantifier> qs) {
		if (qs != null) {
			if (quantifiers == null) quantifiers = new ArrayList<Quantifier>();
			Quantifier q;
			for (int i = 0; i < qs.size(); i++) {
				q = qs.get(i);
				quantifiers.add(i,q);
			}
		}
	}
	
	//设置公式中所有同名的项为变元，调用时，公式的项列表应该已生成
	private void setVariable(Term v) {
		if (this instanceof AtomicFormula) {
			((AtomicFormula)this).setVariable(v);
		} else if (this instanceof NonFormula) {
			((NonFormula)this).getChild().setVariable(v);
		} else {
			//assertEquals(this instanceof BinaryFormula, true, "Unexpected formula type in setVariable().");			
			((BinaryFormula)this).getLeftChild().setVariable(v);
			((BinaryFormula)this).getRightChild().setVariable(v);
		}
	}

	
	public abstract Formula clone();
	
	protected void cloneQuantifiersFrom(Formula r) {
		List<Quantifier> qs = r.getQuantifiers();
		if (qs != null) {
			quantifiers = new ArrayList<Quantifier>();
			for (Quantifier q: qs) {
				quantifiers.add(q.clone());
			}
		}
	}
}
