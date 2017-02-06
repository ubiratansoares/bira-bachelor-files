package compiler;

/**
 * Indica que a leitura do código-fonte atingiu o fim de arquivo.
 *
 */
public class EOFException extends RuntimeException {

    /**
     * Creates a new instance of <code>EOFException</code> without detail message.
     */
    public EOFException() {
    }


    /**
     * Constructs an instance of <code>EOFException</code> with the specified detail message.
     * @param msg the detail message.
     */
    public EOFException(String msg) {
        super(msg);
    }
}
