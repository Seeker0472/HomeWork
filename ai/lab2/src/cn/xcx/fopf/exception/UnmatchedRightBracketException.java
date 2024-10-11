package cn.xcx.fopf.exception;

public class UnmatchedRightBracketException extends ParseException {
	
	public UnmatchedRightBracketException(String s, int from, int status) {
		super("Unmatched ')' starts from [" + s.substring(from) + "]@("+ from +").  " + getExpectedCharacters(status) + " is expected");
	}

}
