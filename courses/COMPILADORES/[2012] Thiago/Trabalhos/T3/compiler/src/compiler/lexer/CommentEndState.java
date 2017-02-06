package compiler.lexer;

import compiler.LexicalException;

/**
 * Estado de comentário fechado mas não aberto.
 *
 */
public class CommentEndState extends StopState {

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
	 * Lançada para indicar um comentário mal formado,
	 * isto é, um comentário fechado mas não aberto.
	 */
	@Override
	public compiler.Token getToken(String str) throws LexicalException {
		throw new LexicalException(str, "Comentário fechado, mas não aberto.");
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
	private CommentEndState() {
	}

	private static final State SINGLETON = new CommentEndState();

}
