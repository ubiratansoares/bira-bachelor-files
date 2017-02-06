package compiler.lexer;

import compiler.Token;

/**
 * Estado de único símbolo identificado.
 *
 */
public class SingleSymbolState extends StopState {

	/**
	 * Retorna o token válido de único símbolo.
	 *
	 * @return
	 * Um token de símbolo reservado de comprimento 1.
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
	private SingleSymbolState() {
	}

	private static final State SINGLETON = new SingleSymbolState();

}
