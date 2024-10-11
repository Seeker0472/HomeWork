package test;

import static org.junit.jupiter.api.Assertions.*;

import java.util.Set;

import org.junit.jupiter.api.Test;

import com.alibaba.fastjson.JSON;

import cn.xcx.fopf.Formula;
import cn.xcx.fopf.exception.ParseException;
import cn.xcx.reasoning.Clause;

class ClauseTest {
	@Test
	void test_replace2_0() throws ParseException {
		Formula input =   Formula.parse("(∀x)(∀y)(∀z)(P(x,y)∧P(y,z))∧(∀y)(∃x)P(x,y)");
		Formula output = Formula.parse("(∀x)(∀y)(∀z)(P(x,y)∧P(y,z))∧(∀y)(∃x)P(x,y)");
		Formula result = Clause.replace2(input);
		assertEquals(JSON.toJSONString(result).equals(JSON.toJSONString(output)), true);
	}
	
	@Test
	void test_replace2_1() throws ParseException {
		Formula input = Formula.parse("(∀x)(∀y)(∀z)(P(x,y)∧P(y,z)→P(x,z))∧(∀y)(∃x)P(x,y)");
		Formula output = Formula.parse("(∀x)(∀y)(∀z)(¬(P(x,y)∧P(y,z))∨P(x,z))∧(∀y)(∃x)P(x,y)");
		Formula result = Clause.replace2(input);
		assertEquals(JSON.toJSONString(result).equals(JSON.toJSONString(output)), true);
	}
	
	@Test
	void test_replace2_2() throws ParseException {
		Formula input = Formula.parse("(∀x)((∀y)P(x,y)→¬(∀y)(Q(x,y)→R(x,y)))");
		Formula output = Formula.parse("(∀x)(¬(∀y)P(x,y)∨¬(∀y)(¬Q(x,y)∨R(x,y)))");
		Formula result = Clause.replace2(input);
		assertEquals(JSON.toJSONString(result).equals(JSON.toJSONString(output)), true);
	}
	
	
	@Test
	void test_pushDownNot_0() throws ParseException {
		Formula input =   Formula.parse("(∀x)(∀y)(∀z)(¬P(x,y)∧P(y,z))∧(∀y)(∃x)P(x,y)");
		Formula output = Formula.parse("(∀x)(∀y)(∀z)(¬P(x,y)∧P(y,z))∧(∀y)(∃x)P(x,y)");
		Formula result = Clause.pushDownNot(input);
		assertEquals(JSON.toJSONString(result).equals(JSON.toJSONString(output)), true);
	}

	@Test
	void test_pushDownNot_1() throws ParseException {
		Formula input =   Formula.parse("(∀x)(∀y)(∀z)(¬(P(x,y)∧P(y,z))∨P(x,z))∧(∀y)(∃x)P(x,y)");
		Formula output = Formula.parse("(∀x)(∀y)(∀z)(¬P(x,y)∨¬P(y,z)∨P(x,z))∧(∀y)(∃x)P(x,y)");
		Formula result = Clause.pushDownNot(input);
		assertEquals(JSON.toJSONString(result).equals(JSON.toJSONString(output)), true);
	}

	@Test
	void test_pushDownNot_2() throws ParseException {
		Formula input =   Formula.parse("(∀x)(¬(∀y) P(x,y) ∨¬(∀y)(¬Q(x,y)∨R(x,y)))");
		Formula output = Formula.parse("(∀x)((∃y)¬ P(x,y) ∨(∃y)(Q(x,y)∧¬R(x,y)))");
		Formula result = Clause.pushDownNot(input);
		assertEquals(JSON.toJSONString(result).equals(JSON.toJSONString(output)), true);
	}
	
	
	@Test
	void test_changeVariableName_0() throws ParseException {
		Formula input =   Formula.parse("(∀x)(∀y)(∀z)(¬P(x,y)∧P(y,z))∧(∀y)(∃x)P(x,y)");
		Formula output = Formula.parse("(∀x)(∀y)(∀z)(¬P(x,y)∧P(y,z))∧(∀y)(∃x)P(x,y)");
		Clause.changeVariableName(input, "u", "v");
		assertEquals(JSON.toJSONString(input).equals(JSON.toJSONString(output)), true);
	}
	
	@Test
	void test_changeVariableName_1() throws ParseException {
		Formula input =   Formula.parse("(∀x)(∀y)(∀z)(¬P(x,y)∧P(y,z))∧(∀u)(∃v)P(v,u)");
		Formula output = Formula.parse("(∀x)(∀y)(∀z)(¬P(x,y)∧P(y,z))∧(∀u)(∃v)P(v,w)");
		Clause.changeVariableName(input, "u", "w");
		assertEquals(JSON.toJSONString(input).equals(JSON.toJSONString(output)), true);
	}
	
	@Test
	void test_changeVariableName_2() throws ParseException {
		Formula input =   Formula.parse("(∀x)(∀y)(∀z)(¬P(x,y)∧P(y,z))∧(∀x)(∃v)P(x,u)");
		Formula output = Formula.parse("(∀x)(∀y)(∀z)(¬P(u,y)∧P(y,z))∧(∀x)(∃v)P(u,u)");
		Clause.changeVariableName(input, "x", "u");
		assertEquals(JSON.toJSONString(input).equals(JSON.toJSONString(output)), true);
	}
	
	
	@Test
	void test_replaceExistentialQuantifiers_0() throws ParseException {
		Formula input =   Formula.parse("(∀x)(∀y)(∀z)(¬P(x,y)∧P(y,z))∧(∀y)(∀x)P(x,y)");
		Formula output = Formula.parse("(∀x)(∀y)(∀z)(¬P(x,y)∧P(y,z))∧(∀y)(∀x)P(x,y)");
		Clause.replaceExistentialQuantifiers(input);
		assertEquals(JSON.toJSONString(input).equals(JSON.toJSONString(output)), true);
	}
	
