package robot;

import java.util.*;

public class DBFactory implements Config {

	private static Vector conn;

	static {
		conn = new Vector();

		try {
			for (int i = 0; i < numconns; i++)
				conn.add(new DBConnection(driver, url, username, password));
		} catch (Exception e) {}
	}

	public static DBConnection getConnection() throws Exception {
		if (conn.size() > 0) {
			DBConnection c = (DBConnection) conn.elementAt(0);
			conn.removeElementAt(0);
			return c;
		} 
		return null;
	}

	public static void closeConnection(DBConnection c) {
		conn.add(c);
	}

	public static int availableConnection() {
		return conn.size();
	}
}
