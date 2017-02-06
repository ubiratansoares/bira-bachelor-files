package tpw.agenda.model;

import tpw.agenda.database.*;

import java.sql.*;
import java.util.*;

/**
 * Representa as informações de autenticação de um usuário.
 */
public class Privilege implements DatabaseObject<Privilege> {
	
	private Integer id = null;
	private String description = null;
	
	private Connection conn = null;
	
	//construtores
	
	public Privilege() {
	}
	
	public Privilege(Connection conn) {
		this.conn = conn;
	}
	
	//setters e getters
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	public void setId(String id) {
		try {
			this.id = Integer.valueOf(id);
		} catch(NumberFormatException e) {
			this.id = null;
		}
	}
	
	public void setDescription(String desc) {
		this.description = null;
		if(desc != null && !desc.trim().isEmpty())
			this.description = desc.trim();
	}
	
	public Integer getId() {
		return this.id;
	}
	
	public String getDescription() {
		return this.description;
	}
	
	//other methods
	
	@Override
	public boolean equals(Object o) {
		if(o == null) {
			return false;
		} else if(o instanceof String) {
			String str = (String) o;
			return str.trim().equals(this.getDescription());
		} else if(o instanceof Privilege) {
			Privilege p = (Privilege) o;
			if(this.id != null && p.id != null)
				return (p.id == this.id);
			else
				return (this.description == null)? 
					(p.description == null): 
					this.description.equals(p.description);
		} else
			return false;
	}
	
	//persistence
	
	public void setConnection(Connection conn) {
		this.conn = conn;
	}
	
	public void reviveKeys(ResultSet cursor) throws DatabaseException {
		try {
			this.id = cursor.getInt("id");
			if(cursor.wasNull())
				this.id = null;
		} catch(SQLException e) {
			throw new DatabaseException();
		}
	}
	
	public void revive(ResultSet cursor) throws DatabaseException {
		try {
			this.reviveKeys(cursor);
			
			this.description = cursor.getString("description");
			if(this.description != null)
				this.description = this.description.trim();
		} catch(SQLException e) {
			throw new DatabaseException();
		}
	}
	
	public void add() throws DatabaseException {
		if(this.description == null)
			throw new DatabaseException("Description cannot be null nor empty.");
		
		String query = String.format(
			"INSERT INTO privilege (description) " + 
			"VALUES ('%s')", this.description);
		DatabaseUtilities.executeInsert(this.conn, query, this, "id");
	}
	
	public boolean remove() throws DatabaseException {
		if(this.id == null)
			throw new DatabaseException("Need an id for this operation.");
		
		String query = String.format(
			"DELETE FROM privilege WHERE id = %d", this.id);
		return DatabaseUtilities.executeUpdate(this.conn, query);
	}
	
	public boolean get() throws DatabaseException {
		if(this.id == null && this.description == null)
			throw new DatabaseException("Need an id or a description for this operation.");
		
		String filter = (this.description != null)?
			String.format("description = '%s'", this.description):
			String.format("id = %d", this.id);
		
		String query = String.format("SELECT * FROM privilege WHERE %s", filter);
		return DatabaseUtilities.getSingleRecord(this.conn, query, this);
	}
	
	public boolean set() throws DatabaseException {
		if(this.id == null)
			throw new DatabaseException("Need an id for this operation.");
		
		String query = String.format(
				"UPDATE privilege SET description = '%s' WHERE id = %d", 
				this.description, this.id);
		return DatabaseUtilities.executeUpdate(this.conn, query);
	}
	
	//data sets
	
	public Cursor list() throws DatabaseException {
		String query = String.format("SELECT * FROM privilege ORDER BY description");
		ResultSet cursor = DatabaseUtilities.executeQuery(this.conn, query);
		return new Cursor(cursor);
	}
	
	/**
	 * Lista todas as permissões associadas a uma conta. Requer o
	 * identificador da conta.
	 */
	public Cursor list(Account acc) throws DatabaseException {
		if(acc.getId() == null)
			throw new DatabaseException("An account ID is required for this operation.");
		
		String query = String.format("SELECT p.* FROM privilege p " +
			"JOIN account_privilege ap ON (p.id = ap.priv_id) " + 
			"WHERE ap.acc_id = %d", acc.getId());
		
		ResultSet cursor = DatabaseUtilities.executeQuery(this.conn, query);
		return new Cursor(cursor);
	}
	
	public class Cursor extends Privilege implements DatabaseObject.Cursor<Privilege> {
		
		private ResultSet cursor;
		
		public Cursor(ResultSet cursor) {
			if(cursor == null)
				throw new NullPointerException();
			
			this.cursor = cursor;
		}
		
		public boolean next() throws DatabaseException {
			try {
				if(cursor.next()) {
					this.revive(cursor);
					return true;
				} else {
					cursor.close();
					return false;
				}
			} catch(SQLException e) {
				throw new DatabaseException(e);
			}
		}
		
		public boolean fastForward(int n) throws DatabaseException {
			try {
				return (n >= 0)? cursor.relative(n): false;
			} catch(SQLException e) {
				throw new DatabaseException(e);
			}
		}
		
		public void close() throws DatabaseException {
			try {
				if(cursor.getStatement() != null)
					cursor.getStatement().close();
			} catch(SQLException e) {
				throw new DatabaseException(e);
			}
		}
	
		public java.util.Iterator<Privilege> iterator() {
			return new Iterator(this);
		}
		
		public List<Privilege> load() throws DatabaseException {
			List<Privilege> l = new LinkedList<Privilege>();
			for(Privilege a : this)
				l.add(a);
			return l;
		}
		
	}
	
	public class Iterator implements java.util.Iterator<Privilege> {
		
		private Cursor cursor;
		private Privilege nextItem;
		
		public Iterator(Cursor c) {
			if(c == null)
				throw new NullPointerException();
			
			try {
				this.cursor = c;
				this.nextItem = c.next()? (Privilege) c.clone(): null;
			} catch(DatabaseException e) {
				this.nextItem = null;
				this.close();
			} catch(CloneNotSupportedException e) {
				this.nextItem = null;
				this.close();
			}
		}
		
		@Override
		public boolean hasNext() {
			return (nextItem != null);
		}
		
		@Override
		public Privilege next() {
			Privilege r = nextItem;
			try {
				this.nextItem = cursor.next()? (Privilege) cursor.clone(): null;
			} catch(DatabaseException e) {
				this.nextItem = null;
				this.close();
			} catch(CloneNotSupportedException e) {
				this.nextItem = null;
				this.close();
			}
			return r;
		}
		
		@Override
		public void remove() {
			throw new UnsupportedOperationException();
		}
		
		public void close() {
			try {
				cursor.close();
			} catch(DatabaseException e) {
			}
			return;
		}
	
	}
	
}
