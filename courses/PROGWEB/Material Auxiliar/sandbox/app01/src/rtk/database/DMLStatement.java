package rtk.database;

import java.sql.*;

public class DMLStatement extends SQLStatement {
	
	/**
	 * Cria um {@link java.sql.PreparedStatement} que espera o retorno
	 * de algumas colunas cujos valores foram gerados por DEFAULT.
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
		return conn.prepareStatement(sql, keys);
	}
	
	/**
	 * Obtém o cursor com os valores gerados por DEFAULT.
	 * 
	 * @param st
	 * O {@link java.sql.PreparedStatement} que gerou o resultado. Ele
	 * já foi executado e está pronto para retornar um cursor (caso 
	 * haja algum para retornar).
	 */
	protected ResultSet getResults(PreparedStatement st)
			throws SQLException {
		return st.getGeneratedKeys();
	}
	
	/**
	 * Cria um comando vazio.
	 */
	public DMLStatement() {
		super();
	}
	
	/**
	 * Cria um comando vazio.
	 */
	public SQLStatement createClone() {
		return new DMLStatement();
	}
	
}
