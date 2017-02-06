public class Teste {
	public static void main(String args[]) {
		Funcionario f = new Funcionario("123", "João", "Rua abc", 100.23, 2);
		Gerente g = new Gerente("123", "João", "Rua abc", 100.23, 2, "depto");

		f.imprimir();
		g.imprimir();

		System.out.println("nome do funcionário = "+f.nome);
		System.out.println("número de filhos do funcionário = "+f.nfilhos);
	}
}
