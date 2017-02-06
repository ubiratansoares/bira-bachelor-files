package xmls;

import java.io.*;
import java.util.*;
import javax.xml.parsers.*;
import org.xml.sax.*;
import org.xml.sax.helpers.*;

public class TestSAX extends DefaultHandler {

    StringBuffer valor;

    public TestSAX() {
    	valor = new StringBuffer();
    }
    
    /** Read XML from input stream and parse, generating SAX events */
    public void readXML(InputStream inStream) {
        try {
	    // define a implementação de SAX a ser utilizada
            System.setProperty("org.xml.sax.driver", "org.apache.crimson.parser.XMLReaderImpl");

	    // cria objeto da org.xml.sax.XMLReader para ler arquivo
            XMLReader reader = XMLReaderFactory.createXMLReader();

	    // define classe que herda DefaultHandler e, portanto, deve definir os métodos
	    // para o parser
            reader.setContentHandler(this);

	    // inicia o parsing
            reader.parse(new InputSource(new InputStreamReader(inStream)));

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void startDocument() throws SAXException {
	    System.out.println("Começo de documento");
    }

    public void endDocument() throws SAXException {
	    System.out.println("Final de documento");
    }

    public void characters(char[] buffer, int start, int length) {
	    System.out.println("characters -> buffer: "+(new String(buffer)).substring(start, start+length)+" start: "+start+" length: "+length);
	    valor.append(buffer, start, length);
    }
    
    public void startElement (String namespaceURI,
                              String localName,
                              String qName, 
                              Attributes atts)
        throws SAXException {

	valor.setLength(0);
	System.out.println("Início do elemento "+qName);

	for (int i = 0; i < atts.getLength(); i++) {
		System.out.println("   "+atts.getQName(i)+": "+atts.getValue(i));
	}
    }

    public void endElement(String namespaceURI, 
                           String localName,
                           String qName)
        throws SAXException {

	System.out.println("Final do elemento: "+qName+" valor: "+valor);
	valor.setLength(0);
    }

    public static void main(String args[]) throws Exception {
	TestSAX example = new TestSAX();
        InputStream in = new BufferedInputStream(new FileInputStream(new File(args[0])));
	example.readXML(in);
    }
}
