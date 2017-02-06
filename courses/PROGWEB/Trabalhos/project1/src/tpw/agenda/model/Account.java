package tpw.agenda.model;

import tpw.agenda.database.*;

import java.sql.*;
import java.text.*;
import java.util.*;

/**
 * Representa as informações de autenticação de um usuário.
 */
public class Account implements DatabaseObject<Account> {
	
	private Integer id = null;
	private String email = null;
	private String password = null;
	private Map<String, Privilege> privileges = new HashMap<String, Privilege>();
	
	private boolean updatePassword = true;
	private boolean updatePrivileges = true;
	private boolean reloadPrivileges = false;
	
	private java.util.Date createdOn = null;
	private java.util.Date lastVisit = null;
	private String firstName = null;
	private String lastName = null;
	
	private Connection conn = null;
	
	//construtores
	
	public Account() {
	}
	
	public Account(Connection conn) {
		this.conn = conn;
	}
	
	//setters e getters
	
	public void setId(Integer id) {
		this.id = id;
		this.reloadPrivileges = true;
		this.updatePrivileges = true;
	}
	
	public void setId(String id) {
		try {
			this.id = Integer.valueOf(id);
		} catch(NumberFormatException e) {
			this.id = null;
		}
		this.reloadPrivileges = true;
		this.updatePrivileges = true;
	}
	
	public void setEmail(String email) {
		this.email = null;
		if(email != null && !email.trim().isEmpty())
			this.email = email.trim();
		this.reloadPrivileges = true;
		this.updatePrivileges = true;
	}
	
	public void setPassword(String password) {
		if(password != null && !password.trim().isEmpty()) {
			this.password = password.trim();
			this.updatePassword = true;
		} else {
			this.password = null;
			this.updatePassword = false;
		}
	}
	
	public void setFirstName(String name) {
		this.firstName = null;
		if(name != null && !name.trim().isEmpty())
			this.firstName = name;
	}
	
	public void setLastName(String name) {
		this.lastName = null;
		if(name != null && !name.trim().isEmpty())
			this.lastName = name;
	}
	
	public Integer getId() {
		return this.id;
	}
	
	public String getEmail() {
		return this.email;
	}
	
	public String getPassword() {
		return this.password;
	}
	
	public java.util.Date getCreationTime() {
		return this.createdOn;
	}
	
	public java.util.Date getLastVisitTime() {
		return this.lastVisit;
	}
	
	public String getFirstName() {
		return this.firstName;
	}
	
	public String getLastName() {
		return this.lastName;
	}
	
	public void addPrivilege(String p) throws DatabaseException {
		Privilege priv = new Privilege();
		priv.setDescription(p);
		this.privileges.put(p, priv);
		this.updatePrivileges = true;
		this.reloadPrivileges = true;
	}
	
	public void removePrivilege(String p) throws DatabaseException {
		Privilege priv = new Privilege();
		priv.setDescription(p);
		this.privileges.remove(priv.getDescription());
		this.updatePrivileges = true;
		this.reloadPrivileges = true;
	}
	
	public Set<Privilege> getPrivileges() throws DatabaseException {
		if(this.reloadPrivileges) {
			this.privileges.clear();
			for(Privilege p : new Privilege(this.conn).list(this))
				this.privileges.put(p.getDescription(), p);
		}
		this.updatePrivileges = false;
		this.reloadPrivileges = false;
		return new HashSet<Privilege>(this.privileges.values());
	}
	
	//other methods
	
	public boolean authenticate() throws DatabaseException {
		if(this.email == null) return false;
		if(this.password == null) return false;
		
		String query = String.format("SELECT id FROM account a " +
			"WHERE email = '%s' AND password = md5('%s')", 
			this.email, this.password);
		if(DatabaseUtilities.getSingleRecord(this.conn, query, this)) {
			query = String.format("UPDATE account_info " +
				"SET last_visit = CURRENT_TIMESTAMP WHERE id = %d", 
				this.id);
			DatabaseUtilities.executeUpdate(this.conn, query);
			return this.get();
		} else
			return false;
	}
	
	public boolean hasPrivilege(String desc) throws DatabaseException {
		if(this.reloadPrivileges && this.id != null) this.getPrivileges();
		return this.privileges.containsKey(desc);
	}
	
