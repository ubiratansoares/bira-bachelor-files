class Teste2 extends Teste {
	public Teste2(int i) { super(i); }
	public void metodo1() {
		System.out.println("chamando metodo1()");
	}
}
public abstract class Teste {
	private int t;

	public Teste(int t) { this.t = t; }
	public void setT(int t) { this.t = t; }
	public int getT() { return this.t; }

	public abstract void metodo1();

	public static void main(String args[]) {
		// Naum posso criar objetos de classes abstratas
		//Teste t = new Teste(5);
		Teste2 t = new Teste2(5);
		t.metodo1();
	}
}
