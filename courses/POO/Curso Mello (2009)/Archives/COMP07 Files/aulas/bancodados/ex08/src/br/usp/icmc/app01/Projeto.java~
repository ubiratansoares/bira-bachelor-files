// Design Pattern -> persistencia
package br.usp.icmc.app01;

import java.sql.*;

public class Projeto {
	private int codigo;
	private String descricao;
	private Connection conn;

	public Projeto(Connection conn) { this.conn = conn; }
	public Projeto(Connection conn, int codigo, String descricao) {
		this.conn = conn;
		this.codigo = codigo;
		this.descricao = descricao;
	}

	public void setCodigo(int codigo) { this.codigo = codigo; }
	public void setDescricao(String descricao) { this.descricao = descricao; }

	public int getCodigo() { return this.codigo; }
	public String getDescricao() { return this.descricao; }

	public void insert() throws Exception {
		Statement stmt = conn.createStatement();
		stmt.executeUpdate("insert into projeto (codigo, descricao) values ("+this.codigo+", '"+this.descricao+"')");
		stmt.close();
	}

	public void remove() throws Exception {
		Statement stmt = conn.createStatement();
		stmt.executeUpdate("delete from projeto where codigo = "+this.codigo);
		stmt.close();
	}

	public void update() throws Exception {
		Statement stmt = conn.createStatement();
		stmt.executeUpdate("update projeto set descricao = '"+this.descricao+"' where codigo = "+this.codigo);
		stmt.close();
	}

	public static ResultSet selectAll(Connection conn) throws Exception {
		Statement stmt = conn.createStatement();
		ResultSet rs = 
			stmt.executeQuery("select codigo, descricao from projeto");
		//stmt.close(); -> fechar a janela de comandos

		return rs;
	}

	public static Projeto next(ResultSet rs, Connection conn) throws Exception {
		if (rs.next()) {
			Projeto p = new Projeto(conn);
			p.setCodigo(rs.getInt("codigo"));
			p.setDescricao(rs.getString("descricao"));
			return p;
		}

		return null;
	}
}
