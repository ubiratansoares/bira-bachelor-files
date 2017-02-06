package compiler.lexer;

import compiler.Token;
import compiler.LexicalException;

/**
 * Estado de comantário aberto mas não fechado. Primeira linha do comentário.
 *
 */
public class CommentStartState extends State {

	/**
	 * Próximos estados:
	 * {@link StartState},
	 * {@code CommentState}.
	 *
	 * Não atualiza a cadeia.
	 */
	@Override
	public State next(char c) {
		switch(c) {
			case '}': return StartState.getInstance(); //comentário eliminado
			case '\n': return CommentState.getInstance(); //primeiro fim de linha
			default: return this; //comentário restante, descartar
		}
	}

	/**
	 * Armazena o comentário até que o comentário seja fechado 
	 * adequadamente, ou até atingir o primeiro fim de linha.
	 * Quando o comentário fechar, a cadeia será esvaziada.
	 */
	@Override
	public void updateString(char c, StringBuilder sb) {
		switch(c) {
			case '}': sb.delete(0, sb.length());
			case '\n': break;
			default: sb.append(c);
		}
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
	private CommentStartState() {
	}

	private static final State SINGLETON = new CommentStartState();

}
