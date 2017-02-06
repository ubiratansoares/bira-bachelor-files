package compiler.t3b;

import compiler.*;
import compiler.t3a.*;

import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

/**
 * Atributos usados para análise semântica.
 *
 */
public class Attributes {

	private Map<Name, Object> map;

	/**
	 * Nomes válidos de atributos.
	 */
	public static enum Name {
		SCOPE, IDENTIFIER, TYPE, OPERATOR,
		INDEX, NEXT_INDEX, VAR_LIST
	}

	/**
	 * Operadores encontrados na linguagem LALG.
	 */
	public static enum Operator {
		REL_EQ, REL_NE, REL_GE, REL_LE, REL_GT, REL_LT,
		ADDITION, SUBTRACTION, MULTIPLICATION, DIVISION,
		UNARY_PLUS, UNARY_MINUS, NULL
	}

	/**
	 * Cria um mapeamento de atributos.
	 */
	public Attributes() {
		this.map = new HashMap<Name, Object>();
	}

	/**
	 * Exclui todos os mapeamentos.
	 */
	public void clear() {
		if(this.hasDefined(Name.VAR_LIST)) {
			List list = (List) map.get(Name.VAR_LIST);
			list.clear();
		}
		map.clear();
		return;
	}

	/**
	 * Verifica se o atributo está definido.
	 *
	 * @param key
	 * O nome do atributo a ser verificado.
	 *
	 * @return
	 * {@code true} caso o atributo esteja definido.
	 */
	public boolean hasDefined(Name key) {
		return map.containsKey(key);
	}

	/**
	 * Define um escopo.
	 *
	 * @param ident
	 * O identificador que define o escopo.
	 */
	public void setScope(Symbol ident) {
		map.put(Name.SCOPE, ident);
		return;
	}

	/**
	 * Obtém um escopo.
	 *
	 * @return
	 * O identificador que define o escopo.
	 */
	public Symbol getScope() {
		return (Symbol) map.get(Name.SCOPE);
	}

	/**
	 * Define um identificador associado ao contexto.
	 *
	 * @param token
	 * O identificador associado ao contexto.
	 */
	public void setToken(Token token) {
		map.put(Name.IDENTIFIER, token);
		return;
	}

	/**
	 * Obtém um identificador associado ao contexto.
	 *
	 * @return
	 * O identificador associado ao contexto.
	 */
	public Token getToken() {
		return (Token) map.get(Name.IDENTIFIER);
	}

	/**
	 * Define um tipo de dado.
	 *
	 * @param type
	 * O tipo de dado.
	 */
	public void setType(DataType type) {
		map.put(Name.TYPE, type);
		return;
	}

	/**
	 * Obtém um tipo de dado.
	 *
	 * @return
	 * O tipo de dado.
	 */
	public DataType getType() {
		return (DataType) map.get(Name.TYPE);
	}

	/**
	 * Define um operador relacional ou aritmético.
	 *
	 * @param operator
	 * O operador aritmético ou relacional.
	 */
	public void setOperator(Operator operator) {
		map.put(Name.OPERATOR, operator);
		return;
	}

	/**
	 * Obtém um operador relacional ou aritmético.
	 *
	 * @return
	 * O operador aritmético ou relacional.
	 */
	public Operator getOperator() {
		return (Operator) map.get(Name.OPERATOR);
	}

	/**
	 * Define o índice de elemento de lista.
	 *
	 * @param index
	 * O índice do elemento na lista.
	 */
	public void setIndex(int index) {
		map.put(Name.INDEX, index);
		return;
	}

	/**
	 * Obtém o índice de elemento de lista.
	 *
	 * @return
	 * O índice do elemento na lista.
	 */
	public Integer getIndex() {
		return (Integer) map.get(Name.INDEX);
	}

	/**
	 * Define o índice do próximo elemento de lista.
	 *
	 * @param index
	 * O índice do próximo elemento na lista.
	 */
	public void setNextIndex(int index) {
		map.put(Name.INDEX, index);
		return;
	}

	/**
	 * Obtém o índice do próximo elemento de lista.
	 *
	 * @return
	 * O índice do próximo elemento na lista.
	 */
	public Integer getNextIndex() {
		return (Integer) map.get(Name.INDEX);
	}

	/**
	 * Acrescenta uma variável ao fim da lista de variáveis.
	 *
	 * @param var
	 * A variável a ser adicionada.
	 */
	public void addVariable(Symbol var) {
		if(!this.hasDefined(Name.VAR_LIST))
			map.put(Name.VAR_LIST, new LinkedList<Symbol>());
		List<Symbol> list = (List<Symbol>) map.get(Name.VAR_LIST);
		list.add(var);
		return;
	}

	/**
	 * Retorna uma lista (imutável) de variáveis.
	 *
	 * @return
	 * Uma lista de variáveis.
	 */
	public List<Symbol> getVariables() {
		if(this.hasDefined(Name.VAR_LIST)) {
			List<Symbol> list = (List<Symbol>) map.get(Name.VAR_LIST);
			return Collections.unmodifiableList(list);
		} else
			return Collections.emptyList();
	}

}
