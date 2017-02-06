public class Caminhao extends Veiculo {
	double capacidadeEmToneladas;

	public Caminhao(String placa, String marca, String mo, int an, double cap) {
		super(placa, marca, mo, an);
		capacidadeEmToneladas = cap;
	}

	public static void main(String args[]) {
		Veiculo v = new Veiculo("ABC1234", "Ford", "Fiesta", 2003);
		v.print();

		Caminhao c = new Caminhao("BCB1234", "Mercedes", "1113", 1990, 13.1);
		c.print();
	}
}
