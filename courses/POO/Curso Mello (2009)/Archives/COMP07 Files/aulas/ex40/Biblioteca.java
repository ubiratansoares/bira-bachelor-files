import java.util.*;

public class Biblioteca {
	private Vector acervo;

	public Biblioteca() {
		this.acervo = new Vector();
	}

	public void add(Item item) {
		this.acervo.add(item);
	}

	public Item get(int pos) {
		return (Item) this.acervo.elementAt(pos);
	}

	public int size() {
		return this.acervo.size();
	}

	public static void main(String args[]) {
		Biblioteca b = new Biblioteca();

		Media m = new Media("media 01", 1, 1, 10, 1, "company 01");
		DVD d = new DVD("dvd 02", 2, 2, 20, 2, 
				"company 02", "region 02");

		// Item
		b.add(d);
		b.add(m);

		// Listar Itens
		for (int i = 0; i < b.size(); i++) {
			Item it = b.get(i);
			System.out.println("Imprimindo informacoes de "+it);
			it.print();
			System.out.println();
		}
	}
}
