class A extends Thread {
	public void run() {
		double w = 0;
		for (int i = 0; i < 10000; i++) 
		for (int j = 0; j < 10000; j++) 
		for (int k = 0; k < 10000; k++) {
			w += k * j + i;
		}
	}
}
class B extends Thread {
	public void run() {
		double w = 0;
		for (int i = 0; i < 10000; i++) 
		for (int j = 0; j < 10000; j++) 
		for (int k = 0; k < 10000; k++) {
			w += k * j + i;
		}
	}
}
public class Sistema {
	public static void main(String args[]) {
		A a = new A();
		B b = new B();

		a.start();
		b.start();
		System.out.println("Sou o main");
	}
}
