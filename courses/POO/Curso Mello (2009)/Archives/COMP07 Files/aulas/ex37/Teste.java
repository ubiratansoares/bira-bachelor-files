import java.util.*;

public class Teste extends Object {
	private int a;
	public Teste() { this.a = 5; }
	public Teste(int a) { this.a = a; }
	public int getA() { return this.a; }

	public static void main(String args[]) {
		Vector v = new Vector();
		v.add(new Teste());
		//Object t = v.elementAt(0);
		//System.out.println(t.getA());
		Teste t = (Teste) v.elementAt(0);
		System.out.println(t.getA());
	}
}
