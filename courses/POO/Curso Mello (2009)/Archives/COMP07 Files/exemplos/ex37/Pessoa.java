public class Pessoa {
	private String cpf;
	private String nome;

	public void mudarInfo(String cpf) {
		System.out.println("void mudarInfo(String cpf)");
		this.cpf = cpf;
	}

	public void mudarInfo(String cpf, String nome) {
		System.out.println("void mudarInfo(String cpf, String nome)");
		this.cpf = cpf;
		this.nome = nome;
	}

	public static void main(String args[]) {
		Pessoa p = new Pessoa();
		p.mudarInfo("123");
		p.mudarInfo("123", "João");
	}
}