	@Override
	public String toString() {
		return String.format("%d: u? %s, p=%s", this.id, this.updatePrivileges, this.privileges);
	}
	
	//persistence
	
	public void setConnection(Connection conn) {
		this.conn = conn;
	}
	
	public void revive(ResultSet cursor) throws DatabaseException {
		try {
			this.reviveKeys(cursor);
			
			this.privileges.clear();
			this.updatePrivileges = false;
			this.reloadPrivileges = true;
			
			if(cursor.getMetaData().getColumnCount() > 1) {
				this.email = cursor.getString("email");
				if(this.email != null)
					this.email = this.email.trim();
				
				this.password = cursor.getString("password");
				if(this.password != null)
					this.password = this.password.trim();
				this.updatePassword = false;
			}
			
			if(cursor.getMetaData().getColumnCount() > 3) {
				this.createdOn = cursor.getTimestamp("created_on");
				this.lastVisit = cursor.getTimestamp("last_visit");
				
				this.firstName = cursor.getString("first_name");
				if(this.firstName != null && this.firstName.trim().isEmpty())
					this.firstName = null;
				
				this.lastName = cursor.getString("last_name");
				if(this.lastName != null && this.lastName.trim().isEmpty())
					this.lastName = null;
			}
		} catch(SQLException e) {
			throw new DatabaseException();
		}
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
	
	public void add() throws DatabaseException {
		if(this.email == null)
			throw new DatabaseException("E-mail cannot be null nor empty.");
		if(this.password == null)
			throw new DatabaseException("Password cannot be null nor empty.");
		if(this.firstName == null)
			throw new DatabaseException("First name cannot be null nor empty.");
		
		try {
			this.conn.setTransactionIsolation(Connection.TRANSACTION_READ_COMMITTED);
			this.conn.setAutoCommit(false);
			
			String query = String.format(
				"INSERT INTO account (email, password) " + 
				"VALUES ('%s', md5('%s'))", this.email, this.password);
			DatabaseUtilities.executeInsert(this.conn, query, this, "id");
			
			if(this.id == null)
				throw new DatabaseException("Could not retrieve generated account id.");
				
			String surname = (this.lastName == null)? "NULL": "'" + this.lastName + "'";
			query = String.format(
				"INSERT INTO account_info (id, first_name, last_name) " + 
				"VALUES (%d, '%s', %s)", this.id, this.firstName, surname);
			DatabaseUtilities.executeInsert(this.conn, query, this);
			
			if(this.updatePrivileges) {
				for(Privilege p : this.privileges.values()) {
					p.setConnection(this.conn);
					if((p.getId() != null) || (p.getId() == null && p.get())) {
						query = String.format(
							"INSERT INTO account_privilege (acc_id, priv_id) " + 
							"VALUES (%d, %d)", this.id, p.getId());
						DatabaseUtilities.executeInsert(this.conn, query, this);
					}
				}
			}
			this.conn.commit();
			this.updatePassword = false;
			this.updatePrivileges = false;
		} catch(SQLException sqle) {
			try {
				this.conn.rollback();
			} catch(SQLException e) {
			}
			throw new DatabaseException(sqle);
		} catch(DatabaseException dbe) {
			try {
				this.conn.rollback();
			} catch(SQLException e) {
			}
			throw dbe;
		} finally {
			try {
				this.conn.setAutoCommit(true);
			} catch(SQLException e) {
				throw new DatabaseException(e);
			}
		}
	}
	
	public boolean remove() throws DatabaseException {
		if(this.id == null)
			throw new DatabaseException("Need an id for this operation.");
		
		this.updatePassword = true;
		this.updatePrivileges = true;
		this.reloadPrivileges = false;
		
		String query = String.format(
			"DELETE FROM account WHERE id = %d", this.id); //cascades
		return DatabaseUtilities.executeUpdate(this.conn, query);
	}
	
	public boolean get() throws DatabaseException {
		if(this.id == null && this.email == null)
			throw new DatabaseException("Need an id or an email for this operation.");
		
		String filter = (this.email != null)?
			String.format("email = '%s'", this.email):
			String.format("id = %d", this.id);
		
		String query = String.format("SELECT * FROM account " +
			"JOIN account_info USING(id) WHERE %s", filter);
		return DatabaseUtilities.getSingleRecord(this.conn, query, this);
	}
	
	public boolean set() throws DatabaseException {
		if(this.id == null)
			throw new DatabaseException("Need an id for this operation.");
		if(this.email == null)
			throw new DatabaseException("E-mail cannot be null nor empty.");
		
		try {
			this.conn.setTransactionIsolation(Connection.TRANSACTION_READ_COMMITTED);
			this.conn.setAutoCommit(false);
			
			String query;
			if(this.password != null && this.updatePassword) {
				if(this.password == null)
					throw new DatabaseException("Password cannot be null nor empty.");
				
				query = String.format(
					"UPDATE account SET email = '%s', password = md5('%s') WHERE id = %d", 
					this.email, this.password, this.id);
			} else {
				query = String.format(
					"UPDATE account SET email = '%s' WHERE id = %d", this.email, this.id);
			}
			
			boolean r = DatabaseUtilities.executeUpdate(this.conn, query);
			
			String surname = (this.lastName == null)? "NULL": "'" + this.lastName + "'";
			query = String.format(
				"UPDATE account_info SET first_name = '%s', last_name = %s " +
				"WHERE id = %d", this.firstName, surname, this.id);
			
			r = r & DatabaseUtilities.executeUpdate(this.conn, query);
			
			if(this.updatePrivileges) {
				//clear all previous privileges
				query = String.format("DELETE FROM account_privilege WHERE acc_id = %d", this.id);
				DatabaseUtilities.executeUpdate(this.conn, query);
				
				//re-insert new privileges
				for(Privilege p : this.privileges.values()) {
					p.setConnection(this.conn);
					if((p.getId() != null) || (p.getId() == null && p.get())) {
						query = String.format(
							"INSERT INTO account_privilege (acc_id, priv_id) " + 
							"VALUES (%d, %d)", this.id, p.getId());
						DatabaseUtilities.executeInsert(this.conn, query, this);
					}
				}
			}
			this.conn.commit();
			this.updatePassword = false;
			this.updatePrivileges = false;
			return r;
		} catch(SQLException sqle) {
			try {
				this.conn.rollback();
			} catch(SQLException e) {
			}
			throw new DatabaseException(sqle);
		} catch(DatabaseException dbe) {
			try {
				this.conn.rollback();
			} catch(SQLException e) {
			}
			throw dbe;
		} finally {
			try {
				this.conn.setAutoCommit(true);
			} catch(SQLException e) {
				throw new DatabaseException(e);
			}
		}
	}
	
	//data sets
	
	public Cursor list() throws DatabaseException {
		String query = String.format("SELECT * FROM account " +
			"JOIN account_info USING(id) ORDER BY email");
		ResultSet cursor = DatabaseUtilities.executeQuery(this.conn, query);
		return new Cursor(cursor);
	}
	
	public Cursor list(Privilege p) throws DatabaseException {
		if(p.getId() == null)
			throw new DatabaseException("A privilege ID is required for this operation.");
		
		String query = String.format("SELECT a.* FROM account a " +
			"JOIN account_privilege ap ON (a.id = ap.acc_id) " + 
			"WHERE ap.priv_id = %d", p.getId());
		
		ResultSet cursor = DatabaseUtilities.executeQuery(this.conn, query);
		return new Cursor(cursor);
	}
	
	public class Cursor extends Account implements DatabaseObject.Cursor<Account> {
		
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
	
		public java.util.Iterator<Account> iterator() {
			return new Iterator(this);
		}
		
		public List<Account> load() throws DatabaseException {
			List<Account> l = new LinkedList<Account>();
			for(Account a : this)
				l.add(a);
			return l;
		}
		
	}
	
	public class Iterator implements java.util.Iterator<Account> {
		
		private Cursor cursor;
		private Account nextItem;
		
		public Iterator(Cursor c) {
			if(c == null)
				throw new NullPointerException();
			
			try {
				this.cursor = c;
				this.nextItem = c.next()? (Account) c.clone(): null;
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
		public Account next() {
			Account r = nextItem;
			try {
				this.nextItem = cursor.next()? (Account) cursor.clone(): null;
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
