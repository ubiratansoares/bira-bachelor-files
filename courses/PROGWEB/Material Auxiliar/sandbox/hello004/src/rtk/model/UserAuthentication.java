package rtk.model;

import rtk.persistence.*;
import rtk.persistence.database.*;

import java.sql.*;
import java.util.*;

/**
 * Representa as informações de autenticação de um usuário.
 * 
 * @author Ricardo Takashi Kagawa
 */
public class UserAuthentication implements Persistable, LocalObject {
	
	private static final long serialVersionUID = 1L;
	
	private Integer id;
	private String email;
	private String password;
	
	private enum FieldKeys {
		ID, EMAIL, PASSWORD
	}
	
	private DatabaseObject<UserAuthentication> db;
	
	//construtores
	
	public UserAuthentication() {
		db = new DatabaseObject<UserAuthentication>("user_auth", this, UserAuthentication.class);
		db.bind(FieldKeys.ID, "id", DatabaseObject.ColumnType.INTEGER);
		db.bind(FieldKeys.EMAIL, "email", DatabaseObject.ColumnType.STRING);
		db.bind(FieldKeys.PASSWORD, "password", DatabaseObject.ColumnType.STRING);
	}
	
	public UserAuthentication(UserAuthentication src) {
		this();
		this.id = src.id;
		this.email = src.email;
		this.password = src.password;
	}
	
	public UserAuthentication(Connection conn) throws Exception {
		this();
		this.db.setConnection(conn);
	}
	
	public UserAuthentication createInstance() {
		return new UserAuthentication();
	}
	
	//setters e getters
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	public void setEmail(String email) {
		this.email = email;
	}
	
	public void setPassword(String password) {
		this.password = password;
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
	
	@Override
	public void set(Enum key, Object value) {
		if(key instanceof FieldKeys) {
			switch((FieldKeys) key) {
				case ID: this.id = (Integer) value; break;
				case EMAIL: this.email = (String) value; break;
				case PASSWORD: this.password = (String) value; break;
			}
		}
		return;
	}
	
	@Override
	public Object get(Enum key) {
		if(key instanceof FieldKeys) {
			switch((FieldKeys) key) {
				case ID: return this.id;
				case EMAIL: return this.email;
				case PASSWORD: return this.password;
			}
		}
		return null;
	}
	
	//persistence
	
	public void add() throws PersistenceException {
		db.useColumns(FieldKeys.EMAIL, FieldKeys.PASSWORD);
		db.add();
	}
	
	public boolean remove() throws PersistenceException {
		db.useKeys(FieldKeys.ID);
		return db.remove();
	}
	
	public boolean get() throws PersistenceException {
		if(this.email != null && !this.email.isEmpty())
			db.useKeys(FieldKeys.EMAIL);
		else
			db.useKeys(FieldKeys.ID);
		return db.get();
	}
	
	public boolean set() throws PersistenceException {
		db.useKeys(FieldKeys.ID);
		db.useColumns(FieldKeys.EMAIL, FieldKeys.PASSWORD);
		return db.set();
	}
	
	//data sets
	
	public Cursor list() throws PersistenceException {
		db.useColumns(FieldKeys.ID, FieldKeys.EMAIL, FieldKeys.PASSWORD);
		return new Cursor(db.list());
	}
	
	public class Cursor extends UserAuthentication implements rtk.persistence.Cursor<UserAuthentication> {
		
		private static final long serialVersionUID = 1L;
		
		private DatabaseObject<UserAuthentication>.Cursor cursor;
		
		public Cursor(DatabaseObject<UserAuthentication>.Cursor cursor) {
			this.cursor = cursor;
		}
		
		public boolean next() throws PersistenceException {
			return cursor.next();
		}
		
		public boolean fastForward(int n) throws PersistenceException {
			return cursor.fastForward(n);
		}
		
		public void close() throws PersistenceException {
			cursor.close();
		}
	
		@Override
		public java.util.Iterator<UserAuthentication> iterator() {
			return cursor.iterator();
		}
		
		public List<UserAuthentication> load() throws PersistenceException {
			return cursor.load();
		}
		
	}
	
}
