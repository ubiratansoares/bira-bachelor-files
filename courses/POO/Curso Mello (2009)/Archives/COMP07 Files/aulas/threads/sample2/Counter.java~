public class Counter {
	private int c = 0;
	public synchronized void increment() { c++; }
	public synchronized void decrement() { c--; }
	public synchronized int value() { return c; }
	public static void main(String args[]) {
		Counter c = new Counter();

		/*
		Produtor p = new Produtor(c);
		p.start();
		*/
		(new Produtor(c)).start();
		(new Consumidor(c)).start();
	}
}

class Produtor extends Thread {
	private Counter r;
	public Produtor(Counter r) { this.r = r; }
	public void run() {
		while (true) {
			r.increment();
			System.out.println("produtor -> "+r.value());
			try { Thread.sleep(1000); } catch (Exception e) {}
		}
	}
}

class Consumidor extends Thread {
	private Counter r;
	public Consumidor(Counter r) { this.r = r; }
	public void run() {
		while (true) {
			r.decrement();
			System.out.println("consumidor -> "+r.value());
			try { Thread.sleep(1000); } catch (Exception e) {}
		}
	}
}

//
// Ambos lêem o valor da variável c. Um deles incrementa e o outro decrementa.
// Espera-se que o decremento ocorra depois do incremento. Contudo, pode ocorrer
// que a thread que incrementa leia o valor de c e seja chaveada para aquela
// que decrementa o valor de c. Essa que decrementa lê o valor de c e, possivelmente,
// decrementa o valor para -1. a outra thread então volta a executar e incrementa
// esse ultimo valor
