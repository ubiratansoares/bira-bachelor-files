package compiler.lexer;

import compiler.Token;

/**
 * Estado de identificador identificado.
 *
 */
public class IdentifierEndState extends StopState {

	/**
	 * Indica que o caracter lido anteriormente deve
	 * ser devolvido ao buffer.
	 *
	 * @return
	 * Apenas {@code true}.
	 */
	@Override
	public boolean hasForwardLooked() {
		return true;
	}

	/**
	 * Retorna um identificador.
	 */
	@Override
	public Token getToken(String str) {
		return Token.createIdentifier(str);
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
	private IdentifierEndState() {
	}

	private static final State SINGLETON = new IdentifierEndState();

}
