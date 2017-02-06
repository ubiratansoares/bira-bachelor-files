package xmls;

import java.io.* ;
import java.util.* ;
import javax.xml.parsers.* ;
import org.xml.sax.* ;
import org.w3c.dom.* ;

public class TestDOMlibrary03 {

	public static void main(String args[]) {
		Document doc = DOMlibrary.getLibrary().getDOM(args[0], false);

		Element dE = doc.getDocumentElement();
		NodeList booklist = dE.getElementsByTagName("Book");

		//Sabemos o número de Elements desta maneira:
		int bookCt = booklist.getLength();

		for (int i = 0; i < bookCt; i++) {
			//e podemos recuperar um determinado Elemento desta maneira:
			Element bookE = (Element) booklist.item(i);

			// criando novo atributo no tag Edition
			NodeList editNL = bookE.getElementsByTagName("Edition");
			// Obtendo a primeira ocorrencia de Edition
			Element edition = (Element) editNL.item(0);
			// criando um novo atributo para Edition
			edition.setAttribute("printing" , "3");

			NamedNodeMap map = edition.getAttributes();
			System.out.println(map);
			//Node nd = map.getNamedItem("printing");
			//String value = nd.getNodeValue();
			//System.out.println("printing = " + value);
		}

		// criando novo livro
		//Element addBook = doc.createElement("Book");
		//doc.appendChild(addBook);
	}
}
