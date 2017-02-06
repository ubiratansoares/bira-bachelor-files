package xmls;

import java.util.*;

public class Boleto {
	private String nome;
	private double valor;
	private Date data;

	public Boleto(String nome, Date data, double valor) {
		this.nome = nome;
		this.data = data;
		this.valor = valor;
	}

	public void setNome(String nome) { this.nome = nome; }
	public void setValor(double valor) { this.valor = valor; }
	public void setData(Date data) { this.data = data; }

	public String getNome() { return this.nome; }
	public double getValor() { return this.valor; }
	public Date getData() { return this.data; }
}
