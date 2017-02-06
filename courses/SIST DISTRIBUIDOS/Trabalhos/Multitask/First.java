class Worker1 extends Thread
{
	public void run() {
		System.out.println("Eu sou uma thread criada");
	}
}

public class First
{
	public static void main(String args[]) {
		Thread runner = new Worker1();
		runner.start();
		System.out.println("Eu sou a thread principal");
	}
}