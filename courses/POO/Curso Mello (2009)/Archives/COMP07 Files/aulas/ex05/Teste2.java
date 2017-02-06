import java.io.*;

public class Teste2 {

	public static void main(String args[]) throws Exception {
		InputStreamReader isr = 
			new InputStreamReader(System.in); // InputStream
		BufferedReader br = new BufferedReader(isr);

		String str = br.readLine();

		System.out.println(str);
	}
}
