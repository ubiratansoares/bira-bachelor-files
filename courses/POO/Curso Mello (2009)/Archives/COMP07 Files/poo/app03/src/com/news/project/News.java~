package com.news.project;

import java.sql.*;
import com.news.db.*;

public class News {
	private String date;
	private String title;
	private String body;

	public News() {}

	public void setDate (String date   ) { this.date = date; }
	public void setTitle(String title) { this.title = title; }
	public void setBody (String body ) { this.body = body; }

	public String getDate()  { return this.date; }
	public String getTitle() { return this.title; }
	public String getBody()  { return this.body; }

	public void insert() throws Exception {
		DBConnection db = new DBConnection();
		db.execute("insert into _news (date, title, body) values ('"+this.date+"', '"+this.title+"', '"+this.body+"')");
		db.close();
	}

	public static ResultSet findAll() throws Exception {
		DBConnection db = new DBConnection();
		return db.query("select date, title, body from _news");
	}

	public static ResultSet findNewer() throws Exception {
		DBConnection db = new DBConnection();
		return db.query("select date, title, body from _news where date < now() order by date desc limit 10");
	}

	public static News next(ResultSet rs) throws Exception {
		if (rs.next()) {
			News news = new News();
			news.setDate(rs.getString("date"));
			news.setTitle(rs.getString("title"));
			news.setBody(rs.getString("body"));
			return news;
		}
		return null;
	}
}
