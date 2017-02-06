public class Mat {
	
	public int soma(int a, int b) { return a + b; }
	public double soma(double a, double b) { return a + b; }


	public static void main(String args[]) {
		Mat m = new Mat();

		m.soma(1, 2);
		m.soma(1.5, 2.5);
	}
}
