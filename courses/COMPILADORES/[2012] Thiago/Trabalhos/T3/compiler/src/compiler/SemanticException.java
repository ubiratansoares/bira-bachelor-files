package compiler;

/**
 * Indica um problema semântico no processo de compilação.
 *
 */
public class SemanticException extends CompilerException {

    /**
     * Creates a new instance of <code>SemanticException</code> without detail message.
     */
    public SemanticException() {
    }

    /**
     * Constructs an instance of <code>SemanticException</code> with the specified detail message.
     * @param msg the detail message.
     */
    public SemanticException(String msg) {
        super(msg);
    }

}
