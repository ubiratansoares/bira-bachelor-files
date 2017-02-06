package com.news.db;

public interface Config {
	// PostgreSQL
	public String driver = "org.postgresql.Driver";
	public String url = "jdbc:postgresql://localhost:5432/userdb01";
	public String username = "userown";
	public String password = "userpass";
}
