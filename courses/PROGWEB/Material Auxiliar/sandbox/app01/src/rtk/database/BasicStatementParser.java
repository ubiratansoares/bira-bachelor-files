package rtk.database;

import rtk.util.*;

import java.io.*;
import java.sql.*;
import java.util.*;
import javax.xml.stream.*;

import static javax.xml.stream.XMLStreamConstants.*;

/**
 * Construtor de comandos básicos SQL. Constrói comandos a
 * partir de moldes em XML, e associa argumentos usando nomes.
 * 
 * STATEMENT = <statement> TABLE COL+ BUILD+ </statement> .
 * TABLE = <table> NAME </table>
 * COL = <col id="ID" name="NAME" type="DTYPE" (pk="true")?/> .
 * ID = identificador interno do programa.
 * NAME = nome na base de dados.
 * DTYPE = integer | string | _TODO_ .
 * BUILD = <build id="(insert|update|select|delete)"> GEN* </sql> .
 * GEN = <gen id="ID"/> .
 */
public class BasicStatementParser {
	
	/**
	 * Processa o elemento XML "statement", ignorando seus atributos. 
	 * A stream já deverá estar aberta e posicionada no primeiro 
	 * evento não-ignorável. O método não fechará a stream.
	 * 
	 * STATEMENT = <statement> TABLE COL+ BUILD+ </statement> .
	 */
	public Map<String, SQLStatement> parse_statement(StaxParser p) 
			throws XMLStreamException {
		p.requireStartTag("statement");
		p.step();
		
		String name = this.parse_table(p);
		BasicStatementBuilder stb = new BasicStatementBuilder(name);
		
		Column c = this.parse_col(p);
		stb.setColumn(c.id, c.name, c.type);
		stb.setPrimaryKey(c.id, c.pk);
		while(p.acceptStartTag("col")) {
			c = this.parse_col(p);
			stb.setColumn(c.id, c.name, c.type);
			stb.setPrimaryKey(c.id, c.pk);
		}
		
		SQLStatement st; String[] str;
		Map<String, SQLStatement> stmap = new HashMap<String, SQLStatement>();
		
		str = this.parse_build(p);
		stb.setGeneratedKeys(); //clear
		for(int i = 1; i < str.length; i++)
			stb.setGeneratedKey(str[i], true);
		st = this.buildStatement(stb, str[0]);
		stmap.put(str[0], st);
		while(p.acceptStartTag("build")) {
			str = this.parse_build(p);
			stb.setGeneratedKeys(); //clear
			for(int i = 1; i < str.length; i++)
				stb.setGeneratedKey(str[i], true);
			st = this.buildStatement(stb, str[0]);
			stmap.put(str[0], st);
		}
		
		p.requireEndTag("statement");
		p.step();
		
		return stmap;
	}
	
	// TABLE = <table> NAME </table>
	private String parse_table(StaxParser p) throws XMLStreamException {
		p.requireStartTag("table");
		p.step();
		
		String name = p.requireCharacters();
		
		p.requireEndTag("table");
		p.step();
		
		return name;
	}
	
	// COL = <col id="ID" name="NAME" type="DTYPE" (pk="true")?/> .
	private Column parse_col(StaxParser p) throws XMLStreamException {
		p.requireStartTag("col");
		Map<String, Integer> attr = p.mapAttributes();
		String id = p.requireAttribute(attr, "id");
		String name = p.requireAttribute(attr, "name");
		String type = p.requireAttribute(attr, "type");
		String pk = p.acceptAttribute(attr, "pk");
		p.step();
		
		p.requireEndTag("col");
		p.step();
		
		Column col = new Column();
		col.id = id;
		col.name = name;
		col.type = types.get(type);
		col.pk = Boolean.valueOf(pk);
		return col;
	}
	
	// BUILD = <build id="(insert|update|select|delete)"> GEN* </sql> .
	// GEN = <gen id="ID"/> .
	private String[] parse_build(StaxParser p) throws XMLStreamException {
		List<String> str = new LinkedList<String>();
		
		p.requireStartTag("build");
		Map<String, Integer> attr = p.mapAttributes();
		str.add(p.requireAttribute(attr, "id"));
		p.step();
		
		while(p.acceptStartTag("gen")) {
			attr = p.mapAttributes();
			str.add(p.requireAttribute(attr, "id"));
			p.step();
			
			p.requireEndTag("gen");
			p.step();
		}
		
		p.requireEndTag("build");
		p.step();
		return str.toArray(new String[0]);
	}
	
	private SQLStatement buildStatement(BasicStatementBuilder stb, String id) {
		if(id.equals("insert")) return stb.buildInsert();
		if(id.equals("delete")) return stb.buildDelete();
		if(id.equals("update")) return stb.buildUpdate();
		if(id.equals("select")) return stb.buildSelect();
		if(id.equals("select-all")) return stb.buildSelectAll();
		return null;
	}
	
	/* == atributos/construtores == */
	
	private static final Map<String, Class<?>> types = 
		new HashMap<String, Class<?>>();
		
	static {
		types.put("integer", Integer.class);
		types.put("string", String.class);
	}
	
	/* == classes aninhadas == */
	
	private class Column {
		public String id;
		public String name;
		public Class<?> type;
		public boolean pk;
	}
	
}
