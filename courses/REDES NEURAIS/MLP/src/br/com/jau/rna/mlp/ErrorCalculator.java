package br.com.jau.rna.mlp;

public class ErrorCalculator {
	private MLP m;

	public ErrorCalculator(MLP m) {
		this.m = m;
	}

	private double calculateEnergySum(double[] desiredOutput) {
		
		int size = m.getOutputSize();

		double energy = 0;

		for (int i=0; i < size; i++) 
			energy += (desiredOutput[i] - m.getOutput(i)) * (desiredOutput[i] - m.getOutput(i));	

		return energy;
	}

	public double calculateSingleEnergy(double[] desiredOutput) {
		return (calculateEnergySum(desiredOutput)/2);
	}

	public double calculateBatchEnergy(double[][] trainingSet, double[] desiredOutput) {
		
		int size = trainingSet.length;
		
		double energy = 0;

		for (int i=0; i < size; i++) {
			m.forward(trainingSet[i]);
			energy += calculateEnergySum(desiredOutput);
		}

		return (energy/2);
	}
}
