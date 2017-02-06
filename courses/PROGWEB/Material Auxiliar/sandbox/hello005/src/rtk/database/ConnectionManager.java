package rtk.database;

import java.io.*;
import java.sql.*;
import java.util.*;
import javax.xml.stream.*;

import static javax.xml.stream.XMLStreamConstants.*;

/**
 * Classe responsável por carregar e gerenciar conexões com a(s) 
 * base(s) de dados da aplicação.
 * 
 * @author Ricardo Takashi Kagawa
 */
public class ConnectionManager {
	
	/**
	 * Obtém uma nova conexão com a base de dados mapeada pela chave.
	 * 
	 * @param key
	 * A chave que identifica a conexão a ser criada.
	 * 
	 * @return 
	 * A conexão criada.
	 */
	public synchronized Connection getConnection(String key) throws DatabaseException {
		return connections.get(key).getConnection();
	}
	
	/**
	 * Obtém as chaves aceitas por este mapeamento.
	 * 
	 * @return
	 * O conjunto de chaves aceitas pelo gerenciador.
	 */
	public synchronized Set<String> getIdentifiers() {
		return connections.keySet();
	}
	
	/**
	 * Configura o gerenciador usando o arquivo xml dado.
	 * 
	 * @param configFile
	 * O nome do arquivo de configuração a ser usado.
	 */
	public synchronized void open(String configFile) throws 
			FileNotFoundException, IOException, XMLStreamException {
		if(valid) this.close();
		InputStream stream = new FileInputStream(configFile);
		try {
			//lê o arquivo xml
			ConfigurationParser parser = new ConfigurationParser();
			parser.parse(stream);
			Map<String, String> drivers = parser.getDrivers();
			Map<String, DatabaseConnection> conns = parser.getConnections();
			
			this.valid = true;
			
			//carregando drivers
			for(String name : drivers.values()) {
				try {
					Class.forName(name);
					messages.add(String.format("INFO: Loaded JDBC driver \"%s\".", name));
				} catch(ClassNotFoundException e) {
					messages.add(String.format("ERROR: Could not load JDBC driver \"%s\".", name));
				}
			}
			
			//registrando conexões
			this.connections = new HashMap<String, DatabaseConnection>(conns);
		} finally {
			stream.close();
		}
	}

	/**
	 * Libera todos os recursos gerenciados pelo gerenciador.
	 */
	public synchronized void close() {
		if(!valid) return;
		
		for(DatabaseConnection conn : connections.values()) {
			try {
				conn.close();
				messages.add(String.format("INFO: Closed connection \"%s\".", conn.getIdentifier()));
			} catch(DatabaseException e) {
				messages.add(String.format("ERROR: Failed to close connection \"%s\".", conn.getIdentifier()));
			}
		}
		connections.clear();
		
		Enumeration<Driver> drivers = DriverManager.getDrivers();
		while(drivers.hasMoreElements()) {
			Driver driver = drivers.nextElement();
			try {
				DriverManager.deregisterDriver(driver);
				messages.add(String.format("INFO: Unloaded driver \"%s\".", driver.getClass().getName()));
			} catch(SQLException e) {
				messages.add(String.format("ERROR: Failed to unload driver \"%s\".", driver.getClass().getName()));
			}
		}
		
		this.valid = false;
	}

	public Map<String, DatabaseConnection> connections;
	private boolean valid;
	
	@Override
	public void finalize() {
		if(this.valid) this.close();
	}
	
	/* ===== singleton ===== */
	
	private ConnectionManager() {
		DriverManager.setLoginTimeout(120); //2 min
		this.connections = new HashMap<String, DatabaseConnection>();
		this.valid = false;
	}
	
	private static ConnectionManager singleton = new ConnectionManager();
	
	public static ConnectionManager getInstance() {
		return singleton;
	}
	
	/* ===== test ===== */
	
	private List<String> messages = Collections.synchronizedList(new LinkedList<String>());
	
	public void addMessage(String message) {
		messages.add(message);
	}
	
	public List<String> getMessages() {
		return new LinkedList<String>(messages);
	}
	
	public void clearMessages() {
		messages.clear();
	}
	
	public static void main(String[] args) {
		try {
			ConnectionManager.getInstance().open("cfg/db-config.xml");
			System.out.println("Connection manager is configured.");
			ConnectionManager.getInstance().close();
			
			for(String msg : ConnectionManager.getInstance().getMessages())
				System.out.println(msg);
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	
}
