package robot;

import java.sql.*;
import java.net.*;
import java.io.*;

public class RSS {
	// atributos
	private String datetime;
	private String link;
	private String title;
	private String description;

	// conexão com banco de dados
	DBConnection db;

	public RSS() {}

	public RSS(String datetime, String link, String title, String description) {
		this.datetime = datetime;
		this.link = link.replace('\'', ' ');
		this.title = title.replace('\'', ' ');
		this.description = description.replace('\'', ' ');
	}

	public void setDatetime(String datetime) { this.datetime = datetime; }
	public void setLink(String link) { this.link = link.replace('\'', ' '); }
	public void setTitle(String title) { this.title = title.replace('\'', ' '); }
	public void setDescription(String description) { this.description = description.replace('\'', ' '); }

	public String getDatetime() { return datetime; }
	public String getLink() { return link; }
	public String getTitle() { return title; }
	public String getDescription() { return description; }

	public int insert() throws Exception {
		if (db == null) db = DBFactory.getConnection();

		//System.out.println(this.link+" "+this.title+" "+this.description);
		int r = db.update("insert into _rss (datetime, link, title, description)"+ 
				"values (now(), '"+this.link+"', '"+
				this.title+"', '"+this.description+"')");
		DBFactory.closeConnection(db);
		return r;
	}

	public int remove() throws Exception {
		if (db == null) db = DBFactory.getConnection();
		int r = db.update("delete from _rss where link = '"+this.link+"'");
		DBFactory.closeConnection(db);
		return r;
	}

	public int update() throws Exception {
		if (db == null) db = DBFactory.getConnection();
		String sql = "update _rss set title = '"+this.title+"', "+
				" description = '"+this.description+"', "+
				" where link = '"+this.link+"'";
		int r = db.update(sql);
		DBFactory.closeConnection(db);
		return r;
	}

	public static ResultSet findByDateTime() throws Exception {
		DBConnection db = DBFactory.getConnection();
		return db.query("select datetime, link, title, description from _rss order by datetime desc limit 2");
	}

	public static ResultSet findAll() throws Exception {
		DBConnection db = DBFactory.getConnection();
		return db.query("select datetime, link, title, description from _rss");
	}

	public static RSS findByPrimaryKey(String link) throws Exception {
		DBConnection db = DBFactory.getConnection();
		return next(db.query("select datetime, link, title, description from _rss where link = '"+link+"'"));
	}

	public static RSS next(ResultSet rs) throws Exception {
		if (rs.next()) {
			return new RSS( rs.getString("datetime"),
					rs.getString("link"),
					rs.getString("title"),
					rs.getString("description"));
		}
		return null;
	}
}
