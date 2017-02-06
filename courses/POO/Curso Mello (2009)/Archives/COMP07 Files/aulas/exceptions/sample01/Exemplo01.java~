import java.util.*;

public class Exemplo01 {
	Vector v;

	public Exemplo01() {
		v = new Vector();
	}

	public void test() {
		try {
			v = null;
			v.elementAt(10);
		} catch (NullPointerException p2) {
			System.out.println("Vetor não foi definido");
		} catch (ArrayIndexOutOfBoundsException p1) {
			//p1.printStackTrace();
			System.out.println("Cliente vc está tentando acessar um elemento inexistente no vetor");

		} catch (Exception e) {
			System.out.println("Ocorreu uma exceção");
		}
		
	}

	public static void main(String args[]) {
		Exemplo01 ex01 = new Exemplo01();
		ex01.test();
	}
}
