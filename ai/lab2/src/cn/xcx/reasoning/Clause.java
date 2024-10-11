package cn.xcx.reasoning;

import java.util.*;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.annotation.JSONField;
import com.alibaba.fastjson.serializer.SerializerFeature;

import cn.xcx.fopf.*;
import cn.xcx.fopf.exception.*;

public class Clause {	
	private Set<Literal> literals;
    @JSONField(serialize=false)
	public Set<Literal> getLiterals() {
		return literals;
	}	
	public void setLiterals(Set<Literal> ls) {
		literals = ls;
	}
	
	public Clause() {
		literals = new HashSet<Literal>();
	}
	
	@JSONField(name = "name", ordinal = 1)
	public char getName() {
		return Formula.OP_DISJ;
	}
	public void setName() {
		
	}
	
	@JSONField(name = "children", ordinal = 2)
	public Set<Literal> getChildren() {
		return literals;
	}	
	public void setChildren(Set<Literal> ls) {
		literals = ls;
	}
	
	public String toString() {
		String r = "";
		for (Literal l: literals) {
			if (r.length() > 0) r += Formula.OP_DISJ;
			r += l.toString();
		}
		return r;
	}
	
	//生成一个空子句
	public static Clause getNIL() {
		Clause r = new Clause();
		return r;
	}
	
	@Override
	public int hashCode() {
		int r = 0;
		Iterator<Literal> i = this.getLiterals().iterator();
		while (i.hasNext()) {
			r += i.next().hashCode();
		}
		return r;
	}
	
