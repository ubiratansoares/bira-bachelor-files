package compiler.lexer;

import compiler.Token;

/**
 * Estado de token de menor-ou-igual reconhecido.
 *
 */
public class LesserOrEqualState extends StopState {

	/**
	 * Retorna o token de menor-ou-igual.
	 *
	 * @return
	 * Um token de menor-ou-igual.
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
	private LesserOrEqualState() {
	}

	private static final State SINGLETON = new LesserOrEqualState();

}
