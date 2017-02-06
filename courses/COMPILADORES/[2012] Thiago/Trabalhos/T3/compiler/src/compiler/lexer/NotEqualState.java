package compiler.lexer;

import compiler.Token;

/**
 * Estado de token de desigualdade reconhecido.
 *
 */
public class NotEqualState extends StopState {

	/**
	 * Retorna o token de desigualdade.
	 *
	 * @return
	 * Um token de diferente.
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
	private NotEqualState() {
	}

	private static final State SINGLETON = new NotEqualState();

}
