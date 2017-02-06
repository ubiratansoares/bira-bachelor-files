import java.io.*;

public class Teste {
	private int min;
	private int max;

	public Teste(int min, int max) {
		this.min = min;
		this.max = max;
	}

	public void guess(int number) throws GuessRangeException {
		if (number < min || number > max) {
			throw new GuessRangeException("Out of Bounds");
		}
	}

	public static void main(String args[]) /*throws GuessRangeException */ {
		while (true) {
			try {
				int n;

				BufferedReader br = 
					new BufferedReader(
					new InputStreamReader(System.in));

				String str = br.readLine(); // IOException

				n = Integer.parseInt(str);

				Teste t = new Teste(1, 10);
				t.guess(n);
				System.out.println("Voce acertou!!!");
			} catch (GuessRangeException e) {
				System.out.println("Voce errou!!!");
			} catch (IOException e1) {
				System.out.println("Voce naum errou, nem acertou..");
				System.out.println("Que interessante... deu pau!");
			} catch (NumberFormatException nfe) {
				System.out.println("Perai... é numero!!!");
			}
		}
	}
}

class GuessRangeException extends Exception {
	public GuessRangeException(String message) {
		super(message);
	}
}
