package org.stockmarket.system;

import java.util.*;
import org.stockmarket.db.*;
import java.sql.*;

// para o main
import java.net.*;
import java.io.*;

public class Price {
	// atributos da classe
	private int code;
	private String date;
	private double open;
	private double high;
	private double low;
	private double close;
	private double volume;
	private double adjclose;

	public Price() {}
	public Price(int code, String date, double open, double high, 
			double low, double close, double volume, double adjclose) {
		this.code = code;
		this.date = date;
		this.open = open;
		this.high = high;
		this.low = low;
		this.close = close;
		this.volume = volume;
		this.adjclose = adjclose;
	}

	public void setCode     (int code      ) { this.code     = code; }
	public void setDate     (String date     ) { this.date     = date; }
	public void setOpen     (double open   ) { this.open     = open; }
	public void setHigh     (double high   ) { this.high     = high; }
	public void setLow      (double low    ) { this.low      = low; }
	public void setClose    (double close  ) { this.close    = close; }
	public void setVolume   (double volume ) { this.volume   = volume; }
	public void setAdjclose (double adjclose) { this.adjclose = adjclose; }

	public int    getCode     () { return this.code; }
	public String   getDate     () { return this.date; }
	public double getOpen     () { return this.open; }
	public double getHigh     () { return this.high; }
	public double getLow      () { return this.low; }
	public double getClose    () { return this.close; }
	public double getVolume   () { return this.volume; }
	public double getAdjclose () { return this.adjclose; }

	public int insert() throws Exception {
		DBConnection db = DBFactory.getConnection();

		int ret = db.update("insert into price (code, date, open, high, low, close, volume, adjclose) values ("+this.code+", '"+this.date+"', "+this.open+", "+this.high+", "+this.low+", "+this.close+", "+this.volume+", "+this.adjclose+")");
		DBFactory.closeConnection(db);
		return ret;
	}

	public int update() throws Exception {
		DBConnection db = DBFactory.getConnection();
		int ret = db.update("update price set open = "+this.open+", high = "+this.high+", low = "+this.low+", close = "+this.close+", volume = "+this.volume+", adjclose = "+this.adjclose+" where code = "+this.code+" and date = '"+this.date+"'");
		DBFactory.closeConnection(db);
		return ret;
	}

	public int remove() throws Exception {
		DBConnection db = DBFactory.getConnection();
		int ret = db.update("delete from price where code = "+this.code+" and date = '"+this.date+"'");
		DBFactory.closeConnection(db);
		return ret;
	}

	public static ResultSet findAll () throws Exception {
		DBConnection db = DBFactory.getConnection();
		return db.query("select code, date, open, high, low, close, volume, adjclose from price");
	}

	public static ResultSet findByPrimaryKey(int code) throws Exception {
		DBConnection db = DBFactory.getConnection();
		return db.query("select code, date, open, high, low, close, volume, adjclose from price where code = "+code);
	}

	public static Price next(ResultSet rs) throws Exception {
		if (rs.next()) {

			return new Price(Integer.parseInt(rs.getString("code")),
					  rs.getString("date"),
					  rs.getDouble("open"),
					  rs.getDouble("high"),
					  rs.getDouble("low"),
					  rs.getDouble("close"),
					  rs.getDouble("volume"),
					  rs.getDouble("adjclose"));
		}

		return null;
	}
}
