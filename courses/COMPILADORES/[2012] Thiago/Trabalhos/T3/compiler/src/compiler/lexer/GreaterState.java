package compiler.lexer;

import compiler.Token;

/**
 * Estado de maior-que lido do buffer.
 *
 */
public class GreaterState extends State {

	/**
	 * Próximos estados:
	 * {@link GreaterOrEqualState},
	 * {@link GreaterEndState}.
	 *
	 * A cadeia é atualizada.
	 *
	 * @return
	 * O próxmo estado.
	 */
	@Override
	public State next(char c) {
		if(c == '=')
			return GreaterOrEqualState.getInstance();
		else
			return GreaterEndState.getInstance();
	}

	/**
	 * Acrescenta o caractere à cadeia, se for um '='.
	 * Não altera a cadeia, caso contrário.
	 */
	@Override
	public void updateString(char c, StringBuilder sb) {
		if(c == '=')
			sb.append(c);
		return;
	}

	/**
	 * Retorna o token de maior-que.
	 *
	 * @return
	 * Um token de maior-que.
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
	private GreaterState() {
	}

	private static final State SINGLETON = new GreaterState();

}
