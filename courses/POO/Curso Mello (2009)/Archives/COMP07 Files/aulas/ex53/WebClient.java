import java.net.*; // acesso a rede
import java.io.*;

public class WebClient {
	private Socket s;
	public WebClient(String machine, int port) throws Exception { 
		this.s = new Socket(machine, port); 
	}
	public void executa() throws Exception {
				// canal de saida
		PrintStream ps = new PrintStream(s.getOutputStream());
		ps.println("alo mundo");

		BufferedReader br = new BufferedReader(
				new InputStreamReader(s.getInputStream()));
		String text = br.readLine();

		System.out.println(text);

		s.close();
	}
	public static void main(String args[]) throws Exception {
		WebClient ws = new WebClient("localhost", 80);
		ws.executa();
	}
}
