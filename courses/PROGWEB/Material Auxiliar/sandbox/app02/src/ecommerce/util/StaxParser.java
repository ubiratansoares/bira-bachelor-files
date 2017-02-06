package ecommerce.util;

import java.util.*;
import javax.xml.stream.*;

import static javax.xml.stream.XMLStreamConstants.*;

/**
 * Classe para manipular símbolos terminais de uma gramática em XML
 * usando StAX.
 */
public class StaxParser {

	/**
	 * Ignora espaços em branco e comentários.
	 */
	public void step() throws XMLStreamException {
		while(reader.hasNext()) {
			switch(reader.next()) {
				case CHARACTERS:
					if(!reader.isWhiteSpace()) return;
					break;
					
				case COMMENT: 	break;
				default:		return;
			}
		}
	}
	
	/**
	 * Espera um dado tipo de evento. Caso não encontre, lança uma exceção.
	 * 
	 * @param type
	 * O tipo de evento esperado (do {@link javax.xml.stream.XMLStreamConstants}).
	 */
	public void requireEvent(int type) throws XMLStreamException {
		if(reader.getEventType() != type) 
			throw new XMLStreamException("Invalid XML.", reader.getLocation());
	}
	
	/**
	 * Espera um dado tipo de evento. Caso não encontre, retorna {@code false}.
	 * 
	 * @param type
	 * O tipo de evento esperado (do {@link javax.xml.stream.XMLStreamConstants}).
	 */
	public boolean acceptEvent(int type) throws XMLStreamException {
		return (reader.getEventType() == type);
	}
	
	/**
	 * Espera uma abertura de tag com dado nome. Caso não encontre, lança uma exceção.
	 * 
	 * @param tagName
	 * O nome de tag esperado (esta implementação ignora namespaces).
	 */
	public void requireStartTag(String tagName) throws XMLStreamException {
		if(reader.getEventType() != START_ELEMENT || !reader.getLocalName().equals(tagName)) 
			throw new XMLStreamException("Invalid XML.", reader.getLocation());
	}
	
	/**
	 * Espera uma abertura de tag opcional com dado nome. Caso não encontre, retorna {@code false}.
	 * 
	 * @param tagName
	 * O nome de tag esperado (esta implementação ignora namespaces).
	 */
	public boolean acceptStartTag(String tagName) throws XMLStreamException {
		return (reader.getEventType() == START_ELEMENT) && reader.getLocalName().equals(tagName);
	}
	
	/**
	 * Espera um fechamento de tag com dado nome. Caso não encontre, lança uma exceção.
	 * 
	 * @param tagName
	 * O nome de tag esperado (esta implementação ignora namespaces).
	 */
	public void requireEndTag(String tagName) throws XMLStreamException {
		if(reader.getEventType() != END_ELEMENT || !reader.getLocalName().equals(tagName)) 
			throw new XMLStreamException("Invalid XML.", reader.getLocation());
	}
	
	/**
	 * Espera um fechamento de tag com dado nome. Caso não encontre, retorna {@code false}.
	 * 
	 * @param tagName
	 * O nome de tag esperado (esta implementação ignora namespaces).
	 */
	public boolean acceptEndTag(String tagName) throws XMLStreamException {
		return (reader.getEventType() == END_ELEMENT && reader.getLocalName().equals(tagName));
	}
	
	/**
	 * Espera uma sequência de caracteres. Caso não encontre, lança uma exceção.
	 * 
	 * @return
	 * A sequência de caracteres lida até então.
	 */
	public String requireCharacters() throws XMLStreamException {
		if(reader.getEventType() != CHARACTERS) 
			throw new XMLStreamException("Invalid XML.", reader.getLocation());
		return this.readCharacters();
	}
	
	/**
	 * Espera uma sequência de caracteres. Caso não encontre, retorna uma cadeia vazia.
	 * 
	 * @return
	 * A sequência de caracteres lida até então. Pode ser vazia.
	 */
	public String acceptCharacters() throws XMLStreamException {
		if(reader.getEventType() != CHARACTERS) return "";
		return this.readCharacters();
	}
	
	/**
	 * Lê a sequência de caracteres entre a última tag (início ou fim) e a
	 * posição atual (provavelmente o início ou o fim de alguma tag).
	 * 
	 * @return
	 * A sequência de caracteres lida até então. Pode ser vazia.
	 */
	private String readCharacters() throws XMLStreamException {
		StringBuilder text = new StringBuilder();
		do {
			switch(reader.getEventType()) {
				case CHARACTERS:
					if(!reader.isWhiteSpace())
						text.append(reader.getTextCharacters(), reader.getTextStart(), reader.getTextLength());
					else
						text.append(' ');
					break;
				
				case COMMENT:	break;
				default:		return text.toString();
			}
			reader.next();
		} while(reader.hasNext());
		return text.toString().trim();
	}
	
	/**
	 * Cria um mapa com os atributos encontrados na tag. O mapa será 
	 * usado pelo método {@link #requireAttribute} para encontrar os
	 * valores dos atributos.
	 * 
	 * @return
	 * Um mapa que indica a posição do atributo na tag, a partir do 
	 * nome do atributo.
	 */
	public Map<String, Integer> mapAttributes() throws XMLStreamException {
		Map<String, Integer> map = new HashMap<String, Integer>(reader.getAttributeCount());
		for(int i = 0; i < reader.getAttributeCount(); i++)
			map.put(reader.getAttributeLocalName(i), i);
		return map;
	}
	
	/**
	 * Espera a presença de um dado atributo na tag. Se o atributo foi omitido
	 * lançará uma exceção.
	 * 
	 * @return
	 * A sequência de caracteres lida do atributo.
	 */
	public String requireAttribute(Map<String, Integer> attrs, String attrName) 
			throws XMLStreamException {
		if(!attrs.keySet().contains(attrName))
			throw new XMLStreamException("Invalid XML.", reader.getLocation());
		return reader.getAttributeValue(attrs.get(attrName));
	}
	
	/**
	 * Espera a presença de um dado atributo na tag. Se o atributo foi omitido
	 * retornará {@code null}.
	 * 
	 * @return
	 * A sequência de caracteres lida do atributo, ou {@code null}.
	 */
	public String acceptAttribute(Map<String, Integer> attrs, String attrName) 
			throws XMLStreamException {
		if(!attrs.keySet().contains(attrName)) return null;
		return reader.getAttributeValue(attrs.get(attrName));
	}
	
	/**
	 * Lança uma exceção com a mensagem indicada.
	 */
	public void throwException(String message) throws XMLStreamException {
		throw new XMLStreamException(message, reader.getLocation());
	}
	
	/**
	 * Obtém o leitor XML associado ao analisador.
	 */
	public XMLStreamReader getReader() {
		return reader;
	}
	
	private XMLStreamReader reader;
	
	public StaxParser(XMLStreamReader reader) {
		this.reader = reader;
	}
	
}
