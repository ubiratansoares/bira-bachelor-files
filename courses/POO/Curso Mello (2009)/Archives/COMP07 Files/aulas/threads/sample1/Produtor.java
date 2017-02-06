import java.util.*;

public class Produtor extends Thread {
	private Recurso r;

	public Produtor(Recurso r) { this.r = r; }

	public void run() {
		Random rnd = new Random();
		while (true) {
			int val = rnd.nextInt(1000);
			System.out.println("putting: "+val);
			r.put(val);
			try { Thread.sleep(1000); } catch (Exception e) {}
		}
	}

	public static void main(String args[]) {
		Recurso r = new Recurso();
		Produtor p = new Produtor(r);
		Consumidor c = new Consumidor(r);
		p.start();
		c.start();
	}
}

class Consumidor extends Thread {
	private Recurso r;

	public Consumidor(Recurso r) { this.r = r; }

	public void run() {
		while (true) {
			System.out.println("getting: "+r.get());
			try { Thread.sleep(1000); } catch (Exception e) {}
		}
	}
}

class Recurso {
	private int n;

	public Recurso() { this.n = -1; }
	public void put(int n) { this.n = n; }
	public int get() { return this.n; }
}
