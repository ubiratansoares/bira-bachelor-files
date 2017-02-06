package org.usp.news;

import java.io.*;
import java.util.*;
import javax.xml.parsers.*;
import org.xml.sax.*;
import org.xml.sax.helpers.*;

public class ParserSAX extends DefaultHandler {

    StringBuffer valor;

    String title;
    String link;
    String description;

    public ParserSAX() {
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
		//System.out.println("title: "+valor);
		title = valor.toString();
	} else if (qName.equals("link")) {
		//System.out.println("link: "+valor);
		link = valor.toString();
	} else if (qName.equals("description")) {
		//System.out.println("description: "+valor);
		description = valor.toString();
	} else if (qName.equals("item")) {
		try {
			RSS rss = new RSS(null, link, title, description);
			rss.insert();
		} catch (Exception e) { e.printStackTrace(); }
	}
	valor.setLength(0);
    }

/*
    public static void main(String args[]) throws Exception {
	ParserSAX example = new ParserSAX();
        InputStream in = new BufferedInputStream(new FileInputStream(new File(args[0])));
	example.readXML(in);
    }*/
}
