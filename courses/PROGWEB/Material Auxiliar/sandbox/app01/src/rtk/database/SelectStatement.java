package rtk.database;

import java.sql.*;

public class SelectStatement extends SQLStatement {
	
	/**
	 * Cria um {@link java.sql.PreparedStatement} que ignora a
	 * lista de colunas DEFAULT.
	 * 
	 * @param conn
	 * A conexão com a base de dados envolvida na operação.
	 * 
	 * @param sql
	 * O comando SQL que será criado.
	 * 
	 * @param keys
	 * Os nomes das colunas cujos valores serão gerados pelo comando.
	 */
	protected PreparedStatement createStatement(Connection conn, 
			String sql, String[] keys) throws SQLException {
		return conn.prepareStatement(sql);
	}
	
	/**
	 * Obtém o cursor com os resultados da consulta.
	 * 
	 * @param st
	 * O {@link java.sql.PreparedStatement} que gerou o resultado. Ele
	 * já foi executado e está pronto para retornar um cursor (caso 
	 * haja algum para retornar).
	 */
	protected ResultSet getResults(PreparedStatement st) 
			throws SQLException {
		return st.getResultSet();
	}
	
	/**
	 * Cria um comando vazio.
	 */
	public SelectStatement() {
		super();
	}
	
	/**
	 * Cria um comando vazio.
	 */
	public SQLStatement createClone() {
		return new SelectStatement();
	}
	
}
