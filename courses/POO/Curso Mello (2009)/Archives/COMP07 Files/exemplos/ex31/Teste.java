public class Teste {
	int x, y;

	public Teste(int x, int y) {
		this.x = x;
		this.y = y;
	}

	public void imprimir() {
		System.out.println(this.x+" "+this.y);
		//System.out.println(x+" "+y);
	}

	public static void main(String args[]) {
		Teste t = new Teste(5,4);
		t.imprimir();
	}
}
