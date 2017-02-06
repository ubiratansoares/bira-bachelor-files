package teste;

public class Teste {
	private int codigo;
	private String nome;

	public Teste(int codigo, String nome) {
		this.codigo = codigo;
		this.nome = nome;
	}
	public Teste(String nome, int codigo) {
		this.codigo = codigo;
		this.nome = nome;
	}
	public Teste(String nome) { this.nome = nome; }
	public Teste(int codigo) { this.codigo = codigo; }

	public void setCodigo(int codigo) { this.codigo = codigo; }
	public void setNome(String nome) { this.nome = nome; }

	public int getCodigo() { return this.codigo; }
	public String getNome() { return this.nome; }

	public void print() {
		System.out.println(this.codigo+", "+this.nome);
	}

	public void print(String msg) {
		System.out.println(msg);
		System.out.println(this.codigo+", "+this.nome);
	}

	public static void main(String args[]) {
		Teste t1 = new Teste(1, "nome1");
		t1.print();

		Teste t2 = new Teste("nome2");
		t2.print("Mensagem padrao");

		Teste t3 = new Teste(3);
		t3.print();

		Teste t4 = new Teste("nome4", 4);
		t4.print();

	}
}
