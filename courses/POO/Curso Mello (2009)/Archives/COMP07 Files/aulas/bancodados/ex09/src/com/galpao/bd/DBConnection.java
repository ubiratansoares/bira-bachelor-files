package com.galpao.bd;

import java.sql.*;

public class DBConnection implements Config {
	private Connection conn;

	public DBConnection() throws Exception {
		Class.forName(driver);
		conn = DriverManager.getConnection(url, username, password);
	}

	public int execute(String sql) throws Exception {
		Statement stmt = conn.createStatement();
		int ret = stmt.executeUpdate(sql); // insert, update, delete
		stmt.close();
		return ret;
	}

	public ResultSet query(String sql) throws Exception {
		Statement stmt = conn.createStatement();
		return stmt.executeQuery(sql); // select
	}

	public void close() throws Exception {
		conn.close();
	}
}
