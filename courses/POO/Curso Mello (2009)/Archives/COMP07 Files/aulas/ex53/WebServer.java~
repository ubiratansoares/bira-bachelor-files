import java.net.*; // acesso a rede
import java.io.*;
import java.util.*;

public class WebServer {
	private ServerSocket ss;
	private Hashtable components;

	public WebServer(int port) throws Exception { 
		this.components = new Hashtable();
		this.ss = new ServerSocket(port); 
	}

	public void add(String name, Component c) {
		components.put(name, c);
	}

	public void executa() throws Exception {
		while (true) {
			Socket s = this.ss.accept();
			BufferedReader br = 
				new BufferedReader(new InputStreamReader(
						s.getInputStream()));
			String name = br.readLine();

			Component c = (Component) 
				this.components.get(name);

			if (c != null)
				c.run(s);

			s.close();
		}
	}
	public static void main(String args[]) throws Exception {
		WebServer ws = new WebServer(80);
		ws.add("echoserver", new Echo());
		ws.add("servidordecalculo", new Calculo());
		ws.executa();
	}
}
