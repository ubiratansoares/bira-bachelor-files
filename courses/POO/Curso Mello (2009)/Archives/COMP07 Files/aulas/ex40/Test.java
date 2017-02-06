import java.util.*;

public class Test {

	public static void main(String args[]) {
		Vector v = new Vector();

//		Item i = new Item("titulo item", 0, 0);
		Media m = new Media("titulo media", 1, 1, 1, 1, "company 01");
		DVD d = new DVD("titulo DVD", 2, 2, 2, 2, "company 02", "regiao 02");
		CD c = new CD("titulo CD", 3, 3, 3, 3, "company 03");

		v.add(i);
		v.add(m);
		v.add(d);
		v.add(c);

		for (int idx = 0 ; idx < v.size(); idx++) {
			//Item obj = (Item) v.elementAt(idx);
			Media obj = (Media) v.elementAt(idx);
			System.out.println(obj.getTitle());
			System.out.println(obj.getTracks());
		}
	}
}
