package compiler.lexer;

import compiler.Token;

/**
 * Estado de identificador parcialmente identificado.
 *
 */
public class IdentifierState extends State {

	/**
	 * Próximos estados: 
	 * {@code IdentifierState},
	 * {@link IdentifierEndState}.
	 */
	@Override
	public State next(char c) {
		if(Character.isLetterOrDigit(c))
			return this;
		else
			return IdentifierEndState.getInstance();
	}

	/**
	 * Acrescenta o caractere à cadeia, se for uma letra ou um dígito.
	 * Não altera a cadeia, caso contrário.
	 */
	@Override
	public void updateString(char c, StringBuilder sb) {
		if(Character.isLetterOrDigit(c))
			sb.append(c);
		return;
	}

	/**
	 * Retorna um identificador. Não é verificado se o identificador
	 * é na realidade uma palavra reservada.
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
	private IdentifierState() {
	}

	private static final State SINGLETON = new IdentifierState();

}
