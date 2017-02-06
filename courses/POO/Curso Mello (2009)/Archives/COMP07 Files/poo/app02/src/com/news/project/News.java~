package com.news.project;

import java.sql.*;
import com.news.db.*;

public class News {
	private java.util.Date date;
	private String title;
	private String body;

	public News() {}

	public void setDate (java.util.Date date   ) { this.date = date; }
	public void setTitle(String title) { this.title = title; }
	public void setBody (String body ) { this.body = body; }

	public java.util.Date   getDate()  { return this.date; }
	public String getTitle() { return this.title; }
	public String getBody()  { return this.body; }

	public static ResultSet findAll() throws Exception {
		DBConnection db = new DBConnection();
		return db.query("select date, title, body from _news");
	}
}
