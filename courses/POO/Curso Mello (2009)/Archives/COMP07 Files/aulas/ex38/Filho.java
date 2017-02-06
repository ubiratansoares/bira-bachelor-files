import java.util.*;

class Pai {
	private int a;
	public Pai() { this.a = 5; }
	public Pai(int a) { this.a = a; }
	public int getA() { return this.a; }
}

public class Filho extends Pai {
	public Filho() { super(); }
	public Filho(int a) { super(a); }
	public void print() { System.out.println("sou o filho");}

	public static void main(String args[]) {
		/*
		Pai pai = new Pai();
		Filho filho = null;
		filho = (Filho) pai;
		*/
		
		Filho filho = new Filho();
		Pai pai = null;

		//pai = (Pai) filho;
		pai = filho;
		System.out.println(pai.getA());

		Filho filho_o_retorno = null;
		filho_o_retorno = (Filho) pai;
		filho_o_retorno.print();
	}
}
