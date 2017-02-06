package xmls;

import java.io.* ;
import java.util.* ;
import javax.xml.parsers.* ;
import org.xml.sax.* ;
import org.w3c.dom.* ;

public class TestDOMlibrary01 {

	public static void main(String args[]) {
		Document doc = DOMlibrary.getLibrary().getDOM(args[0], false);

		Element dE = doc.getDocumentElement();
		NodeList booklist = dE.getElementsByTagName("Book");

		//Sabemos o número de Elements desta maneira:
		int bookCt = booklist.getLength();

		for (int i = 0; i < bookCt; i++) {
			//e podemos recuperar um determinado Elemento desta maneira:
			Element bookE = (Element) booklist.item(i);

			// lista
			NodeList title = bookE.getElementsByTagName("Title");
			// pega primeiro da lista
			Element titleElement = (Element) title.item(0);
			// cada elemento ainda pode ter filhos
			System.out.println("Título = " + 
					titleElement.getFirstChild().getNodeValue());

			NodeList authors = bookE.getElementsByTagName("Author");

			for(int j = 0; j < authors.getLength(); j++) {
			     Element aE = (Element) authors.item( j );
			     String txt = aE.getFirstChild().getNodeValue();
			     System.out.println("Author = " + txt );
			}

			System.out.println();
		}
	}
}
