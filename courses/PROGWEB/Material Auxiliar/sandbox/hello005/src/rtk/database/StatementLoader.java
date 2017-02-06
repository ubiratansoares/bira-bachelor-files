package rtk.database;

import rtk.util.*;

import java.io.*;
import java.net.*;
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
public class StatementLoader {
	
	/**
	 * Carrega um arquivo de configuração com comandos SQL.
	 * 
	 * @param location
	 * O local do arquivo de configuração a ser carregado.
	 */
	public void load(URL location) 
			throws XMLStreamException, IOException {
		Reader reader = new BufferedReader(
				new InputStreamReader(location.openStream()));
		try {
			this.statements.clear();
			this.parse(reader);
		} finally {
			reader.close();
		}
	}
	
	/**
	 * Carrega um arquivo de configuração com comandos SQL.
	 * 
	 * @param filename
	 * O nome do arquivo de configuração a ser carregado.
	 */
	public void parse(Reader in) throws XMLStreamException {
		XMLInputFactory factory = XMLInputFactory.newFactory();
		XMLStreamReader reader = factory.createXMLStreamReader(in);
		try {
			StaxParser p = new StaxParser(reader);
			
			this.valid = false;
			p.requireEvent(START_DOCUMENT);
			p.step();
			
			this.parse_statements(p);
			
			p.requireEvent(END_DOCUMENT);
			this.valid = true;
		} catch(NoSuchElementException e) {
			throw new XMLStreamException("Invalid XML.", reader.getLocation());
		} finally {
			reader.close();
		}
	}
	
	// MODEL = <statements database="ID" dialect="ID"> STATEMENT* </statements>
	private void parse_statements(StaxParser p) 
			throws XMLStreamException {
		p.requireStartTag("statements");
		Map<String, Integer> attr = p.mapAttributes();
		this.dbname = p.acceptAttribute(attr, "database");
		this.dialect = p.acceptAttribute(attr, "dialect");
		p.step();
		
		while(p.acceptStartTag("statement"))
			this.parse_statement(p);
		
		p.requireEndTag("statements");
		p.step();
		
		return;
	}
	
	// STATEMENT = <statement id="ID" parser="(basic|custom)"> CONTENT </statement>
	private void parse_statement(StaxParser p) throws XMLStreamException {
		p.requireStartTag("statement");
		Map<String, Integer> attr = p.mapAttributes();
		
		String id = p.requireAttribute(attr, "id");
		String parser = p.requireAttribute(attr, "parser");
		if(parser.equals("basic")) {
			BasicStatementParser stb = new BasicStatementParser();
			Map<String, SQLStatement> stmap = stb.parse_statement(p);
			for(String suffix : stmap.keySet()) {
				String full_id = String.format("%s.%s", id, suffix);
				statements.put(full_id, stmap.get(suffix));
			}
		} else if(parser.equals("custom")) {
			SQLStatement st = new CustomStatementBuilder().parse_statement(p);
			statements.put(id, st);
		} else 
			p.throwException("Invalid XML.");
	}
	
	/* == getters ==*/
	
	public String getDatabaseRef() {
		return dbname;
	}
	
	public String getSQLDialect() {
		return dialect;
	}
	
	public SQLStatement getStatement(String id) {
		return statements.get(id);
	}
	
	/* == atributos/construtores == */
	
	private boolean valid;
	private String dbname;
	private String dialect;
	private Map<String, SQLStatement> statements;
	
	/**
	 * Construtor vazio.
	 */
	public StatementLoader() {
		this.valid = false;
		this.dbname = "default";
		this.dialect = "default";
		this.statements = new HashMap<String, SQLStatement>();
	}
	
}
