import java.net.*;
import java.io.*;

public class Calculo extends Component {
	public void run(Socket s) throws Exception {
		// formatando canal de entrada de dados
		BufferedReader br = new BufferedReader(
			new InputStreamReader(s.getInputStream()));

		String str = br.readLine();

		System.out.println(str);

		double w = 0;
		for (int i = 0; i < 1000; i++)
		for (int j = 0; j < 1000; j++)
		for (int k = 0; k < 1000; k++) {
			w += i * j + k;
			w--;
		}

		// formatando canal de saida de dados
		PrintStream ps = new PrintStream(s.getOutputStream());
		ps.println("terminei de processar");
	}
}
