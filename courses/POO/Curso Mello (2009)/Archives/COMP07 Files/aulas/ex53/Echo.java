import java.io.*;
import java.net.*;

public class Echo extends Component {
	public void run(Socket s) throws Exception {
		// canal de entrada
		BufferedReader br = new BufferedReader(
				new InputStreamReader(s.getInputStream()));
		String text = br.readLine();

		// canal de saida
		PrintStream ps = new PrintStream(s.getOutputStream());
		ps.println("echo: "+text);
	}
}
