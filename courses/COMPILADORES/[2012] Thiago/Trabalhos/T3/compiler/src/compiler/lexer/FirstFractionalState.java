package compiler.lexer;

import compiler.LexicalException;

/**
 * Estado de ponto lido do buffer, após a leitura de um inteiro.
 *
 */
public class FirstFractionalState extends State {

	/**
	 * Próximos estados:
	 * {@link FractionalState},
	 * {@link BadRealState}.
	 *
	 * @return
	 * O próxmo estado.
	 */
	@Override
	public State next(char c) {
		if(Character.isDigit(c))
			return FractionalState.getInstance();
		else
			return BadRealState.getInstance();
	}

	/**
	 * Acrescenta o caractere à cadeia, se for um dígito.
	 * Não altera a cadeia, caso contrário.
	 */
	@Override
	public void updateString(char c, StringBuilder sb) {
		if(Character.isDigit(c))
			sb.append(c);
		return;
	}

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
	 * Obtém uma instância desta classe.
	 *
	 * @return
	 * A instância desta classe.
	 */
	public static State getInstance() {
		return SINGLETON;
	}

	//oculta o construtor padrão
	private FirstFractionalState() {
	}

	private static final State SINGLETON = new FirstFractionalState();

}
