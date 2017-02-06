package org.stockmarket.db;

public interface Config {
	public int numconns = 5;

	public String driver = "org.postgresql.Driver";
	public String url = "jdbc:postgresql://localhost:5432/stockdb";
	public String username = "stock02";
	public String password = "stock02";
}
