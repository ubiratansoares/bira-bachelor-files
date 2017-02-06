package compiler.lexer;

import compiler.Token;

/**
 * Estado de menor-que lido do buffer.
 *
 */
public class LesserState extends State {

	/**
	 * Próximos estados:
	 * {@link LesserOrEqualState},
	 * {@link LesserEndState},
	 * {@link NotEqualState}.
	 *
	 * @return
	 * O próxmo estado.
	 */
	@Override
	public State next(char c) {
		switch(c) {
			case '=':
				return LesserOrEqualState.getInstance();
			case '>':
				return NotEqualState.getInstance();
			default:
				return LesserEndState.getInstance();
		}
	}

	/**
	 * Acrescenta o caractere à cadeia, se for um '=' ou um '&gt;'.
	 * Não altera a cadeia, caso contrário.
	 */
	@Override
	public void updateString(char c, StringBuilder sb) {
		if(c == '=' || c == '>')
			sb.append(c);
		return;
	}

	/**
	 * Retorna o token de menor-que.
	 *
	 * @return
	 * Um token de menor-que.
	 */
	@Override
	public compiler.Token getToken(String str) {
		return Token.createSymbol(str);
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
	private LesserState() {
	}

	private static final State SINGLETON = new LesserState();

}
