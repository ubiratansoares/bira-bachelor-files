public class ProdutorVetor extends Thread {
	public ProdutorVetor() {}

	public void run() {
		int val = 0;
		RecursoVetor r = RecursoVetor.getInstance();
		while (true) {
			r.put(val);
			val = val + 10;
			try { Thread.sleep(100); } catch (Exception e) {}
		}
	}

	public static void main(String args[]) {
		ProdutorVetor p = new ProdutorVetor();
		ConsumidorVetor c = new ConsumidorVetor();
		p.start();
		c.start();
	}
}

class ConsumidorVetor extends Thread {
	public ConsumidorVetor() {}

	public void run() {
		RecursoVetor r = RecursoVetor.getInstance();
		while (true) {
			int v[] = r.get();
			for (int i = 0; i < v.length; i++)
				System.out.print(v[i]+" ");
			System.out.println();
			try { Thread.sleep(100); } catch (Exception e) {}
		}
	}
}

class RecursoVetor {
	private int[] n;
	private static RecursoVetor r = null;
	public RecursoVetor() { this.n = new int[10]; }
	public synchronized static RecursoVetor getInstance() {
		if (r == null)
			r = new RecursoVetor();
		return r;
	}
	public synchronized void put(int v) { 
		for (int i = 0; i < 10; i++) {
			n[i] = v + i;
			try { Thread.sleep(100); } catch (Exception e) {}
		}
	}
	public synchronized int[] get() { return this.n; }
}
