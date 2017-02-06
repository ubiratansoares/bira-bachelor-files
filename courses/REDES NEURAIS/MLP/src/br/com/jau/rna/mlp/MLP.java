package br.com.jau.rna.mlp;

public class MLP {
	private Layer[] layers;
	private int lastLayer = -1;

	public MLP(int layers) {
		this.layers = new Layer[layers];
	}


	public void addFirstLayer(int neurons, int connections) {
		layers[0] = new Layer(neurons, connections);
		lastLayer = 0;
	}

	public void addLayer(int neurons) {

		layers[lastLayer + 1] = new Layer(neurons, layers[lastLayer].getSize());

		lastLayer++;
	}

	public void forward(double[] input) {

		
		for (int i=0; i < layers.length; i++) {
			layers[i].compute(input);
			
			input = layers[i].getOutput();
		}

	}
	

	public double getOutput(int neuron) {
		return layers[lastLayer].getOutput(neuron);
	}

	public int getOutputSize() {
		return layers[lastLayer].getSize();
	}


	public int getWinner() {
		int winner = Integer.MIN_VALUE;
		double value = Double.MIN_VALUE;

		for (int i = 0; i < layers[lastLayer].getSize(); i++) {
			if (layers[lastLayer].getOutput(i) > value) {
				winner = i;
				value = layers[lastLayer].getOutput(i);
			}
		}

	 return winner;
	}

	public int getSize() {
		return layers.length;
	}

	public String toString() {
		return getClass().getName() + "[" + layers.length + "]";
	}

	final Layer getLayer(int i) {
		return layers[i];
	}

	final Layer getLastLayer() {
		return layers[lastLayer];
	}
}
