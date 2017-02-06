package robot;

public interface Config {
	public int numconns = 5;

	public String driver = "org.postgresql.Driver";
	public String url = "jdbc:postgresql://localhost:5432/sis02";
	public String username = "usernews";
	public String password = "password";
}
