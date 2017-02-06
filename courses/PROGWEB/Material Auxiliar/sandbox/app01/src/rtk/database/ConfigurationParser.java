package rtk.database;

import rtk.util.*;

import java.io.*;
import java.util.*;
import javax.xml.stream.*;

import static javax.xml.stream.XMLStreamConstants.*;

public class ConfigurationParser {
	
	//1. CONFIG = '<db-config>' DRIVER+ DATABASE+ '</db-config>'.
	//2. DRIVER = '<driver id="' ID '">' TEXT '</driver>'.
	//3. DATABASE = '<database id="' ID '">' DB_PARAM+ '</database>'.
	//4. DB_PARAM = URL | UNAME? | PWD? | LB? | UB? | TO?.
	//5. URL = '<url>' TEXT '</url>'
	//6. UNAME = '<username>' TEXT '</username>'
	//7. PWD = '<password>' TEXT '</password>'
	//8. LB = '<lower-bound>' INTEGER '</lower-bound>'
	//9. UB = '<upper-bound>' INTEGER '</upper-bound>'
	//10. TO = '<timeout>' INTEGER '</timeout>'
	
	public void parse(InputStream in) throws XMLStreamException {
		this.valid = false;
		XMLInputFactory factory = XMLInputFactory.newFactory();
		XMLStreamReader reader = factory.createXMLStreamReader(in);
		try {
			StaxParser p = new StaxParser(reader);
			p.requireEvent(START_DOCUMENT);
			p.step();
			
			this.config(p);
			
			p.requireEvent(END_DOCUMENT);
		} catch(NoSuchElementException e) {
			throw new XMLStreamException("Invalid configuration file.");
		} finally {
			reader.close();
		}
		this.valid = true;
	}
	
	/* ===== non-terminal symbols ===== */
	
	private void config(StaxParser p) throws XMLStreamException {
		p.requireStartTag("db-config");
		p.step();
		
		this.drivers(p);
		
		this.databases(p);
		
		p.requireEndTag("db-config");
		p.step();
	}
	
	private void drivers(StaxParser p) throws XMLStreamException {
		// DRIVERS = DRIVER+ [lookahead = DATABASE].
		this.driver(p);
		while(p.acceptStartTag("driver"))
			this.driver(p);
	}
	
	private void driver(StaxParser p) throws XMLStreamException {
		p.requireStartTag("driver");
		Map<String, Integer> attrs = p.mapAttributes();
		String id = p.requireAttribute(attrs, "id");
		p.step();
		
		String driverClass = p.requireCharacters();
		
		drivers.put(id, driverClass);
		
		p.requireEndTag("driver");
		p.step();
	}
	
	private void databases(StaxParser p) throws XMLStreamException {
		// DATABASES = DATABASE+ [lookahead = '</db-config>'].
		this.database(p);
		while(p.acceptStartTag("database"))
			this.database(p);
	}
	
	private void database(StaxParser p) throws XMLStreamException {
		p.requireStartTag("database");
		Map<String, Integer> attrs = p.mapAttributes();
		String id = p.requireAttribute(attrs, "id");
		String driver = p.requireAttribute(attrs, "driver");
		p.step();
		
		Set<String> tagNames = new HashSet<String>(6);
		String names = "url, username, password, lower-bound, upper-bound, timeout";
		for(String name : names.split(", ")) tagNames.add(name);
		
		Map<String, String> properties = new HashMap<String, String>(6);
		String key = null, value = null;
		while(p.acceptEvent(START_ELEMENT) && 
				tagNames.contains(p.getReader().getLocalName())) {
			key = p.getReader().getLocalName();
			value = this.property(p, key);
			properties.put(key, value);
		}
		
		if(!properties.keySet().contains("url"))
			p.throwException("Invalid configuration file.");
		
		DatabaseConnection conn = new DatabaseConnection(id, properties.get("url"));
		
		conn.setUsername(properties.get("username"));
		conn.setPassword(properties.get("password"));
		conn.setDialect(driver);
		
		if(properties.get("lower-bound") == null)	properties.put("lower-bound", "0");
		if(properties.get("upper-bound") == null)	properties.put("upper-bound", "0");
		if(properties.get("timeout") == null)		properties.put("timeout", "0");
		
		conn.setMinimumPoolSize(Integer.valueOf(properties.get("lower-bound")));
		conn.setMaximumPoolSize(Integer.valueOf(properties.get("upper-bound")));
		conn.setIdleTimeout(Integer.valueOf(properties.get("timeout")));
		
		connections.put(id, conn);
		
		p.requireEndTag("database");
		p.step();
	}
	
	private String property(StaxParser p, String name) throws XMLStreamException {
		p.requireStartTag(name);
		p.step();
		
		String value = p.requireCharacters();
		
		p.requireEndTag(name);
		p.step();
		
		return value;
	}
	
	/* ===== parameters ===== */
	
	private boolean valid = false;
	private Map<String, String> drivers = new HashMap<String, String>();
	private Map<String, DatabaseConnection> connections = new HashMap<String, DatabaseConnection>();
	
	public Map<String, String> getDrivers() {
		if(!valid) return Collections.<String, String>emptyMap();
		return new HashMap<String, String>(drivers);
	}
	
	public Map<String, DatabaseConnection> getConnections() {
		if(!valid) return Collections.<String, DatabaseConnection>emptyMap();
		return new HashMap<String, DatabaseConnection>(connections);
	}
	
	/* ===== test ===== */
	
	private void testParsing(XMLStreamReader reader) throws XMLStreamException {
		do {
			switch(reader.getEventType()) {
				case ATTRIBUTE: System.out.println("ATTRIBUTE"); break;
				case CDATA: System.out.println("CDATA"); break;
				case CHARACTERS: System.out.println("CHARACTERS"); break;
				case COMMENT: System.out.println("COMMENT"); break;
				case DTD: System.out.println("DTD"); break;
				case END_DOCUMENT: System.out.println("END_DOCUMENT"); return;
				case END_ELEMENT: System.out.println("END_ELEMENT"); break;
				case ENTITY_DECLARATION: System.out.println("ENTITY_DECLARATION"); break;
				case ENTITY_REFERENCE: System.out.println("ENTITY_REFERENCE"); break;
				case NAMESPACE: System.out.println("NAMESPACE"); break;
				case NOTATION_DECLARATION: System.out.println("NOTATION_DECLARATION"); break;
				case PROCESSING_INSTRUCTION: System.out.println("PROCESSING_INSTRUCTION"); break;
				case SPACE: System.out.println("SPACE"); break;
				case START_DOCUMENT: System.out.println("START_DOCUMENT"); break;
				case START_ELEMENT: System.out.println("START_ELEMENT"); break;
			}
			reader.next();
		} while(true);
	}
	
	public static void main(String[] args) {
		try {
			InputStream stream = new FileInputStream("cfg/db-config.xml");
			try {
				ConfigurationParser parser = new ConfigurationParser();
				parser.parse(stream);
				System.out.println(parser.getDrivers());
				System.out.println(parser.getConnections());
			} finally {
				stream.close();
			}
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	
}
