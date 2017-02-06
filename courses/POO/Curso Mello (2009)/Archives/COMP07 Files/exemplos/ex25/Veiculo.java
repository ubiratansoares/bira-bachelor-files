public class Veiculo {
	String placa;
	String marca;
	String modelo;
	int ano;

	public Veiculo(String placa, String marca, String modelo, int ano) {
		this.placa = placa;
		this.marca = marca;
		this.modelo = modelo;
		this.ano = ano;
	}

	public void print() {
		System.out.println(placa+" "+marca+" "+modelo+" "+ano);
	}
}
