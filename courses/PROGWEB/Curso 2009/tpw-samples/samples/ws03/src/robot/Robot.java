package robot;

import java.net.*;
import java.io.*;
import java.util.*;
import javax.xml.parsers.*;
import org.xml.sax.*;
import org.xml.sax.helpers.*;
import org.usp.news.*;

public class Robot extends DefaultHandler {

    Vector elements;
    StringBuffer valor;

    String title;
    String link;
    String description;

    public Robot() {
	elements = new Vector();
    	valor = new StringBuffer();
    }
    
    /** Read XML from input stream and parse, generating SAX events */
    public void readXML(InputStream inStream) {
        try {
            System.setProperty("org.xml.sax.driver", "org.apache.crimson.parser.XMLReaderImpl");
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
    }

    public void endElement(String namespaceURI, 
                           String localName,
                           String qName)
        throws SAXException {

	if (qName.equals("title")) {
		title = valor.toString();
	} else if (qName.equals("link")) {
		link = valor.toString();
	} else if (qName.equals("description")) {
		description = valor.toString();
	} else if (qName.equals("item")) {
		try {
			RSS rss = new RSS(null, link, title, description);
			elements.add(rss);
		} catch (Exception e) { e.printStackTrace(); }
	}
	valor.setLength(0);
    }

    public Vector getNews(String url) throws Exception {
		URL site = new URL(url);
		URLConnection sld = site.openConnection();
		BufferedReader in = new BufferedReader(
				new InputStreamReader(sld.getInputStream()));
		BufferedWriter out = 
			new BufferedWriter(new FileWriter("/tmp/file.xml"));
		String inputLine;

		while ((inputLine = in.readLine()) != null)  {
			out.write(inputLine+"\n");
		}
		out.close();
		in.close();

		InputStream ins = new BufferedInputStream(
				new FileInputStream(new File("/tmp/file.xml")));
		this.readXML(ins);

		return this.elements;
    }
}
