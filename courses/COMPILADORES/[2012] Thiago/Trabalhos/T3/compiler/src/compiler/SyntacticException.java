package compiler;

/**
 * Indica um problema sintático no processo de compilação.
 *
 */
public class SyntacticException extends CompilerException {

	private Token token;

    /**
     * Creates a new instance of <code>SyntacticException</code> without detail message.
     */
    public SyntacticException() {
    }

    /**
     * Constructs an instance of <code>SyntacticException</code> with the specified detail message.
	 * @param t the token that caused the exception.
     * @param msg the detail message.
     */
    public SyntacticException(Token t, String msg) {
        super(msg);
		this.token = t;
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

}
