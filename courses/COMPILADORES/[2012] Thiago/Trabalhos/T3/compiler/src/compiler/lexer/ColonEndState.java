package compiler.lexer;

import compiler.Token;

/**
 * Estado de token de dois-pontos reconhecido.
 *
 */
public class ColonEndState extends StopState {

	/**
	 * Retorna o token de dois-pontos.
	 *
	 * @return
	 * Um token de dois-pontos.
	 */
	@Override
	public compiler.Token getToken(String str) {
		return Token.createSymbol(str);
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
	private ColonEndState() {
	}

	private static final State SINGLETON = new ColonEndState();

}
