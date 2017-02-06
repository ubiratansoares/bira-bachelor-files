import java.util.*;

public class Test2 {

	public static void main(String args[]) {
		DVD d = new DVD("titulo DVD", 2, 2, 2, 2, 
				"company 02", "regiao 02");

		//Media m = (Media) d;
		Media m = d;
		d = null;
		d = (DVD) m;
		System.out.println(d.getRegion());
		System.out.println(d.getTitle());
		System.out.println(d.getTracks());
	}
}
