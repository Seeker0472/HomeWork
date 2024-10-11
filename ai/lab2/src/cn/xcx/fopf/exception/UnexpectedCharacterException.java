package cn.xcx.fopf.exception;

public class UnexpectedCharacterException extends ParseException {
	public UnexpectedCharacterException(String s, int from, int status) {
		super("Unexpected character starts from '" + s.substring(from) + "'@("+ from +").  " + getExpectedCharacters(status) + " is expected");
	}
}
