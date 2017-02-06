class Worker2 implements Runnable
{
	public void run() {
		System.out.println("Eu sou uma thread criada");
	}
}

public class Second
{
	public static void main(String args[]) {
		Thread thrd = new Thread(new Worker2());
		thrd.start();
		System.out.println("Eu sou a thread principal");
	}
}