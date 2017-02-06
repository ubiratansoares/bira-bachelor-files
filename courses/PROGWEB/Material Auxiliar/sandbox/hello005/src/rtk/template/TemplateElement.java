package rtk.template;

/**
 * Elemento de um molde de página.
 */
public class TemplateElement {
	private Type type;
	private String value;
	
	/**
	 * Cria um elemento com tipo e valor. O valor pode ser
	 * um texto qualquer ou um identificador, dependendo do
	 * tipo do elemento.
	 */
	public TemplateElement(Type type, String value) {
		this.type = type;
		this.value = value;
	}
	
	/**
	 * Obtém o tipo do elemento. O tipo pode ser texto,
	 * instrução ou header.
	 */
	public Type getType() {
		return type;
	}
	
	/**
	 * Obtém o valor do elemento. O valor pode ser um fragmento
	 * de texto, um HTTP header ou um identificador, dependendo
	 * do elemento.
	 */
	public String getValue() {
		return value;
	}
	
	/**
	 * Representa o elemento textualmente (para depuração).
	 */
	@Override
	public String toString() {
		return String.format("%s:\"%s\"", type, value);
	}
	
	/**
	 * Tipos de elementos de molde. Além dos fragmentos de texto,
	 * existem elementos de instrução e elementos de header.
	 */
	public static enum Type {
		/** fragmento de texto do documento */	TEXT, 
		/** valor a ser substituído */			VAR, 
		/** início de bloco opcional */			OPTIONAL, 
		/** início de bloco de repetição */		LOOP, 
		/** início de bloco de seleção */		SELECT, 
		/** início de opção de seleção*/		OPTION, 
		/** fim de bloco (qualquer) */			END, 
		/** sugestão de header HTTP */			HEAD, 
		/** URI a ser substituído */			URI
	}
}
