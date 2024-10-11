package cn.xcx.fopf.exception;

public class ParseException extends Exception {
	private static final String[] EXPECTED_CHARACTERS = {
			"[' ' | ~ | (]",
			"[' ' | ~ | (]",
			"[' ' | ~ | * | ? | A...Z | a...z]",
			"[' ' | ~ | * | ? | A...Z | a...z | 0...9]",
			"[' ' | (]",
			"[' ' | A...Z | a...z]",  //5
			"[' ' | A...Z | a...z]",
			"[' ' | , | A...Z | a...z | 0...9 | ( | )]",
			"[' ' | , | ) | (]",
			"[' ' | ) | & | | | > | =]",
			"[' ' | ) | & | | | > | = | »»ÐÐ·û]",
			"[' ' | A...Z | a...z]",  //11
			"[' ' | A...Z | a...z | 0...9 | )]",
			"[' ' | )"
	};
	
	public ParseException(String s) {
		super(s);
	}
	
	public static String getExpectedCharacters(int status) {
		return EXPECTED_CHARACTERS[status];
	}
}
