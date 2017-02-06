import java.util.*;

public class Teste {
	public static void main(String args[]) {
		Vector v = new Vector();
		try {
			v.add(new Object()); 
			Object o = v.elementAt(5);
			// 100 linhas
		} catch (NullPointerException n) {
			System.out.println("Oiii "+n.getMessage());
		} catch (ArrayIndexOutOfBoundsException a) {
			System.out.println("Ourl... "+a.getMessage());
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			System.out.println("finally");
		}
	}
}
