
import br.com.jau.rna.mlp.*;

public class TestError2 {

	public static void main(String[] args) {
		final double ERROR = 0.001;
		final int ITERACTIONS = 7000;

		final double[][] input = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
		//final double[][] input = { {-1, -1}, {-1, 0}, {1, -1}, {0, 1} };
		//final double[][] input = { {2, 2}, {2, 0}, {1, 2}, {0, 1} };
		//final double[][] desiredOutput = { {0}, {1}, {1}, {0} };
		final double[][] desiredOutput = { {0}, {1}, {1}, {0} };

		MLP m = new MLP(2);

		m.addFirstLayer(2, 2);
		m.addLayer(1);

		Backpropagation b = new Backpropagation(m);

		b.setEta(0.005);
		b.setAlpha(0.995);
		
		ErrorCalculator ec = new ErrorCalculator(m);
		
		double error = ERROR+1;

		long t0 = System.currentTimeMillis();

		long i=0;

		int count = 0;

		while ( (error > ERROR) && (i < ITERACTIONS) ) {
			System.out.print( i + "\t");

			error = 0;
			for (int j=0; j < 4; j++) {
				b.train(input[j], desiredOutput[j]);
				error += ec.calculateSingleEnergy(desiredOutput[j]);
				System.out.print(m.getOutput(0) + "\t");
			}
			
			System.out.println(error);
			
			if ( (i % (ITERACTIONS/10)) == 0 )
				System.err.print( (count++)*10 + "%...");
			

			i++;
		}

		System.err.println("100%");

		long t1 = System.currentTimeMillis();

		System.err.println("Duração: " + (t1 - t0) + " milisegundos");
	}

}
