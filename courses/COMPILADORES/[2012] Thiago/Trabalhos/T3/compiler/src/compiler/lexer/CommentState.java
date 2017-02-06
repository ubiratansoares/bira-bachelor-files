package compiler.lexer;

import compiler.Token;
import compiler.LexicalException;

/**
 * Estado de comantário aberto mas não fechado.
 *
 */
public class CommentState extends State {

	/**
	 * Próximos estados:
	 * {@link StartState},
	 * {@code CommentState}.
	 *
	 * Não atualiza a cadeia.
	 */
	@Override
	public State next(char c) {
		if(c == '}')
			return StartState.getInstance(); //comentário eliminado
		else
			return this; //comentário restante, descartar
	}

	/**
	 * Se o comentário fechar, a cadeia será esvaziada.
	 * Não faz nada, caso contrário.
	 */
	@Override
	public void updateString(char c, StringBuilder sb) {
		if(c == '}')
			sb.delete(0, sb.length());
		return;
	}

	/**
	 * Lança um erro léxico.
	 *
	 * @return
	 * Não retorna.
	 *
	 * @throws LexicalException
	 * Lançada para indicar um comentário mal formado,
	 * isto é, um comentário aberto mas não fechado.
	 */
	@Override
	public Token getToken(String str) throws LexicalException {
		throw new LexicalException(str, "Comentário aberto, mas não fechado.");
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
	private CommentState() {
	}

	private static final State SINGLETON = new CommentState();

}
