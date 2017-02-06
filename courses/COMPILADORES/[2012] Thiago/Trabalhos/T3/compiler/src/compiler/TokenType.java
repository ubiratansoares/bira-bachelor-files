package compiler;

/**
 * Enumera os tipos de tokens.
 *
 */
public enum TokenType {

	//tipos genéricos
	ERROR("erro"), EOF("fim de arquivo"),
	IDENTIFIER("identificador"),
	CONST_INTEGER("constante inteira"),
	CONST_REAL("constante real"),

	//tipos para símbolos reservados
	SYMBOL_COLON(":"), SYMBOL_PERIOD("."),
	SYMBOL_SEMICOLON(";"), SYMBOL_COMMA(","),
	SYMBOL_LPAR("("), SYMBOL_RPAR(")"),
	SYMBOL_ASSIGN(":="),
	SYMBOL_EQ("="), SYMBOL_NE("<>"), 
	SYMBOL_GE(">="), SYMBOL_LE("<="),
	SYMBOL_GT(">"), SYMBOL_LT("<"),
	SYMBOL_PLUS("+"), SYMBOL_MINUS("-"),
	SYMBOL_MULT("*"), SYMBOL_DIV("/"),

	//tipos para palavras reservadas
	KEYWORD_PROGRAM("program"), KEYWORD_PROCEDURE("procedure"),
	KEYWORD_VAR("var"), KEYWORD_BEGIN("begin"), KEYWORD_END("end"),
	KEYWORD_IF("if"), KEYWORD_THEN("then"), KEYWORD_ELSE("else"),
	KEYWORD_WHILE("while"), KEYWORD_DO("do"),
	KEYWORD_REPEAT("repeat"), KEYWORD_UNTIL("until"),
	KEYWORD_READ("readln"), KEYWORD_WRITE("writeln"),
	KEYWORD_INTEGER("integer"), KEYWORD_REAL("real");

	private String name;

	private TokenType(String name) {
		this.name = name;
	}

	/**
	 * Representa o tipo de token pelo nome.
	 *
	 * @return
	 * O nome do tipo de token.
	 */
	@Override
	public String toString() {
		return name;
	}

}
