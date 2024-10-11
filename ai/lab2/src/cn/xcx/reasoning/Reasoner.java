package cn.xcx.reasoning;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import cn.xcx.fopf.Formula;
import cn.xcx.fopf.exception.ParseException;

public class Reasoner {
	private String[] formulaStrings;
	private Set<Clause> clauses;
	private List<Set<Clause>> resolvedClauses; 
	
	public Reasoner(String inputFile) throws IOException, ParseException {		
		readInputFile(inputFile);	
		getClauseSet();			
		resolvedClauses = new ArrayList<Set<Clause>>();
	}
	
	public Reasoner( Set<Clause> clauseSet) throws IOException, ParseException {		
		this.clauses = clauseSet;			
		resolvedClauses = new ArrayList<Set<Clause>>();
	}
	
	public static void main(String[] args) throws ParseException, IOException {
		if (args.length != 1) {
			System.out.println("Only a parameter is permitted, which is a input text file and each line of it is a first-order prdicate formula.");
			System.exit(-1);
		}
		Reasoner reasoner = new Reasoner(args[0]);		
		
		reasoner.reason();
	}
	
	// 对子句集中的每个子句进行处理，完成两个子句归结的前置处理步骤, 
	// 使每个子句内部不存在可合一的文字
	// 每个子句中的变量都加上前缀"c" + start++ + "_",确保每个子句中的变量都不重名
	private void prepareForMerge(Set<Clause> cs, int start) {
		Clause.mergeInternalLiterals(cs);
		for (Clause c: cs) {
			c.addPrefixToVariables("c" + start++ + "_");
		}
	}
	
	// 从输入文件读取问题的公式，要求输入文件中每个公式一行
	private void readInputFile(String path) throws IOException {
		List<String> r = new ArrayList<>();
		
		InputStreamReader read = new InputStreamReader(new FileInputStream(path), "UTF-8");
        BufferedReader fr = new BufferedReader(read);
		String line;
		while ((line = fr.readLine()) != null) {
			System.out.println(line);
			r.add(line);
		}
		fr.close();
		
		formulaStrings =  (String[]) r.toArray(new String[0]);
	}
	
	// 将读取的公式字符串数组，转换为子句集
	private void getClauseSet() throws ParseException {
		clauses = new HashSet<Clause>();
		for (String s: formulaStrings) {
			Formula f = Formula.parse(s);
			Set<Clause> cs = Clause.fromFormulaToClauses(f);
			prepareForMerge(cs, 0);
			clauses.addAll(cs);
		}
	}
	
	// 归结推理
	// 利用clauses属性中保存的子句集作为推理输入
	// 每一次推理循环中产生的归结式集合都保存在推理机的属性resolvedClauses中
	// 输出表示推理结果的字符串
	// 	如果求证成功，输出“Proved!"
	// 	如果是求解问题推理成功，输出”Found {n} answers: {%s }*n“，其中{n}表示答案的数量，{%s }*n表示n个答案，每个答案后面有一个空格
	// 	如果推理失败"Failed!"
	public String reason() {
		StringBuilder r = new StringBuilder();
		Set<Clause> currentClauses = new HashSet<>(clauses);
		Set<Clause> resolvedClauses = new HashSet<>();
		Set<Clause> answers = new HashSet<>();

		boolean find_new = false;
		do {
			find_new=false;
			for (Clause cl1 : currentClauses) {
				for (Clause cl2 : currentClauses) {
					if (!cl1.equals(cl2)) {
						Clause resolved = cl1.resolve(cl2);
						if (resolved == null) {
							continue;
						}
						if (resolved.getLiterals().isEmpty()) {
							r.setLength(0);
							r.append("Proved!");
							return r.toString();
						}
						if(!resolvedClauses.contains(resolved)) {
							find_new = true;
							resolvedClauses.add(resolved);
						}
					}
				}
			}
			currentClauses.addAll(resolvedClauses);
		}while(find_new);

		if(resolvedClauses.isEmpty()) {
			r.setLength(0);
			r.append("Failed!");
			return r.toString();
		}
		for(Clause cl: resolvedClauses) {
			if ((cl.getLiterals().size() == 1) && cl.getLiterals().iterator().next().isAnswer()){
					answers.add(cl);
			}
		}
		r = new StringBuilder("Found " + answers.size() + " answers: ");
		for(Clause cl: answers) {
			r.append(cl.getLiterals().iterator().next().toString());
			r.append(" ");
		}
		return r.toString();
	}
}
