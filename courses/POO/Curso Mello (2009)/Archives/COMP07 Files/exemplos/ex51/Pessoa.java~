public class Pessoa {
	int id;
	static int v = 0;

	public Pessoa() {
		this.id = v++;
	}

	public int getCodigo() {
		return this.id;
	}

	public static void contar(int c) {
		for (int i = 0; i < c; i++) {
			System.out.println(i);
		}
		v = 10;
		//id = 0; // não funciona, pois o método é static e somente pode
		          // acessar variáveis ou métodos do tipo static
	}

	public static void main(String args[]) {
		Pessoa.contar(2); // chamar diretamente sem ter um objeto
				  // é similar à uma biblioteca de funções

		Pessoa p = new Pessoa();
		System.out.println("\n\np - id: "+p.getCodigo());
	}
}
