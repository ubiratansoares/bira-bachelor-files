class Teste2 extends Thread {
	public void run() {
		long x, z = 1, w = 8;
		while (true) {
			x = z * w;
			w++;
			z = x - w;
		}
	}
}

public class Teste extends Thread {
	public void run() {
		long x, z = 1, w = 8;
		while (true) {
			x = z * w;
			w++;
			z = x - w;
		}
	}

	public static void main(String args[]) {
		Teste t = new Teste();
		Teste2 t2 = new Teste2();
		t.start();
		t2.start();
	}
}
