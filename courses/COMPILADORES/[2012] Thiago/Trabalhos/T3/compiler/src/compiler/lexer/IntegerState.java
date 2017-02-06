package compiler.lexer;

import compiler.Token;

/**
 * Estado de número inteiro reconhecido.
 *
 */
public class IntegerState extends StopState {

	/**
	 * Retorna um token de constante inteira.
	 *
	 * @param str
	 * A cadeia do token.
	 *
	 * @return
	 * A constante inteira.
	 */
	@Override
	public Token getToken(String str) {
		return Token.createInteger(str);
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
	private IntegerState() {
	}

	private static final State SINGLETON = new IntegerState();

}
