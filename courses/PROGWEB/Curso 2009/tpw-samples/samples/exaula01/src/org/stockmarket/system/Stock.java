// vitorasso@gmail.com

package org.stockmarket.system;

import org.stockmarket.db.*;
import java.sql.*;
import java.util.*;
import java.io.*;
import java.net.*;

public class Stock {
	private int code;
	private String name;
	private int startDay;
	private int startMonth;
	private int startYear;

	public Stock() {}
	public Stock(int code, String name, int startDay, int startMonth, int startYear) {
		this.code = code;
		this.name = name;
		this.startDay = startDay;
		this.startMonth = startMonth;
		this.startYear = startYear;
	}

	public void setCode(int code) { this.code = code; }
	public void setName(String name) { this.name = name; }
	public void setStartDay(int startDay) { this.startDay = startDay; }
	public void setStartMonth(int startMonth) { this.startMonth = startMonth; }
	public void setStartYear(int startYear) { this.startYear = startYear; }

	public int getCode() { return this.code; }
	public String getName() { return this.name; }
	public int getStartDay() { return this.startDay; }
	public int getStartMonth() { return this.startMonth; }
	public int getStartYear() { return this.startYear; }

	public void insert() throws Exception {
		/*
		 CREATE FUNCTION stock_insert(n TEXT, m INTEGER, d INTEGER, y INTEGER)
		 RETURNS integer AS $$
		 DECLARE scode integer;
		 BEGIN
		         INSERT INTO stock (name, startmonth, startday, startyear) 
			 	VALUES ($1, $2, $3, $4);
			         SELECT last_value INTO scode FROM stock_code;
				         RETURN scode;
					 END;
					 $$  LANGUAGE plpgsql
					 ;
		 **/
		DBConnection db = DBFactory.getConnection();
		ResultSet rs = db.query("select stock_insert('"+this.name+"', "+this.startDay+", "+this.startMonth+", "+this.startYear+")");
		if (rs.next()) {
			this.code = rs.getInt("stock_insert");
		}
		DBFactory.closeConnection(db);
	}

	public int update() throws Exception {
		DBConnection db = DBFactory.getConnection();
		int ret = db.update("update stock set name = '"+this.name+"', startday = "+this.startDay+", startmonth = "+this.startMonth+", startyear = "+this.startYear+" where code = "+this.code);
		DBFactory.closeConnection(db);
		return ret;
	}

	public int remove() throws Exception {
		DBConnection db = DBFactory.getConnection();
		int ret = db.update("delete from stock where code = "+this.code);
		DBFactory.closeConnection(db);
		return ret;
	}

	public static ResultSet findAll() throws Exception {
		DBConnection db = DBFactory.getConnection();
		return db.query("select code, name, startday, startmonth, startyear from stock");
	}

	public static ResultSet findByPrimaryKey(int code) throws Exception {
		DBConnection db = DBFactory.getConnection();
		return db.query("select code, name, startday, startmonth, startyear from stock where code = "+code);
	}

	public static Stock next(ResultSet rs) throws Exception {
		if (rs.next()) {
			return new Stock(Integer.parseInt(rs.getString("code")),
					  rs.getString("name"),
					  rs.getInt("startmonth"),
					  rs.getInt("startday"),
					  rs.getInt("startyear"));
		}

		return null;
	}

	// Teste de Unidade
	public static void main(String args[]) throws Exception {

		ResultSet rs = Stock.findAll();
		Stock stock = null;

		while ((stock = Stock.next(rs)) != null) {
			Calendar rightNow = Calendar.getInstance();

			String quote = stock.getName();
			String em = new Integer(rightNow.get(Calendar.MONTH)).toString();
			String ed = new Integer(rightNow.get(Calendar.DAY_OF_WEEK)).toString();
			String ey = new Integer(rightNow.get(Calendar.YEAR)).toString();
			String sm = new Integer(stock.getStartMonth()).toString();
			String sd = new Integer(stock.getStartDay()).toString();
			String sy = new Integer(stock.getStartYear()).toString();

			String url = "http://ichart.finance.yahoo.com/table.csv?s="+quote+"&d="+em+"&e="+ed+"&f="+ey+"&g=d&a="+sm+"&b="+sd+"&c="+sy+"&ignore=.csv";

			URL site = new URL(url);
			URLConnection yahoo = site.openConnection();

			BufferedReader in = new BufferedReader(
					new InputStreamReader(yahoo.getInputStream()));
			String inputLine = in.readLine(); // só para pegar texto

			while ((inputLine = in.readLine()) != null)  {
				int i1 = inputLine.indexOf(",");
				String date = inputLine.substring(0, i1);

				inputLine = inputLine.substring(i1+1);
				i1 = inputLine.indexOf(",");
				String open = inputLine.substring(0, i1);

				inputLine = inputLine.substring(i1+1);
				i1 = inputLine.indexOf(",");
				String high = inputLine.substring(0, i1);

				inputLine = inputLine.substring(i1+1);
				i1 = inputLine.indexOf(",");
				String low = inputLine.substring(0, i1);

				inputLine = inputLine.substring(i1+1);
				i1 = inputLine.indexOf(",");
				String close = inputLine.substring(0, i1);

				inputLine = inputLine.substring(i1+1);
				i1 = inputLine.indexOf(",");
				String volume = inputLine.substring(0, i1);

				inputLine = inputLine.substring(i1+1);
				String adjclose = inputLine;

				Price p = new Price(stock.getCode(), 
						date,
						Double.parseDouble(open),
						Double.parseDouble(high),
						Double.parseDouble(low),
						Double.parseDouble(close),
						Double.parseDouble(volume),
						Double.parseDouble(adjclose)
						);
				p.insert();
			}

			in.close();
		}
	}

}
