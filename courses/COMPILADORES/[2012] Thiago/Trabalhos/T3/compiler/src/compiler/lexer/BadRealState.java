package compiler.lexer;

import compiler.LexicalException;

/**
 * Estado de constante real mal-formada.
 *
 */
public class BadRealState extends StopState {

	/**
	 * Lança um erro léxico.
	 *
	 * @param str
	 * A cadeia do token.
	 *
	 * @return
	 * Não retorna.
	 *
	 * @throws LexicalException
	 * Lançada para indicar uma constante real mal formada,
	 * isto é, sem a parte fracionária.
	 */
	@Override
	public compiler.Token getToken(String str) throws LexicalException {
		throw new LexicalException(str, "Constante real mal-formada.");
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
	private BadRealState() {
	}

	private static final State SINGLETON = new BadRealState();

}
