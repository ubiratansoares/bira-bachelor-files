package sistema;

public class DBFactory implements Config {

	public static DBConnection getConnection() throws Exception {
		return new DBConnection(driver, url, username, password);
	}
}
