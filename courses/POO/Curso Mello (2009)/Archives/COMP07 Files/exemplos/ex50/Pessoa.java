public class Pessoa {
	int id;
	static int v = 0;

	public Pessoa() {
		this.id = v++;
	}

	public int getCodigo() {
		return this.id;
	}

	public static void main(String args[]) {
		Pessoa p1 = new Pessoa();
		Pessoa p2 = new Pessoa();

		System.out.println("p1 - id: "+p1.getCodigo());
		System.out.println("p2 - id: "+p2.getCodigo());
	}
}
