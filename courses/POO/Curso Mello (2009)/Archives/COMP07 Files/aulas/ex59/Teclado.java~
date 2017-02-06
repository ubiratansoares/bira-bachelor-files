import java.io.*;

public class Teclado {

	public void m1() throws IOException {
		int t = System.in.read(); // byte a byte
		System.out.println(t);
	}

	public static void main(String args[]) {
		try {
			Teclado t = new Teclado();
			t.m1();
		} catch (IOException e) {
			System.out.println("Ocorreu um problema de teclado");
			System.out.println("Sistema não pode responder");
		}
	}
}
