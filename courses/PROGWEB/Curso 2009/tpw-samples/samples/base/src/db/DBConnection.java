package db;

import java.sql.*;

public class DBConnection {

	Connection connection = null;

	public DBConnection(String driver, String url,
			String username, String password) 
		throws Exception {
		Class.forName(driver);
		connection = DriverManager.getConnection(url, username, password);
	}

	public ResultSet query(String query) throws Exception {
		Statement statement = connection.createStatement();
		return statement.executeQuery(query); // Query (SELECT)
	}

	public int update(String sql) throws Exception { 
		// Atualizacao (INSERT, DELETE, DROP TABLE, CREATE TABLE ... )
		Statement statement = connection.createStatement();
		int result = statement.executeUpdate(sql);
		statement.close();
		return result;
	}

	public void close() throws Exception {
		connection.close();
	}
}
