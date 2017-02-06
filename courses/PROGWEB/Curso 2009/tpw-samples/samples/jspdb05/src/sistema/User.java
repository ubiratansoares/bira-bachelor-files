package sistema;

import java.sql.*;

public class User {
	// atributos
	private String name;
	private String login;
	private String passwd;

	DBConnection db;

	public User() {}
	public User(String name, String login, String passwd) {
		this.name = name;
		this.login = login;
		this.passwd = passwd;
	}

	public void setName(String name) { this.name = name; }
	public void setLogin(String login) { this.login = login; }
	public void setPasswd(String passwd) { this.passwd = passwd; }

	public String getName() { return name; }
	public String getLogin() { return login; }
	public String getPasswd() { return passwd; }

	public int insert() throws Exception {

		System.out.println("ANTES: Total de conexões disponíveis = "+DBFactory.availableConnection());

		if (db == null) db = DBFactory.getConnection();

		System.out.println("DEPOIS DE OBTER: Total de conexões disponíveis = "+DBFactory.availableConnection());

		int r = db.update("insert into _user (name, login, passwd) values ('"+
				this.name+"', '"+this.login+"', '"+
				this.passwd+"')");
		DBFactory.closeConnection(db);

		System.out.println("DEPOIS DE FECHAR: Total de conexões disponíveis = "+DBFactory.availableConnection());

		return r;
	}

	public int remove() throws Exception {
		if (db == null) db = DBFactory.getConnection();
		int r = db.update("delete from _user where login = '"+this.login+"'");
		DBFactory.closeConnection(db);
		return r;
	}

	public int update() throws Exception {
		if (db == null) db = DBFactory.getConnection();
		String sql = "update _user set name = '"+this.name+"', "+
				" passwd = '"+this.passwd+"' "+
				" where login = '"+this.login+"'";
		int r = db.update(sql);
		DBFactory.closeConnection(db);
		return r;
	}

	public static ResultSet findAll() throws Exception {
		DBConnection db = DBFactory.getConnection();
		return db.query("select name, login, passwd from _user");
	}

	public static User findByPrimaryKey(String login) throws Exception {
		DBConnection db = DBFactory.getConnection();
		return next(db.query("select name, login, passwd from _user where login = '"+login+"'"));
	}

	public static User next(ResultSet rs) throws Exception {
		if (rs.next()) {
			return new User(rs.getString("name"), 
					rs.getString("login"),
					rs.getString("passwd"));
		}
		return null;
	}
}
