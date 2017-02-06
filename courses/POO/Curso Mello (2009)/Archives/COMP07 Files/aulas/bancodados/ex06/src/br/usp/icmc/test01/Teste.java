package br.usp.icmc.test01;

import java.sql.*;

public class Teste implements Config {
	public static void main(String args[]) throws Exception {
		// Projeto -> codigo, descricao
		// Tarefas -> descricao, datainicio, datafim
	
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
