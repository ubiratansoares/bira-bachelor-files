package xmls;

import java.io.* ;
import java.util.* ;
import javax.xml.parsers.* ;
import org.xml.sax.* ;
import org.w3c.dom.* ;

public class TestDOMlibrary04 {

	public static void main(String args[]) {
		Document doc = DOMlibrary.getLibrary().getDOM(args[0], false);

		Element dE = doc.getDocumentElement();
		NodeList booklist = dE.getElementsByTagName("Book");

		//Sabemos o número de Elements desta maneira:
		int bookCt = booklist.getLength();

		for (int i = 0; i < bookCt; i++) {
			Node bookE = (Node) booklist.item(i);
			NodeList nl = bookE.getChildNodes();

			for (int j = 0; j < nl.getLength(); j++) {
				Node n = (Node) nl.item(j);
				System.out.println(n.getNodeValue());
				//Element e = (Element) nl.item(j);
				//System.out.println(e.getFirstChild().getNodeValue());
			}
		}
	}
}
