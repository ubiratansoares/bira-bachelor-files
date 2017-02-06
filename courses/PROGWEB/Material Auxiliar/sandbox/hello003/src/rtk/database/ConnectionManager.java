package rtk.database;

import java.sql.*;
import java.util.*;

/**
 * Classe responsável por carregar e gerenciar conexões com a(s) 
 * base(s) de dados da aplicação.
 * 
 * @author Ricardo Takashi Kagawa
 */
public class ConnectionManager {
	
	private Map<String, ConnectionManager.Entry> connections;
	
	/**
	 * Obtém uma nova conexão com a base de dados mapeada pela chave.
	 * 
	 * @param key
	 * A chave que identifica a conexão a ser criada.
	 * 
	 * @return 
	 * A conexão criada.
	 */
	public static Connection getConnection(String key) throws DatabaseException {
		return singleton.connections.get(key).getConnection();
	}
	
	/**
	 * Obtém as chaves aceitas por este mapeamento.
	 * 
	 * @return
	 * O conjunto de chaves aceitas pelo gerenciador.
	 */
	public static Set<String> getKeys() {
		return singleton.connections.keySet();
	}

	//singleton
	
	private ConnectionManager() {
		this.connections = new Hashtable<String, ConnectionManager.Entry>(2, 1.0f);
		DriverManager.setLoginTimeout(600); //10 min
		
		try {
			//carregando drivers
			Class.forName("org.postgresql.Driver");
			
			//listando conexões disponíveis
			connections.put("main", new ConnectionManager.Entry(
					"jdbc:postgresql://localhost:5432/sandbox", "appuser", "499u53r"));
		} catch(ClassNotFoundException e) {
			throw new ExceptionInInitializerError("Could not load a JDBC driver class.");
		}
	}
	
	private static ConnectionManager singleton = new ConnectionManager();
	
	private class Entry {
		private String url;
		private String username;
		private String password;
		
		public Entry(String url, String username, String password) {
			this.url = url;
			this.username = username;
			this.password = password;
		}
		
		public Connection getConnection() throws DatabaseException {
			try {
				return DriverManager.getConnection(this.url, this.username, this.password);
			} catch(SQLException e) {
				throw new DatabaseException(e);
			}
		}
		
	}
	
}
