package br.com.jau.rna.mlp;

public class ActivationFunction {
	
	public final double function(double x) {
	 return ( 1 / (1 + Math.exp(-x) ));
	}

	public final double derivative(double x) {
	 return x * (1-x);
	}
}
