package tpw.agenda.database;

import java.sql.*;

public class DatabaseUtilities {
	
	public static boolean executeUpdate(Connection conn, String query) 
			throws DatabaseException {
		Statement st = null;
		try {
			st = conn.createStatement();
			int r = st.executeUpdate(query);
			return (r == 1);
		} catch(SQLException e) {
			throw new DatabaseException(e);
		} finally {
			try {
				if(st != null) st.close();
			} catch(SQLException e2) {
				throw new DatabaseException(e2);
			}
		}
	}
	
	public static ResultSet executeQuery(Connection conn, String query) 
			throws DatabaseException {
		try {
			Statement st = conn.createStatement();
			return st.executeQuery(query);
		} catch(SQLException e) {
			throw new DatabaseException(e);
		}
	}
	
	public static boolean getSingleRecord(Connection conn, String query, DatabaseObject base) 
			throws DatabaseException {
		Statement st = null;
		try {
			st = conn.createStatement();
			ResultSet cursor = st.executeQuery(query);
			boolean found = cursor.next();
			if(found) base.revive(cursor);
			return found;
		} catch(SQLException e) {
			throw new DatabaseException(e);
		} finally {
			try {
				if(st != null) st.close();
			} catch(SQLException e2) {
				throw new DatabaseException(e2);
			}
		}
	}
	
	public static void executeInsert(Connection conn, String query, DatabaseObject base, String... keys) 
			throws DatabaseException {
		Statement st = null;
		try {
			st = conn.createStatement();
			st.executeUpdate(query, keys);
			ResultSet cursor = st.getGeneratedKeys();
			if(cursor.next())
				base.reviveKeys(cursor);
		} catch(SQLException e) {
			throw new DatabaseException(e);
		} finally {
			try {
				if(st != null) st.close();
			} catch(SQLException e2) {
				throw new DatabaseException(e2);
			}
		}
	}
	
}
