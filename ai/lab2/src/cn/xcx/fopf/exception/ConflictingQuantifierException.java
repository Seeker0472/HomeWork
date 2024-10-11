package cn.xcx.fopf.exception;

public class ConflictingQuantifierException extends ParseException {

	public ConflictingQuantifierException(String variable) {
		super("Conflicting quantifiers with variable of " + variable + ".  Please use different variable names.");
	}
}
