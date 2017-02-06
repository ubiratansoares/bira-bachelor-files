package compiler.lexer;

import compiler.Token;

/**
 * Estado inicial.
 *
 */
public class StartState extends State {

	/**
	 * Próximos estados: 
	 * {@code StartState}, {@link CommentState}, 
	 * {@link SingleSymbolState}, {@link ColonState}, 
	 * {@link GreaterState}, {@link LesserState},
	 * {@link CommentEndState}, {@link InvalidSymbolState},
	 * {@link NumberState}, {@link IdentifierState}.
	 */
	@Override
	public State next(char c) {
		switch(c) {
			//tokens triviais
			case ',': case '.': case ';': case '=':
			case '+': case '-': case '*': case '/':
			case '(': case ')':
				return SingleSymbolState.getInstance();

			//tokens de símbolos reservados compostos
			case '>':
				return GreaterState.getInstance();
			case '<':
				return LesserState.getInstance();
			case ':':
				return ColonState.getInstance();

			//outros tokens
			case '{':
				return CommentStartState.getInstance();
			case '}':
				return CommentEndState.getInstance();

			default:
				if(Character.isLetter(c)) {
					return IdentifierState.getInstance();
				} else if(Character.isDigit(c)) {
					return NumberState.getInstance();
				} else if(Character.isWhitespace(c)) {
					return this;
				} else {
					return InvalidSymbolState.getInstance();
				}
		}
	}

	/**
	 * Acrescenta o caractere à cadeia, exceto se o caractere
	 * for um espaço em branco.
	 */
	@Override
	public void updateString(char c, StringBuilder sb) {
		if(!Character.isWhitespace(c))
			sb.append(c);
		return;
	}

	/**
	 * Retorna um token EOF para indicar que não há mais tokens.
	 *
	 * @return
	 * Um token do tipo EOF.
	 */
	@Override
	public Token getToken(String str) {
		return Token.createEOF();
	}

	/**
	 * Obtém uma instância desta classe.
	 *
	 * @return
	 * A instância desta classe.
	 */
	public static State getInstance() {
		return SINGLETON;
	}

	//oculta o construtor padrão
	private StartState() {
	}

	private static final State SINGLETON = new StartState();

}
