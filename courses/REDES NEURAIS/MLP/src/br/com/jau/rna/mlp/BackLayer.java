package br.com.jau.rna.mlp;

class BackLayer {
	private double[] errors = null;
	private BackNeuron[] neurons = null;

	BackLayer(int size, int nsize) {
		errors = new double[size];
		neurons = new BackNeuron[size];

		for (int i=0; i < neurons.length; i++) {
			errors[i] = 0;
			neurons[i] = new BackNeuron(nsize);
		}
	}

	final void setError(int i, double value) {
		errors[i] = value;
	}

	final double getError(int i) {
		return errors[i];
	}

	final BackNeuron getBackNeuron(int i) {
		return neurons[i];
	}

	int getSize() {
		return neurons.length;
	}
}
