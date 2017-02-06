import java.util.*;

public abstract class NetworkingConnection {
	protected int cstatus;
	public NetworkingConnection() {
		this.cstatus = 0;
	}
	public abstract void connect();
	public abstract void disconnect();
	public int status() { return this.cstatus; }
	public Vector statistics() { return null; }
}
