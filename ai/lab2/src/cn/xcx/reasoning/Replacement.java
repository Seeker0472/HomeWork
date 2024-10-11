package cn.xcx.reasoning;

import java.util.*;

import cn.xcx.fopf.Function;
import cn.xcx.fopf.Term;

public class Replacement {
	LinkedHashMap<Term, Term> fromTo;
	
	public Replacement() {
		fromTo = new LinkedHashMap<Term, Term>();
	}
	
	public int size() {
		return fromTo.size();
	}
	
	// 将当前对象表示的置换复合到置换origin, 即origin=origin*this
	protected void combineTo(Replacement origin) {
		Term from, to;
		Iterator<Term> it = fromTo.keySet().iterator();
		while (it.hasNext()) {
			from = it.next();
			to = fromTo.get(from);
			origin.combine(from, to);
		}
	}
	
	// 将置换{to/from}和当前置换复合，即this=this*{to/from}
	public void combine(Term from, Term to) {
		// Iterate over the current replacements, replacing all instances of 'from' with 'to'
		LinkedHashMap<Term, Term> newFromTo = new LinkedHashMap<>();
		for (Map.Entry<Term, Term> entry : fromTo.entrySet()) {
			Term key = entry.getKey();
			Term value = entry.getValue();

			// if contains 'from', replace it with 'to'
			if (value.equals(from)) {
				newFromTo.put(key, to);
			} else {
				newFromTo.put(key, value);
			}
		}

		// Add the 'from' -> 'to' mapping to the replacements
		newFromTo.put(from, to);

		// Update
		fromTo = newFromTo;
	}

	
	// 将项t用置换{to/from}置换，返回置换后新生成的项
	private static Term toSpecialCase(Term t, Term from, Term to) {
		Term r = t;
		
		switch(t.getType()) {
		case Term.T_VARIABLE:
			if (t.getName().equals(from.getName())) {
				r = to.clone();
			}
			break;
		case Term.T_FUNCTION:
			r = new Function();
			r.setName(t.getName());
			List<Term> lt = new ArrayList<>();
			for (Term t1: ((Function)t).getTerms()) {
				lt.add(toSpecialCase(t1, from, to));
			}
			((Function)r).setTerms(lt);
			break;
		default:
			break;
		}
		
		return r;
	}
	
	//将文字l用置换{to/from}置换，返回置换后新生成的文字
	private static void toSpecialCase(Literal l, Term from, Term to) {
		List<Term> lt = new ArrayList<>();
		for (Term t: l.getAtom().getTerms()) {
			lt.add(toSpecialCase(t, from, to));
		}
		l.getAtom().setTerms(lt);
	}
	
	/*
	 * private static void toSpecialCase(Clause c, Term from, Term to) { for
	 * (Literal l: c.getLiterals()) { toSpecialCase(l, from, to); } }
	 */
	//将文字集合sl中的所有文字用置换{to/from}置换，返回置换后新生成的文字的集合
	public Set<Literal> replace(Set<Literal> sl) {
		Set<Literal> r = new HashSet<>();
		Iterator<Term> i;
		Term k;
		Literal n;
		for (Literal l: sl) {
			n = l.clone();
			i = fromTo.keySet().iterator();
			while (i.hasNext()) {
				k = i.next();
				toSpecialCase(n, k, fromTo.get(k));
			}
			r.add(n);
		}
		
		return r;
	}
	
	// 用当前置换置换项t,返回置换生成的新的项
	private Term replace(Term t) {
		Term r = t.clone();
		Iterator<Term> i = fromTo.keySet().iterator();
		Term k;
		while (i.hasNext()) {
			k = i.next();
			r = toSpecialCase(r, k, fromTo.get(k));
		}
		
		return r;
	}
	
	// 求两个文字的mgu，如果两个文字一致，返回空置换，如果两个文字不能合一，返回null
	public static Replacement getMGU(Literal l1, Literal l2) {
		Replacement r = null;

		r =	new Replacement();
		List<Term> ts1 = l1.getAtom().getTerms();
		List<Term> ts2 = l2.getAtom().getTerms();
		Replacement tmp;
		for (int i = 0; i < ts1.size(); i++) {
			tmp = getReplacement(r.replace(ts1.get(i)), r.replace(ts2.get(i)));
			if (tmp == null) {
				r = null;
				break;
			} else {
				tmp.combineTo(r);
			}
		}

		return r;
	}
	
	// 求将两个项合一的置换，如果两个项一致，返回空置换，如果两个项不能合一，返回null
	public static Replacement getReplacement(Term t1, Term t2) {
		Replacement r = null;

		// if euqals
		if (t1.equals(t2)) {
			return new Replacement();
		}

		// if variables
		if (t1.getType()==Term.T_VARIABLE) {
//			if (t2.getName().equals(t1.getName())) {
//				return new Replacement();
//			}
			r=(new Replacement());
			r.fromTo.put(t1, t2);
			return r;
		}

		// if variables
		if (t2.getType()==Term.T_VARIABLE) {
//			if (t1.getName().equals(t2.getName())) {
//				return new Replacement();
//			}
			r=(new Replacement());
			r.fromTo.put(t2, t1);
			return r;
		}

		// if func
		if (t1.getType()==Term.T_FUNCTION && t2.getType()==Term.T_FUNCTION) {
			r=new Replacement();
			if (!t1.getName().equals(t2.getName()) ||
					((Function)t1).getTerms().size() != ((Function)t2).getTerms().size()) {
				return null; // Name or terms size not equal
			}

			for (int i = 0; i < ((Function)t1).getTerms().size(); i++) {
				Replacement tmp = getReplacement(((Function)t1).getTerms().get(i), ((Function)t2).getTerms().get(i));
				if (tmp == null) {
					return null; // terms cannot be unified
				}
				tmp.combineTo(r);
//				r.combineTo(tmp);
			}
			return r;
		}

		// Otherwise
		return null;
	}
	
	// 判断两个项是否为可能合一的函数，即是否都为函数，且名称相同，项的数量相同
	private static boolean isResolvableFunctions(Term f1, Term f2) {
		if (f1 instanceof Function && f2 instanceof Function) {
			return f1.getName().equals(f2.getName()) && 
					((Function)f1).getTerms().size() == ((Function)f2).getTerms().size(); 
		} else {
			return false;
		}
	}
	
	public String toString() {
		String r = "{";
		Iterator<Term> i = fromTo.keySet().iterator();			
		Term k, v;
		while (i.hasNext()) {
			k = (Term) i.next();
			v = fromTo.get(k);
			if (r.length() > 1) {
				r += ", ";
			}
			r += v.toString() + "/" + k.toString();
		}
		
		return r+"}";
	}
}
