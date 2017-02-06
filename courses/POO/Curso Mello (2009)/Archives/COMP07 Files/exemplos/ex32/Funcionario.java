public class Funcionario {
	private String cpf;
	private String nome;
	private String endereco;

	protected double salario;

	public int nfilhos;

	public Funcionario(String cpf, String nome, 
			String endereco, double salario,
			int nfilhos) {
		this.cpf = cpf;
		this.nome = nome;
		this.endereco = endereco;
		this.salario = salario;
		this.nfilhos = nfilhos;
	}

	public void imprimir() {
		System.out.println(cpf);
		System.out.println(nome);
		System.out.println(endereco);
		System.out.println(salario);
		System.out.println(nfilhos);
		System.out.println("\n");
	}
}
