package tpw.agenda.database;

/**
 * Descreve uma exceção relacionada às operações com as bases de dados.
 */
public class DatabaseException extends Exception {

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
