package compiler;

import java.util.HashMap;
import java.util.Map;

import static java.util.Collections.unmodifiableMap;

/**
 * Token identificado pelo analisador léxico.
 *
 */
public class Token implements Cloneable {

	private String string;
	private TokenType type;
	private int line;
	private int column;

	/**
	 * Cria um token com cadeia e tipo.
	 *
	 * @param string
	 * A cadeia do token.
	 *
	 * @param type
	 * O tipo do token.
	 */
	private Token(String string, TokenType type) {
		this.type = type;
		this.string = string;
	}

	/**
	 * Obtém a cadeia do token.
	 *
	 * @return
	 * A cadeia do token.
	 */
	public String getString() {
		if(string.isEmpty())
			return "<vazio>";
		else
			return string;
	}

	/**
	 * Obtém o tipo do token.
	 *
	 * @return
	 * O tipo do token.
	 */
	public TokenType getType() {
		return type;
	}

	/**
	 * Retorna a representação textual do token.
	 * 
	 * @return
	 * A representação textual do token, na forma
	 * "token - cadeia".
	 */
	@Override
	public String toString() {
		return String.format("(%d, %d) %s - %s",
				this.getLine() + 1, this.getColumn() + 1,
				type.toString(), this.string);
	}

	/**
	 * Define o número da linha e o número da coluna onde ocorre o token.
	 *
	 * @param line
	 * O número da linha onde ocorre o token.
	 *
	 * @param column
	 * O número da coluna onde ocorre o token.
	 */
	public void setPosition(int line, int column) {
		this.line = line;
		this.column = column;
		return;
	}

	/**
	 * Obtém o número da linha onde ocorre o token.
	 *
	 * @return
	 * O número da linha onde ocorre o token.
	 */
	public int getLine() {
		return line;
	}

	/**
	 * Obtém o número da coluna onde ocorre o token.
	 *
	 * @return
	 * O número da coluna onde ocorre o token.
	 */
	public int getColumn() {
		return column;
	}

	/**
	 * Cria um token de erro.
	 *
	 * @return
	 * Um novo token de erro.
	 */
	public static Token createEOF() {
		return new Token("EOF", TokenType.EOF);
	}

	/**
	 * Cria um token de erro.
	 *
	 * @param s
	 * A cadeia que causou o erro.
	 *
	 * @return
	 * Um novo token de erro.
	 */
	public static Token createError(String s) {
		return new Token(s, TokenType.ERROR);
	}

	/**
	 * Cria um token de constante inteira.
	 *
	 * @param s
	 * A cadeia do token.
	 *
	 * @return
	 * Um novo token de constante inteira.
	 */
	public static Token createInteger(String s) {
		return new Token(s, TokenType.CONST_INTEGER);
	}

	/**
	 * Cria um token de constante real.
	 *
	 * @param s
	 * A cadeia do token.
	 *
	 * @return
	 * Um novo token de constante real.
	 */
	public static Token createReal(String s) {
		return new Token(s, TokenType.CONST_REAL);
	}

	/**
	 * Cria um token de identificador. O método não verifica se a
	 * cadeia é uma palavra reservada.
	 *
	 * @param s
	 * A cadeia do token.
	 *
	 * @return
	 * Um novo token identificador.
	 */
	public static Token createIdentifier(String s) {
		return new Token(s, TokenType.IDENTIFIER);
	}

	/**
	 * Cria um token de palavra reservada. Como esse tipo de token 
	 * pode ser compartilhado, este método apenas retorna o token
	 * correspondente de uma tabela.
	 *
	 * @param s
	 * A palavra reservada para o qual obter um token correspondente.
	 *
	 * @return
	 * Uma nova instância do token.
	 */
	public static Token createKeyword(String s) {
		if(keywords.containsKey(s)) {
			try {
				return (Token) keywords.get(s).clone();
			} catch(CloneNotSupportedException e) {
				return createError(s);
			}
		} else
			throw new IllegalArgumentException();
	}

