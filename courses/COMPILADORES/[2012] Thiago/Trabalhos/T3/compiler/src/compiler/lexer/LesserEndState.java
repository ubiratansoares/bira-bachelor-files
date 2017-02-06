package compiler.lexer;

import compiler.Token;

/**
 * Estado de token de menor-que reconhecido.
 *
 */
public class LesserEndState extends StopState {

	/**
	 * Retorna o token de menor-que.
	 *
	 * @return
	 * Um token de menor-que.
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
	private LesserEndState() {
	}

	private static final State SINGLETON = new LesserEndState();

}
