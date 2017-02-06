package rtk.persistence.database;

/**
 * Descreve uma exceção relacionada às operações com as bases de dados.
 * 
 * @author Ricardo Takashi Kagawa
 */
public class DatabaseException extends rtk.persistence.PersistenceException {

	private static final long serialVersionUID = 1L;
	
	public DatabaseException() {
		super("A database error has occurred.");
	}
	
	public DatabaseException(String message) {
		super(message);
	}
	
	public DatabaseException(String message, Throwable cause) {
		super(message, cause);
	}
	
	public DatabaseException(Throwable cause) {
		super(cause);
	}
	
}
