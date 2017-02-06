package rtk.database;

import java.lang.reflect.*;
import java.sql.*;
import java.util.*;

/**
 * Interface para manipular objetos que podem ser persistidos
 * em uma base de dados.
 */
public interface DatabaseObject {
	
	/**
	 * Insere o objeto na base de dados.
	 */
	public void add(Connection conn) throws DatabaseException;
	
	/**
	 * Remove o objeto da base de dados.
	 */
	public boolean remove(Connection conn) throws DatabaseException;
	
	/**
	 * Obtém o objeto da base de dados.
	 */
	public boolean get(Connection conn) throws DatabaseException;
	
	/**
	 * Atualiza o objeto na base de dados.
	 */
	public boolean set(Connection conn) throws DatabaseException;
	
}
