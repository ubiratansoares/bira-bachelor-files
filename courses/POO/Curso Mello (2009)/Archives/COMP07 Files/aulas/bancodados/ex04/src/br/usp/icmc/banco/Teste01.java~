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

		for (int i = 0; i < 10; i++) {
			stmt.executeUpdate("insert into _user (codigo, nome, login, senha) values ("+i+", 'nome"+i+"', 'login"+i+"', 'senha"+i+"')");
		}

		ResultSet rs = stmt.executeQuery("select codigo, nome, login, senha from _user");

		while (rs.next()) {
			System.out.println(rs.getInt("codigo")+", "+
				rs.getString("nome")+", "+
				rs.getString("login")+", "+
				rs.getString("senha"));
		}

		stmt.executeUpdate("delete from _user");

		stmt.close();

		conn.close();
	}
}
