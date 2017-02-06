package com.galpao.bd;

public interface Config {

	// MySQL
	public String driver = "org.gjt.mm.mysql.Driver";
	public String url = "jdbc:mysql://localhost:3306/galpaodb";
	public String username = "root";
	public String password = "";

	/*
	// PostgreSQL
	public String driver = "org.postgresql.Driver";
	public String url = "jdbc:postgresql://localhost:5432/galpaodb";
	public String username = "galpaouser";
	public String password = "galpaopass";*/
}
