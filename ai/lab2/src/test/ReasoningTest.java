package test;

import static org.junit.jupiter.api.Assertions.*;

import java.io.IOException;
import java.util.Iterator;
import java.util.Set;

import org.junit.jupiter.api.Test;

import com.alibaba.fastjson.JSON;

import cn.xcx.fopf.Formula;
import cn.xcx.fopf.exception.ParseException;
import cn.xcx.reasoning.Clause;
import cn.xcx.reasoning.Literal;
import cn.xcx.reasoning.Reasoner;
import cn.xcx.reasoning.Replacement;

class ReasoningTest {

	@Test
	void test_reason_0() throws ParseException, IOException {
		Reasoner reasoner = new Reasoner("problems/whois.txt");				
		String output = reasoner.reason();
		assertEquals(output, "Found 2 answers: Answer(q) Answer(s) ");
	}

	@Test
	void test_reason_1() throws ParseException, IOException {
		Reasoner reasoner = new Reasoner("problems/right.txt");				
		String output = reasoner.reason();
		assertEquals(output, "Proved!");
	}

	@Test
	void test_resolve_0() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)P(x)∧¬P(a)");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();
		Clause output = i.next().resolve(i.next());			
		assertEquals(output.toString().equals(""), true);
	}

	@Test
	void test_resolve_1() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)P(x)∧P(a)");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();
		Clause output = i.next().resolve(i.next());			
		assertEquals(output, null);
	}

	@Test
	void test_resolve_2() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)P(x)∧¬Q(a)");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();
		Clause output = i.next().resolve(i.next());			
		assertEquals(output, null);
	}

	@Test
	void test_resolve_3() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)P(x)∧¬P(a, b)");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();
		Clause output = i.next().resolve(i.next());			
		assertEquals(output, null);
	}

	@Test
	void test_resolve_4() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)((¬P(x)∨Q(x, y))∧(P(a)∨Q(x, y)))");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();
		Clause output = i.next().resolve(i.next());	
		assertEquals(output.toString().equals("Q(a, y)"), true);
	}

	@Test
	void test_resolve_5() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)(∀y)((¬P(x, y)∨Q(x, y))∧(P(a, b)∨Q(x, y)))");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();
		Clause output = i.next().resolve(i.next());	
		assertEquals(output.toString().equals("Q(a, b)"), true);
	}

	@Test
	void test_resolve_6() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)(∃y)((¬P(x, y)∨Q(x, y))∧(P(a, y)∨Q(x, y)))");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();
		Clause output = i.next().resolve(i.next());	
		assertEquals(output.toString().equals("Q(a, F_y(a))"), true);
	}

	@Test
	void test_resolve_7() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)(∀y)((¬P(x, y)∨Q(x, y))∧(P(y, a)∨Q(x, x)))");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();	
		Clause output = i.next().resolve(i.next());	
		assertEquals(output.toString().equals("Q(a, a)"), true);
	}

	@Test
	void test_combine_0() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)(∀y)(¬P(x, y)∧P(y, a))");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();	
		Replacement output = Replacement.getMGU(i.next().getLiterals().iterator().next(), i.next().getLiterals().iterator().next());		
		assertEquals(output.toString().equals("{a/x, a/y}"), true);
	}

	@Test
	void test_combine_1() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)(∀y)(¬P(x, y)∧P(x, x))");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();	
		Replacement output = Replacement.getMGU(i.next().getLiterals().iterator().next(), i.next().getLiterals().iterator().next());		
		assertEquals(output.toString().equals("{x/y}"), true);
	}

	@Test
	void test_combine_2() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)(∀y)(¬P(x, y)∧P(y, x))");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();	
		Replacement output = Replacement.getMGU(i.next().getLiterals().iterator().next(), i.next().getLiterals().iterator().next());		
		assertEquals(output.toString().equals("{x/y}"), true);
	}

	@Test
	void test_combine_3() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)(∀z)(∃y)(¬P(x, y)∧P(z, z))");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();	
		Literal l1 = i.next().getLiterals().iterator().next();
		Literal l2 = i.next().getLiterals().iterator().next();
		Replacement output = Replacement.getMGU(l1, l2);
		assertEquals(output.toString().equals("{F_y(x, x)/z, F_y(x, x)/x}"), true);
	}
	

	@Test
	void test_getReplacement_0() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)(∀y)P(x, y)");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();	
		Literal l = i.next().getLiterals().iterator().next();
		Replacement output = Replacement.getReplacement(l.getAtom().getTerms().get(0), l.getAtom().getTerms().get(1));		
		assertEquals(output.toString().equals("{y/x}"), true);
	}
	
	@Test
	void test_getReplacement_1() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)(∀y)P(a, x)");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();	
		Literal l = i.next().getLiterals().iterator().next();
		Replacement output = Replacement.getReplacement(l.getAtom().getTerms().get(0), l.getAtom().getTerms().get(1));		
		assertEquals(output.toString().equals("{a/x}"), true);
	}
	
	@Test
	void test_getReplacement_2() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)(∀y)P(x, x)");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();	
		Literal l = i.next().getLiterals().iterator().next();
		Replacement output = Replacement.getReplacement(l.getAtom().getTerms().get(0), l.getAtom().getTerms().get(1));		
		assertEquals(output.toString().equals("{}"), true);
	}
	
	@Test
	void test_getReplacement_3() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)(∀y)P(a, b)");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();	
		Literal l = i.next().getLiterals().iterator().next();
		Replacement output = Replacement.getReplacement(l.getAtom().getTerms().get(0), l.getAtom().getTerms().get(1));		
		assertEquals(output, null);
	}
	
	@Test
	void test_getReplacement_4() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)(∀y)P(a, a)");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();	
		Literal l = i.next().getLiterals().iterator().next();
		Replacement output = Replacement.getReplacement(l.getAtom().getTerms().get(0), l.getAtom().getTerms().get(1));		
		assertEquals(output.toString().equals("{}"), true);
	}
	
	@Test
	void test_getReplacement_5() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)(∀y)P(x, y)");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();	
		Literal l = i.next().getLiterals().iterator().next();
		Replacement output = Replacement.getReplacement(l.getAtom().getTerms().get(0), l.getAtom().getTerms().get(1));		
		assertEquals(output.toString().equals("{y/x}"), true);
	}
	
	@Test
	void test_getReplacement_6() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)(∃y)P(x, y)");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();	
		Literal l = i.next().getLiterals().iterator().next();
		Replacement output = Replacement.getReplacement(l.getAtom().getTerms().get(0), l.getAtom().getTerms().get(1));		
		assertEquals(output.toString().equals("{F_y(x)/x}"), true);
	}
	
	@Test
	void test_getReplacement_7() throws ParseException, IOException {
		Formula f = Formula.parse("(∀x)(∀z)(∃y)P(f(x, g(y)), f(a, g(z)))");
		Set<Clause> cs = Clause.fromFormulaToClauses(f);
		Iterator<Clause> i = cs.iterator();	
		Literal l = i.next().getLiterals().iterator().next();
		Replacement output = Replacement.getReplacement(l.getAtom().getTerms().get(0), l.getAtom().getTerms().get(1));		
		assertEquals(output.toString().equals("{a/x, F_y(x, z)/z}"), true);
	}

}
