package br.usp.icmc.test01;

public interface Config {
	String driver = "org.gjt.mm.mysql.Driver"; //"org.postgresql.Driver";
	String url = "jdbc:mysql://localhost/test01db"; //"jdbc:postgresql://localhost:5432/test01db";
	String username = "root"; //"testuser";
	String password = ""; //"testpass";
}
