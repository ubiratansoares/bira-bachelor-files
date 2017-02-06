package rtk.database;

import java.sql.*;
import java.util.*;

public class ProxyConnection implements Connection {
	
	/* ===== base object, pooling ===== */
	
	private Connection conn = null;
	private DatabaseConnection pool = null;
	
	public ProxyConnection(DatabaseConnection pool, Connection conn) {
		this.conn = conn;
		this.pool = pool;
	}
	
	/* ===== intercepted interface methods ===== */
	
	@Override
	public void close() throws SQLException {
		if(conn == null) return;
		pool.close(this.conn);
		this.conn = null;
	}
	
	@Override
	public boolean isClosed() throws SQLException {
		return (this.conn == null) || conn.isClosed();
	}
	
	@Override
	public boolean isValid(int timeout) throws SQLException {
		return (conn != null) && conn.isValid(timeout);
	}
	
	@Override
	public void finalize() {
		try {
			this.close();
		} catch(SQLException e) {
			e.printStackTrace();
		}
	}
	
	@Override
	public <T> T unwrap(Class<T> iface) throws SQLException {
		if(iface.equals(Connection.class) || iface.equals(ProxyConnection.class))
			return iface.cast(this);
		else if(conn == null)
			throw new SQLException("This connection has been closed.");
		else
			return conn.unwrap(iface);
	}
	
	@Override
	public boolean isWrapperFor(Class<?> iface) throws SQLException {
		if(iface.equals(Connection.class) || iface.equals(ProxyConnection.class))
			return true;
		else if(conn == null)
			throw new SQLException("This connection has been closed.");
		else
			return conn.isWrapperFor(iface);
	}

	/* ===== delegated interface methods ===== */
	
	@Override
	public Statement createStatement() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.createStatement();
	}
	
	@Override
	public PreparedStatement prepareStatement(String sql) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.prepareStatement(sql);
	}
	
	@Override
	public CallableStatement prepareCall(String sql) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.prepareCall(sql);
	}
	
	@Override
	public String nativeSQL(String sql) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.nativeSQL(sql);
	}
	
	@Override
	public void setAutoCommit(boolean autoCommit) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		conn.setAutoCommit(autoCommit);
	}
	
	@Override
	public boolean getAutoCommit() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.getAutoCommit();
	}
	
	@Override
	public void commit() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		conn.commit();
	}
	
	@Override
	public void rollback() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		conn.rollback();
	}
	
	@Override
	public DatabaseMetaData getMetaData() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.getMetaData();
	}
	
	@Override
	public void setReadOnly(boolean readOnly) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		conn.setReadOnly(readOnly);
	}
	
	@Override
	public boolean isReadOnly() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.isReadOnly();
	}
	
	@Override
	public void setCatalog(String catalog) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		conn.setCatalog(catalog);
	}
	
	@Override
	public String getCatalog() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.getCatalog();
	}
	
	@Override
	public void setTransactionIsolation(int level) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		conn.setTransactionIsolation(level);
	}
	
	@Override
	public int getTransactionIsolation() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.getTransactionIsolation();
	}
	
	@Override
	public SQLWarning getWarnings() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.getWarnings();
	}
	
	@Override
	public void clearWarnings() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		conn.clearWarnings();
	}
	
	@Override
	public Statement createStatement(int resultSetType, int resultSetConcurrency) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.createStatement(resultSetType, resultSetConcurrency);
	}
	
	@Override
	public PreparedStatement prepareStatement(String sql, int resultSetType, int resultSetConcurrency) 
			throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.prepareStatement(sql, resultSetType, resultSetConcurrency);
	}
	
	@Override
	public CallableStatement prepareCall(String sql, int resultSetType, int resultSetConcurrency) 
			throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.prepareCall(sql, resultSetType, resultSetConcurrency);
	}
	
	@Override
	public Map<String,Class<?>> getTypeMap() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.getTypeMap();
	}
	
	@Override
	public void setTypeMap(Map<String,Class<?>> map) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		conn.setTypeMap(map);
	}
	
	@Override
	public void setHoldability(int holdability) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		conn.setHoldability(holdability);
	}
	
	@Override
	public int getHoldability() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.getHoldability();
	}
	
	@Override
	public Savepoint setSavepoint() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.setSavepoint();
	}
	
	@Override
	public Savepoint setSavepoint(String name) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.setSavepoint(name);
	}
	
	@Override
	public void rollback(Savepoint savepoint) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		conn.rollback(savepoint);
	}
	
	@Override
	public void releaseSavepoint(Savepoint savepoint) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		conn.releaseSavepoint(savepoint);
	}
	
	@Override
	public Statement createStatement(int resultSetType, int resultSetConcurrency, 
			int resultSetHoldability) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.createStatement(resultSetType, resultSetConcurrency, resultSetHoldability);
	}
	
	@Override
	public PreparedStatement prepareStatement(String sql, int resultSetType, int resultSetConcurrency,
			int resultSetHoldability) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.prepareStatement(sql, resultSetType, resultSetConcurrency, resultSetHoldability);
	}
	
	@Override
	public CallableStatement prepareCall(String sql, int resultSetType, int resultSetConcurrency,
			int resultSetHoldability) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.prepareCall(sql, resultSetType, resultSetConcurrency, resultSetHoldability);
	}
	
	@Override
	public PreparedStatement prepareStatement(String sql, int autoGeneratedKeys) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.prepareStatement(sql, autoGeneratedKeys);
	}

	@Override
	public PreparedStatement prepareStatement(String sql, int[] columnIndexes) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.prepareStatement(sql, columnIndexes);
	}
	
	@Override
	public PreparedStatement prepareStatement(String sql, String[] columnNames) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.prepareStatement(sql, columnNames);
	}
	
	@Override
	public Clob createClob() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.createClob();
	}
	
	@Override
	public Blob createBlob() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.createBlob();
	}
	
	@Override
	public NClob createNClob() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.createNClob();
	}
	
	@Override
	public SQLXML createSQLXML() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.createSQLXML();
	}
	
	@Override
	public void setClientInfo(String name, String value) throws SQLClientInfoException {
		if(conn == null)
			throw new SQLClientInfoException();
		conn.setClientInfo(name, value);
	}
	
	@Override
	public void setClientInfo(Properties properties) throws SQLClientInfoException {
		if(conn == null)
			throw new SQLClientInfoException();
		conn.setClientInfo(properties);
	}

	@Override
	public String getClientInfo(String name) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.getClientInfo(name);
	}
	
	@Override
	public Properties getClientInfo() throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.getClientInfo();
	}
	
	@Override
	public Array createArrayOf(String typeName, Object[] elements) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.createArrayOf(typeName, elements);
	}
	
	@Override
	public Struct createStruct(String typeName, Object[] attributes) throws SQLException {
		if(conn == null)
			throw new SQLException("This connection has been closed.");
		return conn.createStruct(typeName, attributes);
	}
	
}