	@Override
	public boolean equals(Object c) {
		boolean r = true;
		
		if (c != null && c instanceof Clause) {
			Set<Literal> ls = ((Clause)c).getLiterals();
			//size equals and items the same
			if (literals.size() == ls.size()) {
				Iterator<Literal> i = literals.iterator();
				while (i.hasNext()) {
					if (!ls.contains(i.next())) {
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
	
	
	// 判断子句集cs是否为一个求解问题
	public static boolean containsAnswer(Set<Clause> cs) {
		boolean r = false;
		for (Clause c: cs) {
			Set<Literal> ls = c.getLiterals(); 
			for (Literal l: ls) {
				if (l.isAnswer()) {
					r = true;
					break;
				}
			}
			if (r) break;
		}
		return r;
	}
	
	// 从子句集cs中提取求解问题的最终答案，返回结果为一个集合，其中每个子句都是一个最终答案
	public static Set<Clause> getFinalAnswers(Set<Clause> cs) {
		Set<Clause> r = new HashSet<Clause>();
		for (Clause c: cs) {
			Set<Literal> ls = c.getLiterals(); 
			if (ls.size() == 1) {
				Literal l = ls.iterator().next();
				if (l.isFinalAnswer()) {
					r.add(c);
				}
			}
		}
		return r;
	}
	
	//判断子句集cs中是否包含求解问题的最终答案，即求解问题的推理过程是否结束
	public static boolean containsFinalAnswer(Set<Clause> cs) {
		boolean r = false;
		for (Clause c: cs) {
			Set<Literal> ls = c.getLiterals(); 
			if (ls.size() == 1) {
				Literal l = ls.iterator().next();
				if (l.isFinalAnswer()) {
					r = true;
					break;
				}
			}
			if (r) break;
		}
		return r;
	}
	
	// 求两个子句集的归结式集合，即将两个子句集s1,s2中的子句两两归结，得到的归结式集合
	public static Set<Clause> resolveTwoSets(Set<Clause> s1, Set<Clause> s2) {
		Set<Clause> r = new HashSet<>();
		
		Iterator<Clause> i1, i2;		
		Clause c1, c2, c;
		i1 = s1.iterator();		
		while (i1.hasNext()) {
			c1 = i1.next();
			i2 = s2.iterator();
			while (i2.hasNext()) {
				c2 = i2.next();
				c = c1.resolve(c2);
				if (c != null) {
					r.add(c);
				}
			}
		}
		
		return r;
	}
	
	//将当前子句和子句c进行归结，返回得到的归结式
	//如果两个子句不能归结，则返回null
	//如果返回空子句，则返回的子句的属性literals长度为0
	//NonLiteral---
	public Clause resolve(Clause c) {
		for (Literal l1 : this.getLiterals()) {
			for (Literal l2 : c.getLiterals()) {
				// if complementary
				if (l1.canResolve(l2)) {
					// Get the most general unifier
					Replacement mgu = Replacement.getMGU(l1, l2);
					if (mgu != null) {
						Clause result = new Clause();

						// Add literals from the current clause, excluding l1, and apply the replacement
//						result.getLiterals().addAll(mgu.replace(this.getLiterals()));

						for (Literal lit : this.getLiterals()) {
							if (!lit.equals(l1)) {
								Set<Literal>li=(new HashSet<Literal>());
								li.add(lit);
								result.getLiterals().add(mgu.replace(li).iterator().next());
							}
						}

						// l2
//						result.getLiterals().addAll(mgu.replace(c.getLiterals()));
						for (Literal lit : c.getLiterals()) {
							if (!lit.equals(l2)) {
								Set<Literal>li=(new HashSet<Literal>());
								li.add(lit);
								result.getLiterals().add(mgu.replace(li).iterator().next());
							}
						}

						// If the new clause is empty, return it
						if (result.getLiterals().isEmpty()) {
							return result;
						}

						return result;
					}
				}
			}
		}
		return null;  // Return Null if no resolution is possible
	}


	
	
	//对子句集cs内每个子句，合并子句内部可合一的文字
	public static void mergeInternalLiterals(Set<Clause> cs) {
		for (Clause c: cs) {
			c.mergeLiterals();
		}
	}
	
	// 合并一个子句内部可合一的文字，是求归结式的3个前置处理步骤之一
	public void mergeLiterals() {
		Set<Literal> r = new HashSet<>();
		Iterator<Literal> i = literals.iterator();
		Literal l;
		Iterator<Literal> j;
		Literal l1;
		boolean merged;
		while (i.hasNext()) {
			l = i.next();
			i.remove();

			j = literals.iterator();
			merged = false;
			while (j.hasNext()) {
				l1 = j.next();
				if (l.canMerge(l1)) {
					Replacement mgu = Replacement.getMGU(l, l1);
					if (mgu != null) {
						literals = mgu.replace(literals);
						r = mgu.replace(r);
						merged = true;
						break;
					} 
				}
			}
			
			if (!merged) r.add(l);
			
			i = literals.iterator();
		}
		
		literals = r;
	}

	// 将一个公式的二叉树转换为子句集
	public static Set<Clause> fromFormulaToClauses(Formula f) {
		Formula tmp = replace2(f);
		
		tmp = pushDownNot(tmp);
		
		Map<String, Quantifier> names = new LinkedHashMap<String, Quantifier>();
		pickUpQuantifiers(tmp, names);
		tmp.setQuantifiers(new ArrayList<Quantifier>(names.values()));		
		
		//changeExistingConstantName(tmp, new ArrayList<String>(names.keySet()));
		
		replaceExistentialQuantifiers(tmp);
		
		tmp.setQuantifiers(null);   //删除全称量词
		
		tmp = putUpConjunction(tmp);
		
		Set<Clause> r = splitByConjunction(tmp);
		
		return r; 		
	}
	
	// 替换二叉树中的蕴含符和等价符，返回替换后的新二叉树
	public static Formula replace2(Formula f) {
		Formula r = f;
		
		if (r instanceof NonFormula ) {
			NonFormula nf = (NonFormula) r;
			nf.setChild(replace2(nf.getChild()));
		} else if (r instanceof BinaryFormula) {
			BinaryFormula bf = (BinaryFormula)r;
			bf.setLeftChild(replace2(bf.getLeftChild()));
			bf.setRightChild(replace2(bf.getRightChild()));
			
			if (bf instanceof EquivalentFormula) {
				EquivalentFormula ef = (EquivalentFormula)r;
				r = new DisjunctiveFormula(
						new ConjunctionFormula(
							ef.getLeftChild(), 
							ef.getRightChild()), 
						new ConjunctionFormula(
								new NonFormula(ef.getLeftChild().clone()), 
								new NonFormula(ef.getRightChild().clone())));
				r.setQuantifiers(f.getQuantifiers());
			} else if (bf instanceof ImplicationFormula) {
				ImplicationFormula imf = (ImplicationFormula)r;
				r = new DisjunctiveFormula(
						new NonFormula(imf.getLeftChild()), imf.getRightChild());
				r.setQuantifiers(f.getQuantifiers());
			}
		}
		
		return r;
	}
	
	// 谓词公式转换为子句集的第2步
	// 将二叉树中的逻辑非符号下移到叶节点，即原子公式前，返回处理后新的二叉树
	public static Formula pushDownNot(Formula f) {
		//Deploy Recursion may simplify the code
		Stack<Formula> stack = new Stack<>(); //Use stack to store the nodes to be processed
		stack.push(f); //root Node
		while(!stack.empty()){
			Formula tmp = stack.pop(); //the Node to be processed now
			if(tmp instanceof BinaryFormula) { // if tmp is a binary formula, process its children
				Formula left = ((BinaryFormula) tmp).getLeftChild();
				Formula right = ((BinaryFormula) tmp).getRightChild();
				if (right instanceof NonFormula nf) {//right child
                    if (nf.getChild() instanceof NonFormula) {//merge double noFormula
						((BinaryFormula) tmp).setRightChild(((NonFormula) nf.getChild()).getChild());
						stack.push(((NonFormula) nf.getChild()).getChild());
					}
					if (nf.getChild() instanceof ConjunctionFormula cf) {//push down the noFormula,pull up the quantifiers
                        DisjunctiveFormula df = new DisjunctiveFormula(new NonFormula(cf.getLeftChild()), new NonFormula(cf.getRightChild()));
						df.setQuantifiers(reverseQuantifier(cf.getQuantifiers()));
						((BinaryFormula) tmp).setRightChild(df);
						stack.push(df);
					}
					if (nf.getChild() instanceof DisjunctiveFormula df) {//push down the noFormula
                        ConjunctionFormula cf = new ConjunctionFormula(new NonFormula(df.getLeftChild()), new NonFormula(df.getRightChild()));
						cf.setQuantifiers(reverseQuantifier(df.getQuantifiers()));
						((BinaryFormula) tmp).setRightChild(cf);
						stack.push(cf);
					}
					if (nf.getChild() instanceof AtomicFormula af) {//
                        NonFormula nf1 = new NonFormula(af);
						nf1.setQuantifiers(reverseQuantifier(af.getQuantifiers()));
						af.setQuantifiers(null);
						((BinaryFormula) tmp).setRightChild(nf1);
					}
				}else{
					stack.push(right);
				}

				if (left instanceof NonFormula nf) {
                    if (nf.getChild() instanceof NonFormula) {
						((BinaryFormula) tmp).setLeftChild(((NonFormula) nf.getChild()).getChild());
						stack.push(((NonFormula) nf.getChild()).getChild());
					}
					if (nf.getChild() instanceof ConjunctionFormula cf) {
                        DisjunctiveFormula df = new DisjunctiveFormula(new NonFormula(cf.getLeftChild()), new NonFormula(cf.getRightChild()));
						df.setQuantifiers(reverseQuantifier(cf.getQuantifiers()));
						((BinaryFormula) tmp).setLeftChild(df);
						stack.push(df);
					}
					if (nf.getChild() instanceof DisjunctiveFormula df) {
                        ConjunctionFormula cf = new ConjunctionFormula(new NonFormula(df.getLeftChild()), new NonFormula(df.getRightChild()));
						cf.setQuantifiers(reverseQuantifier(df.getQuantifiers()));
						((BinaryFormula) tmp).setLeftChild(cf);
						stack.push(cf);
					}
					if (nf.getChild() instanceof AtomicFormula af) {
                        NonFormula nf1 = new NonFormula(af);
						nf1.setQuantifiers(reverseQuantifier(af.getQuantifiers()));
						af.setQuantifiers(null);
						((BinaryFormula) tmp).setLeftChild(nf1);
					}
				}else {
					stack.push(left);
				}

			}
			if(tmp instanceof NonFormula){
				NonFormula nf = (NonFormula) tmp;
				if(nf.getChild() instanceof NonFormula){
					((NonFormula) tmp).setChild(((NonFormula) nf.getChild()).getChild());
					stack.push(((NonFormula) nf.getChild()).getChild());
				}else if(nf.getChild() instanceof AtomicFormula){
					AtomicFormula af = (AtomicFormula) nf.getChild();
					tmp.setQuantifiers(af.getQuantifiers());
					af.setQuantifiers(null);
					((NonFormula) tmp).setChild(af);

				}
			}


		}
		return f;
	}
	private static List<Quantifier> reverseQuantifier(List<Quantifier> qs){
		if(qs==null)
			return null;

		List<Quantifier> qs1 = new ArrayList<>();
		for(Quantifier q: qs){
			if(q instanceof FullQuantifier){
				ExistentialQuantifier eq = new ExistentialQuantifier();
				eq.setVariable(q.getVariable());
				qs1.add(eq);
			}
			if(q instanceof ExistentialQuantifier){
				FullQuantifier fq = new FullQuantifier();
				fq.setVariable(q.getVariable());
				qs1.add(fq);
			}
		}
		return qs1;
	}
	
	// 提取二叉树f中所有的量词，放到哈希表names中
	//names：<量词的变元名称, 量词对象>
	private static void pickUpQuantifiers(Formula f, Map<String, Quantifier> names) {
		List<Quantifier> qs = f.getQuantifiers();
		String n, newn;
		if (qs != null) {
			for (Quantifier q: qs) {
				n = q.getVariable().getName();
				if (names.containsKey(n)) {
					newn = getNewTermName(n, names.keySet());
					q.getVariable().setName(newn);
					changeVariableName(f, n, newn);
					n = newn;
				} 
				
				names.put(n, q);
			}
			f.setQuantifiers(null);
		}
		
		if (f instanceof NonFormula) {
			NonFormula nf = (NonFormula)f;
			pickUpQuantifiers(nf.getChild(), names);
		} else if (f instanceof BinaryFormula) {
			BinaryFormula bf = (BinaryFormula)f;
			pickUpQuantifiers(bf.getLeftChild(), names);
			pickUpQuantifiers(bf.getRightChild(), names);
		}
	}
	
	// n为一个项的名称，获取一个新的项名称，保证在names中没有重复的名称
	private static String getNewTermName(String n, Collection<String> names) {
		String r = n;
		do {
			r += "_";   //加"_"以确保新名称不重复
		} while (names.contains(r));
		
		return r;
	}

	// 将二叉树f中所有名为originalName的变元名称改为newName，不包括量词
	public static void changeVariableName(Formula f, String originalName, String newName) {
		if (f instanceof AtomicFormula) {
			List<Term> ts = ((AtomicFormula)f).getTerms();
			changeVariableName(ts, originalName, newName);
		} else if (f instanceof NonFormula) {
			changeVariableName(((NonFormula)f).getChild(), originalName, newName);
		} else {			
			changeVariableName(((BinaryFormula)f).getLeftChild(), originalName, newName);
			changeVariableName(((BinaryFormula)f).getRightChild(), originalName, newName);
		}
	}
	
	// 将项列表ts中所有名为originalName的变元名称改为newName
	private static void changeVariableName(List<Term> ts, String originalName, String newName) {
		for (Term t: ts) {
			if (t.getType() == Term.T_VARIABLE) {
				if (t.getName().equals(originalName)) {
					t.setName(newName);
				}
			} else if (t.getType() == Term.T_FUNCTION) {
				changeVariableName(((Function)t).getTerms(), originalName, newName);
			} 
		}
	}

	
	
	// 替换公式f的所有存在量词
	public static void replaceExistentialQuantifiers(Formula f) {
		List<Quantifier> qs = f.getQuantifiers();
		if (qs != null) {
			Quantifier q;
			List<String> variables;
			String n;
			int i = 0;
			while (i < qs.size()) {
				q = qs.get(i);
				if (q instanceof ExistentialQuantifier) {
					n = q.getVariable().getName();
					if (i > 0) {
						variables = new ArrayList<String>();
						for (int j = 0; j < i; j++) {
							variables.add(qs.get(j).getVariable().getName());
						}
						changeVariableToFunction(f, n, 
								getNewFunctionName(n), variables);
					}
					qs.remove(i);
				} else {				
					i++;
				}
			}
		}		
	}
	
	// 获取名称为n的函数的新名称
	// n is already unique in the tree
	private static String getNewFunctionName(String n) {
		String r = "F_" + n;
		return r;
	}
	
	
	//把谓词公式f中所有名为originalName的变元改为函数newName(variables), variables是作为函数项的所有变元的名称列表
	private static void changeVariableToFunction(Formula f, 
			String originalName, String newName, List<String> variables) {
		if (f instanceof AtomicFormula) {
			List<Term> ts = ((AtomicFormula)f).getTerms();
			changeVariableToFunction(ts, originalName, newName, variables);
		} else if (f instanceof NonFormula) {
			changeVariableToFunction(((NonFormula)f).getChild(), originalName, newName, variables);
		} else {			
			changeVariableToFunction(((BinaryFormula)f).getLeftChild(), originalName, newName, variables);
			changeVariableToFunction(((BinaryFormula)f).getRightChild(), originalName, newName, variables);
		}
	}
	
	// 把项列表ts中所有名为originalName的变元改为函数newName(variables), variables是作为函数项的所有变元的名称列表
	private static void changeVariableToFunction(List<Term> ts, 
		String originalName, String newName, List<String> variables) {
		Function func;
		List<Term> vs;
		Term v;
		Term t;
		for (int i = 0; i < ts.size(); i++) {
			t = ts.get(i);
			if (t.getType() == Term.T_VARIABLE) {
				if (t.getName().equals(originalName)) {
					func = new Function();
					func.setName(newName);
					vs = new ArrayList<Term>();
					for (String n: variables) {
						v = new Term();
						v.setName(n);
						v.setType(Term.T_VARIABLE);
						vs.add(v);
					}
					func.setTerms(vs);
					
					ts.set(i, func);
				}
			} else if (t.getType() == Term.T_FUNCTION) {
				changeVariableToFunction(((Function)t).getTerms(), 
						originalName, newName, variables);
			}
		}
	}
	
	// 谓词公式转换为子句集的第六步
	// 将二叉树转换为析取式的合取式，即将合取符上移，使合取符的祖先节点中没有析取符
	// 已处理非，蕴含，等价等符号，仅需处理析取节点子树中的合取符
	public static Formula putUpConjunction(Formula expr) {
		if(expr instanceof AtomicFormula){
			return expr;
		}
		if(expr instanceof NonFormula){
			//assume all ready
			((NonFormula) expr).setChild(putUpConjunction(((NonFormula) expr).getChild()));
			return expr;
		}
		if(expr instanceof BinaryFormula bf){
			if(bf instanceof ConjunctionFormula){
				bf.setLeftChild(putUpConjunction(bf.getLeftChild()));
				bf.setRightChild(putUpConjunction(bf.getRightChild()));
				return bf;
			}
			if(bf instanceof DisjunctiveFormula){
				Formula left = putUpConjunction(bf.getLeftChild());
				Formula right = putUpConjunction(bf.getRightChild());
				return distributeOr(new DisjunctiveFormula(left, right));
			}
			//Should not reach here
			throw new RuntimeException("Should not reach here");
		}
		throw new RuntimeException("Should not reach here");
	}

	private static Formula distributeOr(DisjunctiveFormula f){
		Formula left = f.getLeftChild();
		Formula right = f.getRightChild();
		if(left instanceof ConjunctionFormula){
			Formula leftLeft = ((ConjunctionFormula) left).getLeftChild();
			Formula leftRight = ((ConjunctionFormula) left).getRightChild();
			Formula rightLeft = right.clone();
			Formula rightRight = right.clone();
			Formula l = distributeOr(new DisjunctiveFormula(leftLeft, rightLeft));
			Formula r = distributeOr(new DisjunctiveFormula(leftRight, rightRight));
			return new ConjunctionFormula(l, r);
		}
		if(right instanceof ConjunctionFormula){
			Formula rightLeft = ((ConjunctionFormula) right).getLeftChild();
			Formula rightRight = ((ConjunctionFormula) right).getRightChild();
			Formula leftLeft = left.clone();
			Formula leftRight = left.clone();
			Formula l = distributeOr(new DisjunctiveFormula(leftLeft, rightLeft));
			Formula r = distributeOr(new DisjunctiveFormula(leftRight, rightRight));
			return new ConjunctionFormula(l, r);
		}
		return new DisjunctiveFormula(left, right);
	}

	// 获取二叉树中的子句，提取所有最下层的合取符的子树，并转换为子句即可
	public static Set<Clause> splitByConjunction(Formula f) {
		Set<Clause> r = new HashSet<Clause>();
		
		if (f instanceof DisjunctiveFormula) {
			Clause c = new Clause();
			Set<Literal> ls = findLiterals(f);
			c.getLiterals().addAll(ls);
			r.add(c);
		} else if (f instanceof ConjunctionFormula) {	
			ConjunctionFormula cf = (ConjunctionFormula)f;
			Set<Clause> lr = splitByConjunction(cf.getLeftChild());
			Set<Clause> rr = splitByConjunction(cf.getRightChild());
			r.addAll(lr);
			r.addAll(rr);
		} else {
			Clause c = new Clause();
			c.getLiterals().add(new Literal(f));
			r.add(c);
		}
		
		return r;
	}
	
	// 提取子树f中的所有文字，用于构建子句
	private static Set<Literal> findLiterals(Formula f) {
		Set<Literal> r = new HashSet<Literal>();		
		
		if (f instanceof DisjunctiveFormula) {
			DisjunctiveFormula df = (DisjunctiveFormula)f;
			Set<Literal> lr = findLiterals(df.getLeftChild());
			Set<Literal> rr = findLiterals(df.getRightChild());
			r.addAll(lr);
			r.addAll(rr);
		} else {
			r.add(new Literal(f));
		}
		
		return r;
	}
	
	// 给每个变元名称前加上字符串prefix
	protected void addPrefixToVariables(String prefix) {
		for (Literal l: getLiterals()) {
			for (Term t: l.getAtom().getTerms()) {
				t.addPrefixToVariables(prefix);
			}
		}
	}
	
	public static void main(String[] args) throws ParseException {	
		
		String s  = "(∀x)(¬pq(aa, b0, f(x, dd1), ee6666) ∨ (∃y)(p(a) → q(b) ∧ r(y,d)) ↔ s(z))";
		//String s = "(∀z)¬(∀x)((∃y)p(x, y, z)∨(∀u)q(x, y, z, u))";
		Formula f = Formula.parse(s);
//		Set<Clause> r = fromFormulaToClauses(f);
//		System.out.println(JSON.toJSONString(r));
//		
		f = replace2(f);
		//System.out.println(JSON.toJSONString(f));
		
		f = pushDownNot(f);
		//System.out.println(JSON.toJSONString(f));
		
		Map<String, Quantifier> names = new LinkedHashMap<String, Quantifier>();
		pickUpQuantifiers(f, names);
		
		f.setQuantifiers(new ArrayList<Quantifier>(names.values()));
		//System.out.println(JSON.toJSONString(f));
		
		replaceExistentialQuantifiers(f);
		f.setQuantifiers(null);   //删除全称量词
		//System.out.println(JSON.toJSONString(f));
		
		f = putUpConjunction(f);
		//System.out.println(JSON.toJSONString(f));
		
		Set<Clause> r = splitByConjunction(f);		
		String json = JSON.toJSONString(r, SerializerFeature.WriteClassName);		
		System.out.println(json);
		Set<Clause> c = (Set<Clause>) JSON.parse(json);  // 反序列化
		
		System.out.println(JSON.toJSONString(c)); 
		
//		String s1 = "(? x1 )(~pq (aa , b0, f ( ce9, dd1)) | ((*x)(?y) (((p( a) & t(c, g(k)))&(~q(b ) | ~r (c,d))|~s(z))&(((~p( a)|~t(c, g(k))) | q(b ) &r (c,d) ) | s(z))) | t(x1, x)) )";		
//		Formula f1 = Formula.parse(s1);
//		String o1 = JSON.toJSONString(f1);
//		System.out.println(o1);
		
//		assertEquals(o.equals(o1), true, "error in pushDownNOt().");
		
		
	}
}
