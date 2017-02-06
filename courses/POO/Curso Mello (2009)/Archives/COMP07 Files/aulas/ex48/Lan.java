import java.util.*;

public class Lan extends NetworkingConnection {
	public Lan() {}

	public void connect() {
		System.out.println("DHCP Discovery");
		System.out.println("DHCP Request");
		System.out.println("I got the IP");
		System.out.println("Agora sim...");
		this.cstatus = 1;
	}
	public void disconnect() {
		System.out.println("killall -9 dhcpcd");
		System.out.println("ifconfig eth0 down");
		this.cstatus = 0;
	}
	public Vector statistics() { return null; }
}
