package compiler;

/**
 * Lançada para indicar erro na análise léxica.
 *
 */
public class LexicalException extends CompilerException {

	private Token token;
	
	/**
	 * Cria uma exceção de análise léxica. A exceção contém
	 * a cadeia que causou a exceção, e uma mensagem explicativa.
	 * 
	 * @param badToken
	 * A cadeia que causou a exceção.
	 *
	 * @param message
	 * A mensagem explicativa.
	 */
	public LexicalException(String badToken, String message) {
		super(message);
		this.token = Token.createError(badToken);
	}

	/**
	 * Obtém o token que causou a exceção.
	 * 
	 * @return
	 * O token que causou a exceção.
	 */
	public Token getToken() {
		return token;
	}

	/**
	 * Define o número da linha e o número da coluna onde ocorre o token.
	 *
	 * @param line
	 * O número da linha onde ocorre o token.
	 *
	 * @param column
	 * O número da coluna onde ocorre o token.
	 */
	public void setPosition(int line, int column) {
		token.setPosition(line, column);
		return;
	}

}
