import java.util.*;
import java.text.*;

public class Available {
	public static void main(String[] args) {
		Locale list[] = DateFormat.getAvailableLocales();
		for (Locale aLocale : list) {
			System.out.println(aLocale.toString());
		}
	}
}

