package compiler.lexer;

import compiler.Token;

/**
 * Estado de token de maior-ou-igual reconhecido.
 *
 */
public class GreaterOrEqualState extends StopState {

	/**
	 * Retorna o token de maior-ou-igual.
	 *
	 * @return
	 * Um token de maior-ou-igual.
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
	private GreaterOrEqualState() {
	}

	private static final State SINGLETON = new GreaterOrEqualState();

}
