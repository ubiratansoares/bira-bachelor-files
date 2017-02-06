package compiler.lexer;

import compiler.LexicalException;

/**
 * Estado do autômato finito do analisador léxico.
 *
 */
public abstract class State {

	/**
	 * Obtém o próximo estado dada uma entrada.
	 *
	 * @param c
	 * A entrada recebida pelo autômato no momento.
	 *
	 * @return
	 * O estado que sucede este, dada a entrada; ou
	 * {@code null} caso o autômato deva parar.
	 */
	public abstract State next(char c);

	/**
	 * Atualiza a cadeia lida pelo autômato.
	 *
	 * @param c
	 * A próxima entrada que o autômato receberá.
	 *
	 * @param sb
	 * A cadeia a ser atualizada.
	 */
	public abstract void updateString(char c, StringBuilder sb);

	/**
	 * Obtém o token identificado pelo autômato.
	 *
	 * @param str
	 * A cadeia do token.
	 *
	 * @return
	 * O token identificado pelo autômato.
	 *
	 * @throws LexicalException
	 * Lançada para indicar erro léxico.
	 */
	public abstract compiler.Token getToken(String str)
			throws LexicalException;

	/**
	 * Indica que o caracter lido anteriormente deve
	 * ser devolvido ao buffer.
	 *
	 * @return
	 * {@code true} caso um caracter deva ser devolvido.
	 * A implementação padrão retorna {@code false}.
	 */
	public boolean hasForwardLooked() {
		return false;
	}

}
