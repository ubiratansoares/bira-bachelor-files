package compiler.t3b;

import compiler.Token;
import compiler.t3a.*;

/**
 * Entrada da tabela de símbolos.
 *
 */
public class Symbol {

	private Symbol scope;
	private Token token;
	private Category category;
	private Integer index;
	private DataType type;
	private Object value;

	private int address;

	/**
	 * Cria um símbolo.
	 *
	 * @param token
	 * O token que representa o símbolo.
	 */
	public Symbol(Token token) {
		this.token = token;
	}

	/**
	 * Cria um símbolo.
	 *
	 * @param scope
	 * O escopo do símbolo.
	 *
	 * @param token
	 * O token que representa o símbolo.
	 */
	public Symbol(Symbol scope, Token token) {
		this.scope = scope;
		this.token = token;
	}

	/**
	 * Constrói um nome único usando informações de escopo.
	 *
	 * @return
	 * Uma cadeia que representa unicamente este identificador.
	 */
	public String getFullName() {
		String fullname = "/" + this.getToken().getString();
		Symbol sc = this.getScope();
		while(sc != null) {
			fullname = "/" + sc.getToken().getString() + fullname;
			sc = sc.getScope();
		}
		if(this.category == Category.PROGRAM)
			fullname = "/" + this.getToken().getString() + fullname;
		return fullname;
	}

	/**
	 * Categorias de identificadores.
	 */
	public static enum Category {
		PROGRAM, PROCEDURE, PARAMETER, VARIABLE
	}

	/**
	 * Obtém a categoria do identificador.
	 *
	 * @return
	 * A categoria do identificador. Caso não 
	 * esteja definida, retorna {@code null}.
	 */
	public Category getCategory() {
		return category;
	}

	/**
	 * Define a categoria do identificador.
	 *
	 * @param category
	 * A categoria do identificador.
	 */
	public void setCategory(Category category) {
		this.category = category;
	}

	/**
	 * Obtém o índice do identificador. Esse índice indica
	 * a posição do identificador na lista de parâmetros ou
	 * argumentos de um procedimento. 
	 *
	 * @return
	 * O índice do identificador. Caso não
	 * esteja definido, retorna {@code null}.
	 */
	public Integer getIndex() {
		return index;
	}

	/**
	 * Define o índice do identificador. Esse índice indica 
	 * a posição do identificador na lista de parâmetros ou
	 * argumentos de um procedimento. 
	 *
	 * @param index
	 * O índice do identificador.
	 */
	public void setIndex(int index) {
		this.index = index;
	}

	/**
	 * Obtém o token do identificador.
	 *
	 * @return
	 * O token do identificador. Caso não
	 * esteja definido, retorna {@code null}.
	 */
	public Token getToken() {
		return token;
	}

	/**
	 * Define o token do identificador.
	 *
	 * @param token
	 * O token do identificador.
	 */
	public void setToken(Token token) {
		this.token = token;
	}

	/**
	 * Obtém o escopo do identificador. 
	 *
	 * @return
	 * O escopo do identificador. Caso não
	 * esteja definido, retorna {@code null}.
	 */
	public Symbol getScope() {
		return scope;
	}

	/**
	 * Define o escopo do identificador. 
	 *
	 * @param scope
	 * O escopo do identificador.
	 */
	public void setScope(Symbol scope) {
		this.scope = scope;
	}

	/**
	 * Obtém o tipo de dados do símbolo.
	 *
	 * @return
	 * O tipo de dados do símbolo. Caso não
	 * esteja definido, retorna {@code null}.
	 */
	public DataType getType() {
		return type;
	}

	/**
	 * Define o tipo de dados do símbolo.
	 *
	 * @param type
	 * O tipo de dados do símbolo.
	 */
	public void setType(DataType type) {
		this.type = type;
	}

	/**
	 * Obtém o valor do símbolo.
	 *
	 * @return
	 * O valor do símbolo. Caso não esteja
	 * definido, retorna {@code null}.
	 */
	public Object getValue() {
		return value;
	}

	/**
	 * Define o valor do símbolo.
	 *
	 * @param value
	 * O valor do símbolo.
	 */
	public void setValue(Object value) {
		this.value = value;
	}

	/**
	 * Obtém o endereço da variável ou procedimento.
	 *
	 * @return
	 * O endereço da variável ou procedimento na memória.
	 * Caso não esteja definido, retorna {@code null}.
	 */
	public int getAddress() {
		return address;
	}

	/**
	 * Define o endereço da variável ou procedimento.
	 *
	 * @param address
	 * O endereço da variável ou procedimento na memória.
	 * Caso não esteja definido, retorna {@code null}.
	 */
	public void setAddress(int address) {
		this.address = address;
	}

	@Override
	public String toString() {
		String cat;
		if(category != null) {
			switch(category) {
				case PROGRAM: cat = "program"; break;
				case PROCEDURE: cat = "procedure"; break;
				case PARAMETER: cat = "parameter"; break;
				case VARIABLE: cat = "variable"; break;
				default: cat = "undefined"; break;
			}
		} else {
			cat = "null";
		}
		return String.format("%-10s %4d", cat, address);
	}

}
