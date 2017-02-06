package compiler.lexer;

import compiler.Token;

/**
 * Estado de dígito lido do buffer.
 *
 */
public class NumberState extends State {

	/**
	 * Próximos estados:
	 * {@code NumberState},
	 * {@link IntegerState},
	 * {@link FractionalState}.
	 *
	 * @return
	 * O próxmo estado.
	 */
	@Override
	public State next(char c) {
		if(c == '.') {
			return FirstFractionalState.getInstance();
		} else if(Character.isDigit(c)) {
			return this;
		} else
			return IntegerState.getInstance();
	}

	/**
	 * Acrescenta o caractere à cadeia, se for um dígito ou um '.'.
	 * Não altera a cadeia, caso contrário.
	 */
	@Override
	public void updateString(char c, StringBuilder sb) {
		if(c == '.' || Character.isDigit(c))
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
		return Token.createInteger(str);
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
	private NumberState() {
	}

	private static final State SINGLETON = new NumberState();

}
