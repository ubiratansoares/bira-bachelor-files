package xmls;

import java.io.*;
import java.util.*;
import javax.xml.parsers.*;
import org.xml.sax.*;
import org.xml.sax.helpers.*;

public class TestSAX01 extends DefaultHandler {
    private StringBuffer valor;
    private String bnome;
    private String bdata;
    private String bvalor;
    private Vector<Boleto> vetor;

    public TestSAX01() {
    	valor = new StringBuffer();
	vetor = new Vector<Boleto>();
    }
    
    public void readXML(InputStream inStream) {
        try {
            System.setProperty("org.xml.sax.driver",
			    "org.apache.crimson.parser.XMLReaderImpl");

            XMLReader reader = XMLReaderFactory.createXMLReader();
            reader.setContentHandler(this);
            reader.parse(new InputSource(new InputStreamReader(inStream)));

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void startDocument() throws SAXException {
    }

    public void endDocument() throws SAXException {
    }

    public void characters(char[] buffer, int start, int length) {
	    valor.append(buffer, start, length);
    }
    
    public void startElement (String namespaceURI,
                              String localName,
                              String qName, 
                              Attributes atts)
        throws SAXException {

	valor.setLength(0);

	for (int i = 0; i < atts.getLength(); i++) {
		System.out.println("   "+atts.getQName(i)+": "+atts.getValue(i));
	}
    }

    public void endElement(String namespaceURI, 
                           String localName,
                           String qName)
        throws SAXException {

	if (qName.equals("nome"))
		bnome = valor.toString();
	else if (qName.equals("data"))
		bdata = valor.toString();
	else if (qName.equals("valor"))
		bvalor = valor.toString();
	else if (qName.equals("boleto")) {
		Boleto boleto = new Boleto(bnome, new Date(bdata),
				Double.parseDouble(bvalor));
		vetor.add(boleto);
	}

	valor.setLength(0);
    }

    public Vector getBoletos() {
	return vetor;
    }

    public static void main(String args[]) throws Exception {
	TestSAX01 example = new TestSAX01();
        InputStream in = new BufferedInputStream(new FileInputStream(new File(args[0])));
	example.readXML(in);
	Vector<Boleto> v = example.getBoletos();
	for (int i = 0; i < v.size(); i++) {
		Boleto b = v.get(i);
		System.out.println(b.getNome()+", "+b.getData()+", "+b.getValor());
	}
    }
}
