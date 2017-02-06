class Avo {
	public Avo() {}
	public void metodo1() { this.metodo2(); }
	public void metodo2() { System.out.println("sou o avo: metodo2"); }
}

class Pai extends Avo {
	public Pai() { super(); }
	public void metodo2() { System.out.println("sou o pai: metodo2"); }
}

public class Filho extends Pai {
	public Filho() { super(); }
	public void metodo2() { System.out.println("sou o filho: metodo2"); }

	public static void main(String args[]) {
/*
		Avo a = new Avo();
		a.metodo1();

		Pai p = new Pai();
		p.metodo1();
*/
		Filho f = new Filho();
		f.metodo1();
	}
}
