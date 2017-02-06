package sistema;

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

	public synchronized static DBConnection getConnection() throws Exception {
		// wait... mas closeConnection naum teria synchronized
		if (conn.size() > 0) {
			DBConnection c = (DBConnection) conn.elementAt(0);
			conn.removeElementAt(0);
			return c;
		} 
		return null;
	}

	public synchronized static void closeConnection(DBConnection c) {
		conn.add(c);
	}

	public synchronized static int availableConnection() {
		return conn.size();
	}
}
