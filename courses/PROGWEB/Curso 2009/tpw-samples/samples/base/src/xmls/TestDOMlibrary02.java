package xmls;

import java.io.* ;
import java.util.* ;
import javax.xml.parsers.* ;
import org.xml.sax.* ;
import org.w3c.dom.* ;

public class TestDOMlibrary02 {

	public static void main(String args[]) {
		Document doc = DOMlibrary.getLibrary().getDOM(args[0], false);

		Element dE = doc.getDocumentElement();
		NodeList booklist = dE.getElementsByTagName("Book");

		//Sabemos o número de Elements desta maneira:
		int bookCt = booklist.getLength();

		for (int i = 0; i < bookCt; i++) {
			//e podemos recuperar um determinado Elemento desta maneira:
			Element bookE = (Element) booklist.item(i);

			NodeList title = bookE.getElementsByTagName("Title");
			Element titleString = (Element) title.item(0);
			System.out.println("Título = " + 
					titleString.getFirstChild().getNodeValue());

			// recuperando atributos da tag Book
			NamedNodeMap map = bookE.getAttributes();
			Node nd = map.getNamedItem("isbn");
			String value = nd.getNodeValue();
			System.out.println("Isbn = " + value);
		}
	}
}
