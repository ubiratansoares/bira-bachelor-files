package teste;

import java.sql.*;

public class Teste {
	public static void main(String args[]) throws Exception {
		String driver = "org.hsqldb.jdbcDriver";
		String url = "jdbc:hsqldb:hsql://localhost/xdb";
		String username = "sa";
		String password = "";

		Class.forName(driver);
		Connection conn = DriverManager.getConnection(url, 
				username, password);

		Statement stmt = conn.createStatement();
		try {
			stmt.executeUpdate("create table cliente (codigo integer not null, nome varchar(50))");
		} catch (Exception e) {}

		stmt.executeUpdate("insert into cliente (codigo, nome) values (1, 'Joao')");
		stmt.executeUpdate("insert into cliente (codigo, nome) values (2, 'Pedro')");
		stmt.executeUpdate("insert into cliente (codigo, nome) values (3, 'Paulo')");

		ResultSet rs = stmt.executeQuery("select codigo, nome from cliente");

		while (rs.next()) {
			System.out.println(rs.getInt("codigo")+" "+
					rs.getString("nome"));
		}

		stmt.close();
		conn.close();
	}
}
