package br.usp.icmc.app01;

import br.usp.icmc.teste.*;
import com.empresa.framework.*;
import com.empresa.framework.core.*;

public class App {
	public App() {}

	public void m1() {
		System.out.println(Calculo.soma(50,10));

		Teste t = new Teste();
		t.teste();

		Abc abc = new Abc();
		abc.metodo();
	}

	public static void main(String args[]) {
		App app = new App();
		app.m1();
	}
}
