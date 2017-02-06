class A {
	private int a = 1;
	protected int b = 2;
	public int c = 3;
	int d = 4; // sem modificador -> package
}
class B extends A {
	public void teste() {
		//System.out.println(a);
		System.out.println(b);
		System.out.println(c);
		System.out.println(d); // se estiver no mesmo diretório
	}
}
public class Exemplo1 {
	public static void main(String args[]) {
		B b = new B();
		b.teste();
	}
}
