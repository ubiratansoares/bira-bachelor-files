public class Funcionario extends Object {
	String nome;
	double salario;

	public Funcionario(String n, float s) {
		nome = n;
		salario = s;
	}

	public void imprimir() {
		System.out.println(nome+" "+salario);
	}

	public static void main(String args[]) {
		Funcionario f = new Funcionario("João", 100);
		f.imprimir();
	}
}
