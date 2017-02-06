import java.util.*;
import java.text.*;

public class Teste {
	public static void main(String args[]) {
		Locale currentLocale = new Locale("pt", "BR");
		ResourceBundle msg =
		    ResourceBundle.getBundle("messages", currentLocale);

		System.out.println(msg.getString("MENSAGEM"));
	}

}
