public class Pessoa {

	private String nome;
	private String endereco;

	public void altera(String nom, String end) {
		nome = nom;
		endereco = end;
	}
	public void imprime() {
		System.out.println("nome = "+nome+" endereco = "+endereco);
	}
	public static void main(String args[]) {
		Pessoa p = new Pessoa();
		p.altera("Joao", "Rua abc, 123");
		p.imprime();
	}
}
