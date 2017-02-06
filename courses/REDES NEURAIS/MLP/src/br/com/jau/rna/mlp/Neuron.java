package br.com.jau.rna.mlp;



// Classe : Neuron
// Definições básicas para o neurônio processador da MLP

class Neuron {
	
	private double[] weights = null;
	private double bias = 0;

	Neuron(int connections) {
		weights = new double[connections];
	}

	int getSize() {
 	 return weights.length;
	}

	public String toString() {
		StringBuffer b = new StringBuffer();

		b.append(getClass().getName() + "[");

		b.append("bias=" + bias + "; ");
		
		for (int i=0; i < weights.length; i++)
			b.append("w" + (i+1) + "=" + weights[i] + "; ");

		b.append("]");

	 return b.toString();
	}

	final double getWeight(int i) {
	 return weights[i];
	}

	final void setWeight(int i, double w) {
		weights[i] = w;
	}

	final double getBias() {
	 return bias;
	}

	final void setBias(double b) {
		bias = b;
	}

	final void correctWeight(int i, double delta) {
		weights[i] += delta;
	}

	final void correctBias(double delta) {
		bias += delta;
	}

	void randomize() {

		for (int i = 0; i < weights.length; i++)
			weights[i] = Math.random();

		bias = Math.random();
	}

}
