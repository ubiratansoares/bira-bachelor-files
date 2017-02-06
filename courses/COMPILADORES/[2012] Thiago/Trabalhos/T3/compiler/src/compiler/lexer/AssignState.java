package compiler.lexer;

import compiler.Token;

/**
 * Estado de símbolo de atribuição reconhecido.
 *
 */
public class AssignState extends StopState {

	/**
	 * Retorna o token de atribuição.
	 *
	 * @return
	 * Um token de atribuição.
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
	private AssignState() {
	}

	private static final State SINGLETON = new AssignState();

}
