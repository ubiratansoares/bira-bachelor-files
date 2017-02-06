package br.com.jau.rna.mlp;

class BackNeuron {
	
	private double[] deltaw = null;
	private double deltaBias = 0;

	BackNeuron(int size) {
		deltaw = new double[size];

		/* Iniciando com zeros */
		for (int i=0; i < deltaw.length; i++)
			deltaw[i] = 0;
	}

	final double getDeltaw(int i) {
		return deltaw[i];
	}

	final void setDeltaw(int i, double value) {
		deltaw[i] = value;
	}

	final double getDeltaBias() {
		return deltaBias;
	}

	final void setDeltaBias(double deltaBias) {
		this.deltaBias = deltaBias;
	}

	int getSize() {
		return deltaw.length;
	}
}