	@Test
	void test_replaceExistentialQuantifiers_1() throws ParseException {
		Formula input =   Formula.parse("(∃v)((¬P(x,y)∧P(y,z))∧P(v,u))");
		Formula output = Formula.parse("(¬P(x,y)∧P(y,z))∧P(v,u)");
		Clause.replaceExistentialQuantifiers(input);
		assertEquals(JSON.toJSONString(input).equals(JSON.toJSONString(output)), true);
	}
	
	@Test
	void test_replaceExistentialQuantifiers_2() throws ParseException {
		Formula input =   Formula.parse("(∀x)(∃v)(∀y)(∀z)(¬P(x,y)∧P(y,z)∧P(v,u))");
		Formula output = Formula.parse("(∀x)(∀y)(∀z)(¬P(x,y)∧P(y,z)∧P(F_v(x), u))");
		Clause.replaceExistentialQuantifiers(input);
		assertEquals(JSON.toJSONString(input).equals(JSON.toJSONString(output)), true);
	}
	
	@Test
	void test_replaceExistentialQuantifiers_3() throws ParseException {
		Formula input =   Formula.parse("(∃u)(∀x)(∃v)(∀y)(∀z)(¬P(x,v)∧P(y,u)∧P(v,u))");
		Formula output = Formula.parse("(∀x)(∀y)(∀z)(¬P(x,F_v(x))∧P(y,u)∧P(F_v(x), u))");
		Clause.replaceExistentialQuantifiers(input);
		assertEquals(JSON.toJSONString(input).equals(JSON.toJSONString(output)), true);
	}
	
	
	@Test
	void test_putUpConjunction_0() throws ParseException {
		Formula input =   Formula.parse("(¬P(x,y)∨P(y,z))∧P(x,y)");
		Formula output = Formula.parse("(¬P(x,y)∨P(y,z))∧P(x,y)");
		Clause.putUpConjunction(input);
		assertEquals(JSON.toJSONString(input).equals(JSON.toJSONString(output)), true);
	}
	
	@Test
	void test_putUpConjunction_1() throws ParseException {
		Formula input =   Formula.parse("¬P(x,y)∨P(y,z)∧P(x,y)");
		Formula output = Formula.parse("(¬P(x,y)∨P(y,z))∧(¬P(x,y)∨P(x,y))");
		input = Clause.putUpConjunction(input);
		assertEquals(JSON.toJSONString(input).equals(JSON.toJSONString(output)), true);
	}
	
	@Test
	void test_putUpConjunction_2() throws ParseException {
		Formula input =   Formula.parse("¬P(x,y)∨P(y,z)∧Q(x,y)∧R(x,y)");
		Formula output = Formula.parse("(¬P(x,y)∨P(y,z))∧(¬P(x,y)∨Q(x,y))∧(¬P(x,y)∨R(x,y))");
		input = Clause.putUpConjunction(input);
		assertEquals(JSON.toJSONString(input).equals(JSON.toJSONString(output)), true);
	}
	
	@Test
	void test_putUpConjunction_3() throws ParseException {
		Formula input =   Formula.parse("¬P(x,y)∧R(x,y)∨P(y,z)∧Q(x,y)");
		Formula output = Formula.parse("((¬P(x,y)∨P(y,z))∧(¬P(x,y)∨Q(x,y)))∧((R(x,y)∨P(y,z))∧(R(x,y)∨Q(x,y)))");
		input = Clause.putUpConjunction(input);
		assertEquals(JSON.toJSONString(input).equals(JSON.toJSONString(output)), true);
	}
	
	@Test
	void test_putUpConjunction_4() throws ParseException {
		Formula input =   Formula.parse("¬P(x,y)∨(R(x,y)∨P(y,z)∧Q(x,y))");
		Formula output = Formula.parse("(¬P(x,y)∨(R(x,y)∨P(y,z)))∧(¬P(x,y)∨(R(x,y)∨Q(x,y)))");
		input = Clause.putUpConjunction(input);
		assertEquals(JSON.toJSONString(input).equals(JSON.toJSONString(output)), true);
	}
	
	@Test
	void test_splitByConjunction_0() throws ParseException {
		Set<Clause> input = Clause.splitByConjunction(Formula.parse("(¬P(x,y)∧P(y,z))∧P(x,y)"));
		Set<Clause> output = Clause.splitByConjunction(Formula.parse("(¬P(x,y)∧P(y,z))∧P(x,y)"));
		assertEquals(JSON.toJSONString(input).equals(JSON.toJSONString(output)), true);
	}
	
	@Test
	void test_splitByConjunction_1() throws ParseException {
		Formula input =   Formula.parse("P(x,y)");
		Set<Clause> output = Clause.splitByConjunction(input);
		assertEquals(output.size(), 1);
	}
	
	@Test
	void test_splitByConjunction_2() throws ParseException {
		Formula input =   Formula.parse("(¬P(x,y)∨P(y,z))∧Q(x,y)");
		Set<Clause> output = Clause.splitByConjunction(input);
		assertEquals(output.size(), 2);
	}
	
	@Test
	void test_splitByConjunction_3() throws ParseException {
		Formula input =  Formula.parse("((¬P(x,y)∨P(y,z))∧(¬P(x,y)∨Q(x,y)))∧((R(x,y)∨P(y,z))∧(R(x,y)∨Q(x,y)))");
		Set<Clause> output = Clause.splitByConjunction(input);
		assertEquals(output.size(), 4);
	}
}
