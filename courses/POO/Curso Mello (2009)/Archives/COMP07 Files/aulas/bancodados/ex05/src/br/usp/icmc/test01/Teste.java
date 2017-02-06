package br.usp.icmc.test01;

import java.sql.*;

public class Teste {
	public static void main(String args[]) throws Exception {
		// Projeto -> codigo, descricao
		// Tarefas -> descricao, datainicio, datafim
		String driver = "org.postgresql.Driver";
		String url = "jdbc:postgresql://localhost:5432/test01db";
		String username = "testuser";
		String password = "testpass";
		
		Class.forName(driver);
		Connection conn = 
			DriverManager.getConnection(url, username, password);
		
		Statement stmt = conn.createStatement();
		stmt.executeUpdate("insert into projeto (codigo, descricao) values (1, 'abc')");
		ResultSet rs = stmt.executeQuery("select codigo, descricao from projeto");

		while (rs.next()) {
			System.out.println(rs.getInt("codigo")+", "+rs.getString("descricao"));
		}

		stmt.close();
		conn.close();
	}
}
