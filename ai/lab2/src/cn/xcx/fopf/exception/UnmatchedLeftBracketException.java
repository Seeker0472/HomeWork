package cn.xcx.fopf.exception;

public class UnmatchedLeftBracketException extends ParseException {
	public UnmatchedLeftBracketException() {
		super("Unmatched '(' in formula");
	}
}
