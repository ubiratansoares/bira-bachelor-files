package compiler;

/**
 * Indica um problema no processo de compilação.
 *
 */
public class CompilerException extends Exception {

    /**
     * Creates a new instance of <code>CompilerException</code> without detail message.
     */
    public CompilerException() {
    }


    /**
     * Constructs an instance of <code>CompilerException</code> with the specified detail message.
     * @param msg the detail message.
     */
    public CompilerException(String msg) {
        super(msg);
    }

    /**
     * Constructs an instance of <code>CompilerException</code> with the specified cause.
     * @param e the cause.
     */
    public CompilerException(Throwable e) {
        super(e);
    }

}
