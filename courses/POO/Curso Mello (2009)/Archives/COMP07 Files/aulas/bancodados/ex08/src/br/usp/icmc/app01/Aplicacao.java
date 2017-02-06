package br.usp.icmc.app01;

import java.sql.*;

public class Aplicacao implements Config {
	public static void main(String args[]) throws Exception {
		Connection conn = 
			DriverManager.getConnection(url, username, password);

		for (int i = 0; i < 10; i++) {
			Projeto p = new Projeto(conn, i, "teste");
			p.insert();
		}

		ResultSet rs = Projeto.selectAll(conn);
		Projeto p = null;
		
		while ((p = Projeto.next(rs, conn)) != null) {
			//p.setDescricao("aaaaaaaaaaaaaaa");
			//p.update();
			System.out.println(p.getCodigo()+", "+p.getDescricao());
		}

		/*
		while (rs.next()) {
			System.out.println(rs.getInt("codigo")+", "+
					rs.getString("descricao"));
		}*/

		conn.close();
	}
}
