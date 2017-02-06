package rtk.util;

import rtk.persistence.database.*;
import rtk.model.*;

import java.sql.*;

public class Main {
	
	public static void test1() {
		UserAuthentication auth;
		Connection conn = null;
		try {
			conn = ConnectionManager.getConnection("main");
			auth = new UserAuthentication(conn);
			for(UserAuthentication ua : auth.list())
				System.out.println(ua.getId() + " : " + ua.getEmail() + " : " + ua.getPassword());
		} catch(Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if(conn != null)
					conn.close();
			}catch(Exception e2) {
				e2.printStackTrace();
			}
		}
		return;
	}
	
	public static void test2() {
		UserAuthentication user = new UserAuthentication();
		System.out.println(user);
		return;
	}
	
	public static void main(String[] args) {
		test1();
		return;
	}
}
