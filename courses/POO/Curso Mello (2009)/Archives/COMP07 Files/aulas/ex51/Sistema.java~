class A extends Thread {
	public void run() {
		while (true) {
			System.out.println("A");
			try { Thread.sleep(100); // milisegundos
			} catch (Exception e ) {}
		}
	}
}
class B extends Thread {
	public void run() {
		while (true) {
			System.out.println("B");
			try { Thread.sleep(500); // milisegundos
			} catch (Exception e ) {}
		}
	}
}
public class Sistema {
	public static void main(String args[]) {
		A a = new A();
		B b = new B();

		a.start();
		b.start();
		/*b.pause();
		b.resume();
		b.stop();*/
		System.out.println("Sou o main");
	}
}
