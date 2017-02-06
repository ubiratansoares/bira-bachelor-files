package com.empresa.projeto;

import com.empresa.bd.*;

// JavaBean
public class User {
	private String name;
	private String login;
	private String password;

	public User() {}

	public void setName(String name) { this.name = name; }
	public void setLogin(String login) { this.login = login; }
	public void setPassword(String password) { this.password = password; }

	public String getName() { return this.name; }
	public String getLogin() { return this.login; }
	public String getPassword() { return this.password; }

	public void insert() throws Exception {
		DBConnection db = new DBConnection();
		db.execute("insert into _user (name, login, password) values ('"+this.name+"', '"+this.login+"', '"+this.password+"')");
	}
}
