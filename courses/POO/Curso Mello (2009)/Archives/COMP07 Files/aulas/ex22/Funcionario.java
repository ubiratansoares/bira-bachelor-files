public class Funcionario extends Pessoa {
	private double salario;

	public Funcionario(String nome, int sexo, int idade, double salario) {
		super(nome, sexo, idade); // ponteiro para o objeto pai
		//setNome(nome);
		//setSexo(sexo);
		//setIdade(idade);
		//this.idade = idade;
		this.salario = salario;
	}

	public void executaTrabalho() {
		System.out.println("estou trabalhando...");
	}

	public static void main(String args[]) {
		Funcionario f = new Funcionario("nome", 0, 19, 1000);
		f.executaTrabalho();
		System.out.println(f.getNome());
	}
}
