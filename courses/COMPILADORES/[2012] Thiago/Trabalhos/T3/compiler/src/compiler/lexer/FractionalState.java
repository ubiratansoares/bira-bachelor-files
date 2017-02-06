package compiler.lexer;

import compiler.Token;

/**
 * Estado de leitura de parte fracionária de um número real.
 *
 */
public class FractionalState extends State {

	/**
	 * Próximos estados:
	 * {@code FractionalState},
	 * {@link RealState}.
	 *
	 * @return
	 * O próxmo estado.
	 */
	@Override
	public State next(char c) {
		if(Character.isDigit(c))
			return this;
		else
			return RealState.getInstance();
	}

	/**
	 * Acrescenta o caractere à cadeia, se for um dígito.
	 * Não altera a cadeia, caso contrário.
	 */
	@Override
	public void updateString(char c, StringBuilder sb) {
		if(Character.isDigit(c))
			sb.append(c);
		return;
	}

	/**
	 * Retorna o token de constante real.
	 *
	 * @return
	 * Um token de constante real.
	 */
	@Override
	public compiler.Token getToken(String str) {
		return Token.createReal(str);
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
	private FractionalState() {
	}

	private static final State SINGLETON = new FractionalState();

}