	/**
	 * Cria um token de símbolo reservado. Como esse tipo de token
	 * pode ser compartilhado, este método apenas retorna o token
	 * correspondente de uma tabela.
	 *
	 * @param s
	 * O símbolo reservado para o qual obter um token correspondente.
	 *
	 * @return
	 * Uma nova instância do token.
	 */
	public static Token createSymbol(String s) {
		if(symbols.containsKey(s)) {
			try {
				return (Token) symbols.get(s).clone();
			} catch(CloneNotSupportedException e) {
				return createError(s);
			}
		} else
			throw new IllegalArgumentException();
	}

	//prototype
	private static final Map<String, Token> keywords = createKeywords();
	private static Map<String, Token> createKeywords() {
		Map<String, Token> map = new HashMap<String, Token>();

		//1. <programa> ::= program ident ; <corpo> .
		map.put("program", new Token("program", TokenType.KEYWORD_PROGRAM));

		//2. <corpo> ::= <dc> begin <comandos> end
		map.put("begin", new Token("begin", TokenType.KEYWORD_BEGIN));
		map.put("end", new Token("end", TokenType.KEYWORD_END));

		//3. <dc> ::= <dc_v> <dc_p>
		//4. <dc_v> ::= var <variaveis> : <tipo_var> ; <dc_v> | λ
		map.put("var", new Token("var", TokenType.KEYWORD_VAR));

		//5. <tipo_var> ::= real | integer
		map.put("real", new Token("real", TokenType.KEYWORD_REAL));
		map.put("integer", new Token("integer", TokenType.KEYWORD_INTEGER));

		//6. <variaveis> ::= ident <mais_var>
		//7. <mais_var> ::= , <variaveis> | λ
		//8. <dc_p> ::= procedure ident <parametros> ; <corpo_p> <dc_p> | λ
		map.put("procedure", new Token("procedure", TokenType.KEYWORD_PROCEDURE));

		//9. <parametros> ::= ( <lista_par> ) | λ
		//10. <lista_par> ::= <variaveis> : <tipo_var> <mais_par>
		//11. <mais_par> ::= ; <lista_par> | λ
		//12. <corpo_p> ::= <dc_loc> begin <comandos> end ;
		//13. <dc_loc> ::= <dc_v>
		//14. <lista_arg> ::= ( <argumentos> ) | λ
		//15. <argumentos> ::= ident <mais_ident>
		//16. <mais_ident> ::= ; <argumentos> | λ

		//17. <pfalsa> ::= else <cmd> | λ
		map.put("else", new Token("else", TokenType.KEYWORD_ELSE));

		//18. <comandos> ::= <cmd> ; <comandos> | λ
		/* 19. <cmd> ::= read ( <variaveis> ) |
		 *		write ( <variaveis> ) |
		 *		while <condicao> do <cmd> |
		 * 		if <condicao> then <cmd> <pfalsa> |
		 * 		ident := <expressao> |
		 * 		ident <lista_arg> |
		 * 		begin <comandos> end |
		 * 		repeat <comandos> until <condicao>
		 */
		map.put("readln", new Token("readln", TokenType.KEYWORD_READ));
		map.put("writeln", new Token("writeln", TokenType.KEYWORD_WRITE));
		map.put("while", new Token("while", TokenType.KEYWORD_WHILE));
		map.put("do", new Token("do", TokenType.KEYWORD_DO));
		map.put("if", new Token("if", TokenType.KEYWORD_IF));
		map.put("then", new Token("then", TokenType.KEYWORD_THEN));
		map.put("repeat", new Token("repeat", TokenType.KEYWORD_REPEAT));
		map.put("until", new Token("until", TokenType.KEYWORD_UNTIL));

		//20. <condicao> ::= <expressao> <relacao> <expressao>
		//21. <relacao> ::= = | <> | >= | <= | > | <
		//22. <expressao> ::= <termo> <outros_termos>
		//23. <op_un> ::= + | - | λ
		//24. <outros_termos> ::= <op_ad> <termo> <outros_termos> | λ
		//25. <op_ad> ::= + | -
		//26. <termo> ::= <op_un> <fator> <mais_fatores>
		//27. <mais_fatores> ::= <op_mul> <fator> <mais_fatores> | λ

		//28. <op_mul> ::= * | /
		//29. <fator> ::= ident | numero_int | numero_real | ( <expressao> )

		return unmodifiableMap(map);
	}

