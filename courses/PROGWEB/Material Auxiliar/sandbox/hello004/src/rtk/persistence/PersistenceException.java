package rtk.persistence;

/**
 * Descreve uma exceção relacionada às operações de persistência.
 * 
 * @author Ricardo Takashi Kagawa
 */
public class PersistenceException extends java.io.IOException {

	private static final long serialVersionUID = 1L;
	
	public PersistenceException() {
	}
	
	public PersistenceException(String message) {
		super(message);
	}
	
	public PersistenceException(String message, Throwable cause) {
		super(message, cause);
	}
	
	public PersistenceException(Throwable cause) {
		super(cause);
	}
	
}
