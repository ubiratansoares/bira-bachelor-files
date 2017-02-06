package compiler.lexer;

import compiler.Token;

/**
 * Estado de número real reconhecido.
 *
 */
public class RealState extends StopState {

	/**
	 * Retorna um token de constante real.
	 *
	 * @param str
	 * A cadeia do token.
	 *
	 * @return
	 * A constante real.
	 */
	@Override
	public Token getToken(String str) {
		return Token.createReal(str);
	}

	/**
	 * Indica que o caracter lido anteriormente deve ser
	 * devolvido ao buffer.
	 *
	 * @return
	 * Apenas {@code true}.
	 */
	@Override
	public boolean hasForwardLooked() {
		return true;
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
	private RealState() {
	}

	private static final State SINGLETON = new RealState();

}
