
import br.com.jau.rna.mlp.*;

public class TestError {

	public static void main(String[] args) {
		final int ITERACTIONS = 15000;

		final double[][] input = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
		//final double[][] input = { {-1, -1}, {-1, 0}, {1, -1}, {0, 1} };
		//final double[][] input = { {2, 2}, {2, 0}, {1, 2}, {0, 1} };
		final double[][] desiredOutput = { {0}, {1}, {1}, {0} };

		MLP m = new MLP(2);

		m.addFirstLayer(2, 2);
		m.addLayer(1);

		Backpropagation b = new Backpropagation(m);

		b.setEta(0.005);
		b.setAlpha(0.995);
		
		ErrorCalculator ec = new ErrorCalculator(m);
		
		for (int i=0; i < ITERACTIONS; i++) {
			double error = 0;
			System.out.print( i + "\t");

			for (int j=0; j < 4; j++) {
				b.train(input[j], desiredOutput[j]);
				error += ec.calculateSingleEnergy(desiredOutput[j]);
				System.out.print(m.getOutput(0) + "\t");
			}
			
			System.out.println(error);
			error = 0;
		}

	}

}
