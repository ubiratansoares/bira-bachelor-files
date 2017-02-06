package org.aplicacao.hello;

//import org.aplicacao.teste.*;
import org.aplicacao.teste.Calculo;

public class Hello {
	private String msg;

	public Hello(String msg) {
		this.msg = msg;
	}

	public void print() {
		System.out.println(Calculo.soma(5,4));
		System.out.println(this.msg);
	}

	//             0    1    2
	// java Hello abc dgdg agag
	//
	//     0    1   2    3
	// ./teste abc dgdg dggd

	public static void main(String args[]) {
		Hello h = new Hello(args[0]);
		h.print();
	}
}