	//prototype
	private static final Map<String, Token> symbols = createSymbols();
	private static Map<String, Token> createSymbols() {
		Map<String, Token> map = new HashMap<String, Token>();

		//1. <programa> ::= program ident ; <corpo> .
		map.put(";", new Token(";", TokenType.SYMBOL_SEMICOLON));
		map.put(".", new Token(".", TokenType.SYMBOL_PERIOD));

		//2. <corpo> ::= <dc> begin <comandos> end
		//3. <dc> ::= <dc_v> <dc_p>
		//4. <dc_v> ::= var <variaveis> : <tipo_var> ; <dc_v> | λ
		map.put(":", new Token(":", TokenType.SYMBOL_COLON));

		//5. <tipo_var> ::= real | integer
		//6. <variaveis> ::= ident <mais_var>
		//7. <mais_var> ::= , <variaveis> | λ
		map.put(",", new Token(",", TokenType.SYMBOL_COMMA));

		//8. <dc_p> ::= procedure ident <parametros> ; <corpo_p> <dc_p> | λ
		//9. <parametros> ::= ( <lista_par> ) | λ
		map.put("(", new Token("(", TokenType.SYMBOL_LPAR));
		map.put(")", new Token(")", TokenType.SYMBOL_RPAR));

		//10. <lista_par> ::= <variaveis> : <tipo_var> <mais_par>
		//11. <mais_par> ::= ; <lista_par> | λ
		//12. <corpo_p> ::= <dc_loc> begin <comandos> end ;
		//13. <dc_loc> ::= <dc_v>
		//14. <lista_arg> ::= ( <argumentos> ) | λ
		//15. <argumentos> ::= ident <mais_ident>
		//16. <mais_ident> ::= ; <argumentos> | λ

		//17. <pfalsa> ::= else <cmd> | λ
		//18. <comandos> ::= <cmd> ; <comandos> | λ
		/* 19. <cmd> ::= read ( <variaveis> ) |
		 *		write ( <variaveis> ) |
		 *		while <condicao> do <cmd> |
		 * 		if <condicao> then <cmd> <pfalsa> |
		 * 		ident := <expressao> |
		 * 		ident <lista_arg> |
		 * 		begin <comandos> end |
		 * 		repeat <comandos> until <condicao>
		 */
		map.put(":=", new Token(":=", TokenType.SYMBOL_ASSIGN));

		//20. <condicao> ::= <expressao> <relacao> <expressao>
		//21. <relacao> ::= = | <> | >= | <= | > | <
		map.put("=", new Token("=", TokenType.SYMBOL_EQ));
		map.put("<>", new Token("<>", TokenType.SYMBOL_NE));
		map.put(">=", new Token(">=", TokenType.SYMBOL_GE));
		map.put("<=", new Token("<=", TokenType.SYMBOL_LE));
		map.put(">", new Token(">", TokenType.SYMBOL_GT));
		map.put("<", new Token("<", TokenType.SYMBOL_LT));

		//22. <expressao> ::= <termo> <outros_termos>
		//23. <op_un> ::= + | - | λ
		map.put("+", new Token("+", TokenType.SYMBOL_PLUS));
		map.put("-", new Token("-", TokenType.SYMBOL_MINUS));

		//24. <outros_termos> ::= <op_ad> <termo> <outros_termos> | λ
		//25. <op_ad> ::= + | -
		//26. <termo> ::= <op_un> <fator> <mais_fatores>
		//27. <mais_fatores> ::= <op_mul> <fator> <mais_fatores> | λ

		//28. <op_mul> ::= * | /
		map.put("*", new Token("*", TokenType.SYMBOL_MULT));
		map.put("/", new Token("/", TokenType.SYMBOL_DIV));

		//29. <fator> ::= ident | numero_int | numero_real | ( <expressao> )

		return unmodifiableMap(map);
	}

}
