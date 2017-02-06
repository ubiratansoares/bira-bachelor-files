package rtk.database;

import rtk.util.*;

import java.io.*;
import java.sql.*;
import java.util.*;
import javax.xml.stream.*;

import static javax.xml.stream.XMLStreamConstants.*;

/**
 * Construtor genérico de comandos SQL. Constrói comandos a
 * partir de moldes em XML, e associa argumentos usando nomes.
 * 
 * STATEMENT = <statement> VAR+ SQL </statement> .
 * VAR = <var id="ID" type="DTYPE" (retrieve="COLUMN")?/> .
 * ID = identificador interno do programa.
 * DTYPE = integer | string | _TODO_ .
 * COLUMN = nome da coluna na tabela.
 * SQL = <sql type="STYPE"> (SQL_TEXT | VAR_REF)* </sql> .
 * STYPE = dml | select .
 * SQL_TEXT = trecho de código em SQL.
 * VAR_REF = <var id="ID"/> .
 */
public class CustomStatementBuilder {
	
	/**
	 * Define o comando SQL usando um valor personalizado. A sintaxe 
	 * é a mesma do formato XML, exceto que o elemento raiz é "query", 
	 * sem atributos (os atributos serão ignorados).
	 * 
	 * @param template
	 * O molde do comando no formato XML.
	 */
	public SQLStatement parse(String template) throws XMLStreamException {
		Reader reader = new StringReader(template);
		XMLInputFactory xif = XMLInputFactory.newFactory();
		XMLStreamReader xin = xif.createXMLStreamReader(reader);
		try {
			StaxParser p = new StaxParser(xin);
			
			p.requireEvent(START_DOCUMENT);
			p.step();
			
			SQLStatement st = this.parse_statement(p);
			
			p.requireEvent(END_DOCUMENT);
			return st;
		} catch(NoSuchElementException e) {
			throw new XMLStreamException("Invalid XML.", xin.getLocation());
		} finally {
			try {
				xin.close();
			} finally {
				try {
					reader.close();
				} catch(IOException e) {
					e.printStackTrace(); //não deve ocorrer
				}
			}
		}
	}
	
	/**
	 * Processa o elemento XML "statement", ignorando seus atributos. 
	 * A stream já deverá estar aberta e posicionada no primeiro 
	 * evento não-ignorável. O método não fechará a stream.
	 * 
	 * STATEMENT = <statement> VAR+ SQL </statement> .
	 */
	public SQLStatement parse_statement(StaxParser p) 
			throws XMLStreamException {
		p.requireStartTag("statement");
		p.step();
		
		this.parse_var(p);
		
		while(!p.acceptStartTag("sql"))
			this.parse_var(p);
		
		SQLStatement st = this.parse_sql(p);
		
		p.requireEndTag("statement");
		p.step();
		
		for(String id : vars.keySet()) {
			Variable v = vars.get(id);
			st.setParameter(id, v.type);
			if(v.name != null)
				st.mapOutParameter(id, v.name);
		}
		return st;
	}
	
	// VAR = <var id="ID" type="DTYPE" (retrieve="COLUMN")?/> .
	// DTYPE = integer | string | _TODO_ .
	private void parse_var(StaxParser p) throws XMLStreamException {
		p.requireStartTag("var");
		Map<String, Integer> attr = p.mapAttributes();
		
		Variable v = new Variable();
		String id = p.requireAttribute(attr, "id");
		String type = p.requireAttribute(attr, "type");
		v.name = p.acceptAttribute(attr, "retrieve");
		p.step();
		
		p.requireEndTag("var");
		p.step();
		
		if(types.containsKey(type))
			v.type = types.get(type.toLowerCase());
		else
			p.throwException("Invalid XML.");
		vars.put(id, v);
	}
	
	// SQL = <sql type="STYPE"> (SQL_TEXT | VAR_REF)* </sql> .
	// STYPE = dml | select .
	private SQLStatement parse_sql(StaxParser p) throws XMLStreamException {
		p.requireStartTag("sql");
		Map<String, Integer> attr = p.mapAttributes();
		String type = p.requireAttribute(attr, "type");
		p.step();
		
		SQLStatement st = null;
		if(type.equals("dml")) {
			st = new DMLStatement();
		} else if(type.equals("select")) {
			st = new SelectStatement();
		} else
			p.throwException("Invalid XML.");
		
		StringBuilder sb = new StringBuilder();
		XMLStreamReader r = p.getReader();
		while(p.acceptStartTag("var") || p.acceptEvent(CHARACTERS)) {
			sb.append(p.acceptCharacters());
			String id = this.parse_ref(p);
			if(id != null) {
				if(vars.containsKey(id)) {
					st.mapInParameter(id);
					sb.append('?');
				} else 
					p.throwException("Invalid XML.");
			}
		}
		
		p.requireEndTag("sql");
		p.step();
		
		st.setTemplate(sb.toString().replaceAll("\\s+", " "));
		return st;
	}
	
	// VAR_REF = <var id="ID"/> .
	private String parse_ref(StaxParser p) throws XMLStreamException {
		if(p.acceptStartTag("var")) {
			Map<String, Integer> attr = p.mapAttributes();
			String id = p.requireAttribute(attr, "id");
			p.step();
			
			p.requireEndTag("var");
			p.step();
			return id;
		} else
			return null;
	}
	
	/* == atributos/construtores == */
	
	private static final Map<String, Class<?>> types = 
		new HashMap<String, Class<?>>();
		
	static {
		types.put("boolean", Boolean.class);
		types.put("integer", Integer.class);
		types.put("string", String.class);
	}
	
	private Map<String, Variable> vars;
	
	/**
	 * Construtor vazio.
	 */
	public CustomStatementBuilder() {
		this.vars = new HashMap<String, Variable>();
	}
	
	/* == classes aninhadas == */
	
	private class Variable {
		public Class<?> type;
		public String name;		//only for out params
		
		@Override
		public String toString() {
			return String.format("{name=%s, type=%s}", name, type);
		}
	}
	
}
