import java.util.*;
import java.util.regex.*;

public class Teste {

	public static void main(String args[]) {
		Scanner s = new Scanner("14334 Rodrigo 22222 Joao 3333");
		s.findInLine("(\\d+) Rodrigo (\\d+) (\\w+) (\\d+)");

		MatchResult result = s.match();
		for (int i=1; i<=result.groupCount(); i++)
			System.out.println(result.group(i));
		s.close();
	}
}
