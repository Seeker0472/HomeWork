package cn.xcx.fopf;

import java.util.ArrayList;
import java.util.List;

import com.alibaba.fastjson.annotation.JSONField;

import cn.xcx.fopf.exception.UnexpectedCharacterException;

public class Term {
	public static final int T_CONSTANT = 0;
	public static final int T_VARIABLE = 1;
	public static final int T_FUNCTION = 2;
		
    private String name;
    private int type;
    
    public Term() {
    	type = T_CONSTANT;
    }    

    
    @JSONField(name = "name", ordinal = 1)
    public String getName()  {
    	return name;
    }
    public void setName(String s){
    	name = s;
    }
 
    @JSONField(serialize=false)
    public int getType()  {
    	return type;
    }
    public void setType(int t){
    	type = t;
    }  
    
    public String toString() {
    	String r = name;
    	
    	if (type == T_FUNCTION) {    		
    		r += "(";
    		List<Term> ts = ((Function)this).getTerms();		
    		for (Term t: ts) {
    			r += t.toString();
    			r += ", ";
    		}
    		r = r.substring(0, r.length()-2);
    		r += ")";
    	}
    	return r;
    }

    public boolean equals(Term v) {
    	boolean r = false;
    	if (this.getType() == v.getType()) {
	    	if (this.getType() == T_FUNCTION) {
	    		r = ((Function) this).equals(v);
	    	} else {
	    		r = getName().equals(v.getName());
	    	}
    	} 
    	return r;
    }
    
    //设置项（包括函数）中同名的项为变元
    void setVariable(Term v) {
		if (this.getType() == T_CONSTANT) {
			if (this.getName().equals(v.getName())) {
				this.setType(T_VARIABLE);
			}
		} else if (this.getType() == T_FUNCTION) {
			((Function)this).setVariable(v);
		} 
    }
    
    // 给变元名称加上字符串prefix，从而保证不同子句中变元的名称都不一样，
    public void addPrefixToVariables(String prefix) {
    	if (this.getType() == T_VARIABLE) {
    		this.setName(prefix + this.getName());
		} else if (this.getType() == T_FUNCTION) {
			((Function)this).addPrefixToVariables(prefix);
		} 
    }
    
    public Term clone() {
    	Term r = null;
    	
    	if (type == T_CONSTANT || type == T_VARIABLE) {
    		r = new Term();
	    	r.setName(name);
	    	r.setType(type);
    	} else {
    		r = ((Function)this).clone();    		
    	}
    	
    	return r;     	
    }
    
    
    // 对字符串s,从第from个字符开始，解析原子谓词或函数的项列表，返回解析结果和对应的字符串长度
    static Object[] parseTerms(String s, int from) throws UnexpectedCharacterException {
    	//assertEquals(s.charAt(from), Formula.L_BRACKET, "Unexpected character at (" + s.substring(from) + "). '(' is expected");
    	List<Term> ts = new ArrayList<Term>();

    	StringBuffer name = null;
    	Term t = null;
    	int status = 5;   	
    	char c;
    	int i = from+1;
    	boolean done = false;
    	while (i < s.length() && !done) {  // 是否还有上层项列表，用于处理项是函数的情况
    		c = s.charAt(i);
    		switch (status) {
    		case 5, 6:
    			if (c == Formula.C_SPACE) {
    				status = 6;
    			} else if (Formula.startsAName(c)) {
    				t = new Term();
    		    	name = new StringBuffer();
    				name.append(c);
        			status = 7;
    			} else {
    				throw new UnexpectedCharacterException(s, i, status);
    			}
    			break;
//    		case 6:
//    			if (c == Formula.C_SPACE) {
//    				status = 6;
//    			} else if (Formula.startsAName(c)) {
//    				t = new Term();
//    		    	name = new StringBuffer();
//    				name.append(c);
//        			status = 7;
//    			} else {
//    				throw new UnexpectedCharacterException(s, i, status);
//    			}
//    			break;
    		case 7:
    			if (c == Formula.C_SPACE) {
    				t.setName(name.toString());
    				status = 8;
    			} else if (c == Formula.L_BRACKET) {  //begin a function terms
        			Object[] rs = Term.parseTerms(s, i);
        			t = new Function();
        			t.setName(name.toString());
        			((Function)t).setTerms((List<Term>) rs[0]);
        			ts.add(t);
        			i = (Integer) rs[1]; 
        			status = 9;    // done是否为True需要由后续字符决定，见case 9
    			} else if (Formula.insideAName(c)) {
    				name.append(c);
    			} else if (c == Formula.C_COMMA) {
    				t.setName(name.toString());
    				ts.add(t);
    				status = 6;
    			} else if (c == Formula.R_BRACKET) {
    				t.setName(name.toString());
    				ts.add(t);
    				status = 9;
    				done = true;
    			} else {
    				throw new UnexpectedCharacterException(s, i, status);
    			}
    			break;
    		case 8:
    			if (c == Formula.C_SPACE) {
    				status = 8;
    			}  else if (c == Formula.C_COMMA) {
    				t.setName(name.toString());
    				ts.add(t);
    				status = 6;
    			} else if (c == Formula.L_BRACKET) {  //begin a function terms
        			Object[] rs = Term.parseTerms(s, i);
        			t = new Function();
        			t.setName(name.toString());
        			((Function)t).setTerms((List<Term>) rs[0]);
        			ts.add(t);        			
        			i = (Integer) rs[1]; 
        			status = 9; 
    			} else if (c == Formula.R_BRACKET) {
    				ts.add(t);
    				status = 9;
    				done = true;
    			} else {
    				throw new UnexpectedCharacterException(s, i, status);
    			}
    			break;
    		case 9:     // 还有上层项列表未结束
    			//assertEquals(done, false, "Unexpected status at (" + s.substring(from) + "). Terms should not be over.");
    			if (c == Formula.C_SPACE) {
    				status = 9;
    			}  else if (c == Formula.C_COMMA) {
    				status = 6;
    			} else if (c == Formula.R_BRACKET) { 
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
    	r[0] = ts;
    	r[1] = i-1;
    	return r;
    }
    

}
