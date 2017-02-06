package rtk.util;

import rtk.database.*;
import rtk.model.*;

import rtk.template.*;
import rtk.view.*;

import java.sql.*;
import java.util.*;

public class Main {
	
	public static void test1() throws Exception {
		UserAuthentication auth = new UserAuthentication();
		Connection conn = ConnectionManager.getInstance().getConnection("main");
		try {
			auth.setEmail("teste");
			auth.setPassword("teste");
			auth.add(conn);
			System.out.println(auth);
		} finally {
			conn.close();
		}
	}
	
	public static void test2() throws Exception {
		UserAuthentication auth = new UserAuthentication();
		Connection conn = ConnectionManager.getInstance().getConnection("main");
		try {
			auth.setEmail("teste");
			auth.get(conn);
			System.out.println(auth);
		} finally {
			conn.close();
		}
		return;
	}
	
	public static void test3() throws Exception {
		UserAuthentication auth = new UserAuthentication();
		Connection conn = ConnectionManager.getInstance().getConnection("main");
		try {
			auth.setEmail("teste");
			auth.get(conn);
			System.out.println(auth);
			auth.setEmail("teste2");
			auth.setPassword(null);
			auth.set(conn);
		} finally {
			conn.close();
		}
		return;
	}
	
	public static void test4() throws Exception {
		UserAuthentication auth = new UserAuthentication();
		Connection conn = ConnectionManager.getInstance().getConnection("main");
		try {
			auth.setEmail("teste2");
			auth.get(conn);
			System.out.println(auth);
			auth.remove(conn);
		} finally {
			conn.close();
		}
		return;
	}
	
	public static void test5() throws Exception {
		UserAuthentication cursor = new UserAuthentication();
		Connection conn = ConnectionManager.getInstance().getConnection("main");
		try {
			cursor.list(conn);
			while(cursor.next())
				System.out.println(cursor);
			cursor.close();
		} finally {
			conn.close();
		}
		return;
	}
	
	public static void test6() throws Exception {
		UserAuthentication cursor = new UserAuthentication();
		Connection conn = ConnectionManager.getInstance().getConnection("main");
		try {
			for(UserAuthentication user : cursor.list(conn))
				System.out.println(user);
			cursor.close();
		} finally {
			conn.close();
		}
		return;
	}
	
	public static void main(String[] args) throws Exception {
		try {
//			ConnectionManager.getInstance().open("cfg/db-config.xml");
			HtmlPage page = new HtmlPage(new Locale("", "", "klingon"));
//			page.setBodyContent(l);
			System.out.println(page.getHeaders());
		} finally {
//			ConnectionManager.getInstance().close();
		}
	}
}
