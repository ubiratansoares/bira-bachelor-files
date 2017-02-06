package compiler;

/**
 * Mensagem de erro do compilador.
 *
 */
public class CompilationError {

	private String message;
	private Token token;

    /**
     * Cria uma mensagem de erro do compilador.
	 *
	 * @param t
	 * O token que causou o erro.
	 *
     * @param msg
	 * A mensagem de erro.
     */
    public CompilationError(Token t, String msg) {
        this.message = msg;
		this.token = t;
    }

	/**
	 * Obtém a mensagem de erro.
	 *
	 * @return
	 * A mensagem de erro.
	 */
	public String getMessage() {
		return message;
	}

	/**
	 * Obtém o token que causou o erro.
	 *
	 * @return
	 * O token que causou o erro.
	 */
	public Token getToken() {
		return token;
	}

}
