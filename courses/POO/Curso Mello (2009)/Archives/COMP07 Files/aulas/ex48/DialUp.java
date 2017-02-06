import java.util.*;

public class DialUp extends NetworkingConnection {
	public DialUp() {}

	public void connect() {
		System.out.println("ATZ");
		System.out.println("Limpar os registradores do meu modem");
		System.out.println("Inicializar o modem");
		System.out.println("Discar...");
		System.out.println("PPP (Point-to-Point Protocol)");
		this.cstatus = 1;
	}

	public void disconnect() {
		System.out.println("ATZ");
		this.cstatus = 0;
	}

	public Vector statistics() { return null; }
}
