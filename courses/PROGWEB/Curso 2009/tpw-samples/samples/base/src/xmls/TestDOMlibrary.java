package xmls;

import java.io.* ;
import java.util.* ;
import javax.xml.parsers.* ;
import org.xml.sax.* ;
import org.w3c.dom.* ;

public class TestDOMlibrary {

	public static void main(String args[]) {
		// Obtendo um parser DOM para o arquivo (false = sem checar DTD)
		Document doc = DOMlibrary.getLibrary().getDOM(args[0], false);

		// obtem elementos internos ao Documento (1 nível)
		Element dE = doc.getDocumentElement();
		NodeList booklist = dE.getElementsByTagName("Book");

		//Sabemos o número de Elements desta maneira:
		int bookCt = booklist.getLength();

		for (int i = 0; i < bookCt; i++) {
			//e podemos recuperar um determinado Elemento desta maneira:
			Element bookE = (Element) booklist.item(i);
			System.out.println(bookE);
		}
	}
}
