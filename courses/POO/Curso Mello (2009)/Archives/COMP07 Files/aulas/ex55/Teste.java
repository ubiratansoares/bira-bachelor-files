public class Teste extends Object implements A, B {
	public void print() {
		System.out.println("teste");
	}

	public void run() {}

	public static void main(String args[]) {
		Teste t = new Teste();
		t.print();
	}
}
