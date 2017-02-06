class OperacaoMatematica {
	// metodo virtual utilizado pelo sistema
	public double calcular(double x, double y) { return 0; }
}

class Soma extends OperacaoMatematica {
	public double calcular(double x, double y) {
		return x+y;
	}
}

class Subtracao extends OperacaoMatematica {
	public double calcular(double x, double y) {
		return x-y;
	}
}

public class Contas {
	public static void mostrarCalculo(OperacaoMatematica operacao, double x, double y) {
		System.out.println("O resultado é: " + operacao.calcular(x, y));
	}
	    
	public static void main(String args[]) {
		Soma s = new Soma();
		Contas.mostrarCalculo(s, 5, 5); 
		Contas.mostrarCalculo(new Subtracao(), 5, 5); 
	}
}
	
