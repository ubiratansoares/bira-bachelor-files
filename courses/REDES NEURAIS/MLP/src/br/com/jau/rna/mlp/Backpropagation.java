package br.com.jau.rna.mlp;


public class Backpropagation {

	private MLP m = null;
	private BackLayer[] layers = null;
	private ActivationFunction f = null;

	private double eta = 0.2;
	private double alpha = 0.8;

	public Backpropagation(MLP m) {
		
		this.m = m;
		f = new ActivationFunction();

		int size = m.getSize();
		layers = new BackLayer[size];

		for (int i=0; i < size; i++) 
			layers[i] = new BackLayer(m.getLayer(i).getSize(), 
					m.getLayer(i).getNeuronSize());	
	}

	public Backpropagation(MLP m, double eta, double alpha) {
		this(m);

		this.eta = eta;
		this.alpha = alpha;
	}

	public void setEta(double eta) {
		this.eta = eta;
	}

	public double getEta() {
	 return eta;
	}

	public void setAlpha(double alpha) {
		this.alpha = alpha;
	}

	public double getAlpha() {
	 return alpha;
	}

	public void backward(double[] input, double[] desiredOutput) {

		computeLastLayerErrors(desiredOutput);
		computeInnerErrors();
		
		
		computeDeltas(input);

		applyCorrections();
	}

	public void train(double[] input, double[] desiredOutput) {

		m.forward(input);
		
		backward(input, desiredOutput);
	}

	private void computeLastLayerErrors(double[] desiredOutput) {
		Layer l = m.getLastLayer();
		BackLayer last  = layers[layers.length - 1];
		
		int size = last.getSize();

		for (int i=0; i < size; i++)
			last.setError(i, 
					(desiredOutput[i] - l.getOutput(i)) * f.derivative(l.getOutput(i))
				     );
	}

	private void computeInnerErrors() {
		double error;

		for (int i = layers.length - 2; i >= 0; i--) // Layer by Layer 
			for (int j = 0; j < layers[i].getSize(); j++) { // Neuron by Neuron
				
				error = 0;

				/* Neuron by Neuron in the next Layer */
				for (int k=0; k < layers[i+1].getSize(); k++)
					error += layers[i+1].getError(k) * 
						 m.getLayer(i+1).getNeuron(k).getWeight(j);
				
				error *= f.derivative(m.getLayer(i).getOutput(j));
				
				layers[i].setError(j, error);
			}
	}

	private void computeDeltas(double[] input) {
		int i, j, k;
		
		for (i=0; i < layers.length; i++) // Layer by Layer
			for (j=0; j < layers[i].getSize(); j++) { // Neuron by Neuron

				for (k=0; k < layers[i].getBackNeuron(j).getSize(); k++) { // Weight by Weight
					
					layers[i].getBackNeuron(j).setDeltaw(k,
						eta * input[k] * layers[i].getError(j) +
						alpha * layers[i].getBackNeuron(j).getDeltaw(k)
					);
				}

				layers[i].getBackNeuron(j).setDeltaBias(
					eta * (-1) * layers[i].getError(j) +
					alpha * layers[i].getBackNeuron(j).getDeltaBias()
				);

				input = m.getLayer(i).getOutput();
			}
	}
	
	private void applyCorrections() {
		int i, j, k;

		for (i=0; i < m.getSize(); i++)
			for (j=0; j < m.getLayer(i).getSize(); j++) {
				
				Neuron n = m.getLayer(i).getNeuron(j);
				
				for (k=0; k < n.getSize(); k++) 
					n.correctWeight(k, layers[i].getBackNeuron(j).getDeltaw(k));

				n.correctBias(layers[i].getBackNeuron(j).getDeltaBias());
			}
	}
}
