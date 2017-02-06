package rtk.model;

import rtk.database.*;

import java.io.*;
import java.sql.*;
import java.util.*;
import javax.xml.stream.*;

/**
 * Representa as informações de autenticação de um usuário.
 * 
 * @author Ricardo Takashi Kagawa
 */
public class UserAuthentication implements DatabaseObject, Cursor<UserAuthentication> {
	
	/* == atributos e construtores == */
	
	private Integer id;
	private String email;
	private String password;
	
	public UserAuthentication() {
	}
	
	public UserAuthentication(UserAuthentication src) {
		this.id = src.id;
		this.email = src.email;
		this.password = src.password;
	}
	
	/* == setters e getters == */
	
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
	
	/* == DatabaseObject == */
	
	private static Map<String, SQLStatement> statements;
	
	static {
		try {
			StatementLoader loader = new StatementLoader();
			loader.load(UserAuthentication.class
				.getResource("UserAuthentication.xml"));
			statements = new HashMap<String, SQLStatement>();
			statements.put("insert", loader.getStatement("user.insert"));
			statements.put("delete", loader.getStatement("user.delete"));
			statements.put("update", loader.getStatement("user.update"));
			statements.put("update.nopass", loader.getStatement("user.update-nopass"));
			statements.put("select", loader.getStatement("user.select"));
			statements.put("select.email", loader.getStatement("user.select-email"));
			statements.put("auth", loader.getStatement("user.auth"));
			statements.put("list", loader.getStatement("user.select-all"));
		} catch(Exception e) {
			throw new ExceptionInInitializerError(e);
		}
	}
	
	public void add(Connection conn) throws DatabaseException {
		SQLStatement sql = statements.get("insert").clone();
		try {
			sql.clear();
			sql.setValue("email", this.email);
			sql.setValue("password", this.password);
			sql.execute(conn);
			
			if(sql.next()) {
				this.id = sql.getValue("id", Integer.class);
				this.password = sql.getValue("password", String.class);
			}
		} finally {
			sql.close();
		}
	}
	
	public boolean remove(Connection conn) throws DatabaseException {
		SQLStatement sql = statements.get("delete").clone();
		try {
			sql.clear();
			sql.setValue("id", this.id);
			sql.execute(conn);
			return (sql.getUpdateCount() > 0);
		} finally {
			sql.close();
		}
	}
	
	public boolean get(Connection conn) throws DatabaseException {
		SQLStatement sql = null;
		try {
			if(this.email != null && !this.email.isEmpty()) {
				sql = statements.get("select.email").clone();
				sql.clear();
				sql.setValue("email", this.email);
			} else {
				sql = statements.get("select").clone();
				sql.clear();
				sql.setValue("id", this.id);
			}
			
			sql.execute(conn);
			
			if(sql.next()) {
				this.id = sql.getValue("id", Integer.class);
				this.email = sql.getValue("email", String.class);
				this.password = sql.getValue("password", String.class);
				return true;
			} else
				return false;
		} finally {
			sql.close();
		}
	}
	
	public boolean set(Connection conn) throws DatabaseException {
		SQLStatement sql = (this.password != null && !this.password.isEmpty())?
				statements.get("update"): statements.get("update-nopass");
		try {
			sql = sql.clone();
			sql.clear();
			sql.setValue("id", this.id);
			sql.setValue("email", this.email);
			sql.setValue("password", this.password);
			
			sql.execute(conn);
			
			if(sql.next()) {
				this.password = sql.getValue("password", String.class);
				return true;
			} else
				return (sql.getUpdateCount() > 0);
		} finally {
			sql.close();
		}
	}
	
	public boolean authenticate(Connection conn) throws DatabaseException {
		if(this.password == null || this.email == null)
			throw new DatabaseException("This operation requires an email and a password.");
		if(this.password.isEmpty() || this.email.isEmpty())
			throw new DatabaseException("This operation requires an email and a password.");
		
		SQLStatement sql = statements.get("auth").clone();
		try {
			sql.clear();
			sql.setValue("email", this.email);
			sql.setValue("password", this.password);
			
			sql.execute(conn);
			
			if(sql.next()) {
				return sql.getValue("ok", Boolean.class);
			} else
				return false;
		} finally {
			sql.close();
		}
	}
	
	/* == Cursor == */
	
	private SQLStatement cursor;
	
	public UserAuthentication list(Connection conn) throws DatabaseException {
		if(cursor != null) cursor.close();
		
		cursor = statements.get("list").clone();
		cursor.clear();
		cursor.execute(conn);
		return this;
	}
	
	public void close() throws DatabaseException {
		if(cursor != null)
			cursor.close();
		cursor = null;
		return;
	}
	
	public boolean next() throws DatabaseException {
		if(cursor != null && cursor.next()) {
			this.id = cursor.getValue("id", Integer.class);
			this.email = cursor.getValue("email", String.class);
			this.password = cursor.getValue("password", String.class);
			return true;
		} else
			return false;
	}
	
	public boolean seek(int n) throws DatabaseException {
		if(cursor != null && cursor.seek(n)) {
			this.id = cursor.getValue("id", Integer.class);
			this.email = cursor.getValue("email", String.class);
			this.password = cursor.getValue("password", String.class);
			return true;
		} else
			return false;
	}
	
	public List<UserAuthentication> load() throws DatabaseException {
		List<UserAuthentication> l = new LinkedList<UserAuthentication>();
		while(this.next())
			l.add(this.clone());
		this.close();
		return l;
	}
	
	public Iterator<UserAuthentication> iterator() {
		return new CursorIterator<UserAuthentication>(this);
	}
	
	/* == outros métodos == */
	
	@Override
	public String toString() {
		return String.format("{id=%d, email=%s, password=%s}", 
				id, email, password);
	}
	
	@Override
	public UserAuthentication clone() {
		UserAuthentication copy = new UserAuthentication();
		copy.id = this.id;
		copy.email = this.email;
		copy.password = this.password;
		return copy;
	}
	
}
