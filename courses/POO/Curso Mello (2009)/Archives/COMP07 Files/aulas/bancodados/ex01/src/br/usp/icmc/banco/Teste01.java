package br.usp.icmc.banco;

import java.sql.*;

// Connection
// Statement
// ResultSet

public class Teste01 {
	public static void main(String args[]) throws Exception {
		Class.forName("org.postgresql.Driver");
		String url = "jdbc:postgresql://localhost:5432/testdb01";
		String username = "testuser";
		String password = "testpass";
		Connection conn = 
		    DriverManager.getConnection(url, username, password);

		// conexao aberta com o banco de dados
		Statement stmt = conn.createStatement();
		int ret = stmt.executeUpdate("insert into _user (codigo, nome, login, senha) values (3, 'Paulo', 'paulo', 'gfdgfg')");
		stmt.close();

		conn.close();
	}
}
