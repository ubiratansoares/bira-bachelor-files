package compiler.t3b;

import compiler.*;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import static compiler.t3b.Symbol.Category;

/**
 * Tablea de símbolos usada durante a análise semântica.
 *
 */
public class SymbolsTable {

	private Map<String, Symbol> table;
	private Symbol programId;

	private int nextIndex;

	/**
	 * Cria uma tabela de símbolos.
	 */
	public SymbolsTable() {
		table = new HashMap<String, Symbol>();
		programId = null;
		nextIndex = 0;
	}

	/**
	 * Esvazia a tabela de símbolos.
	 */
	public void clear() {
		table.clear();
		programId = null;
		nextIndex = 0;
		return;
	}

	public Symbol createNullIdentifier() {
		Symbol nullId = new Symbol(Token.createIdentifier(
				String.format("?ID%04d?", nextIndex)));
		nextIndex++;
		return nullId;
	}

	/**
	 * Verifica se a entidade pode ser declarada com o
	 * nome e o escopo especificados. O identificador
	 * de programa só pode ser declarado uma vez,
	 * independentemente do nome usado na declaração.
	 *
	 * @param symbol
	 * O símbolo a ser verificado. Deve especificar o
	 * nome, a categoria (para identificador de programa)
	 * e o escopo da declaração.
	 *
	 * @return
	 * {@code true} caso o identificador possa ser
	 * declarado.
	 */
	public boolean canDeclare(Symbol symbol) {
		if(symbol.getCategory() == Category.PROGRAM) {
			return (programId == null);
		} else {
			return !table.containsKey(symbol.getFullName());
		}
	}

	/**
	 * Adiciona um identificador à tabela de símbolos.
	 *
	 * @param symbol
	 * O símbolo a ser adicionado como identificador.
	 *
	 * @throws SemanticException
	 * Lançada caso se tente uma redeclaração.
	 */
	public void declare(Symbol symbol) throws SemanticException {
		//verificar se o identificador pode ser declarado
		if(!this.canDeclare(symbol)) {
			throw new SemanticException(
					"Identificador já utilizado em outra declaração.");
		}

		if(symbol.getCategory() == Category.PROGRAM) {
			//declarar nome do programa
			symbol.setScope(null);
			table.put(symbol.getFullName(), symbol);
			programId = symbol;
		} else {
			//declarar procedimento, parâmetro ou variável
			table.put(symbol.getFullName(), symbol);
			if(symbol.getCategory() == Category.PARAMETER) {
				String altName = symbol.getScope().getFullName();
				altName = altName + '/' + symbol.getIndex();
				table.put(altName, symbol);
			}
		}
		return;
	}

	/**
	 * Verifica se o símbolo representa uma entidade já declarada.
	 *
	 * @param scope
	 * O escopo inicial do símbolo a ser verificado.
	 *
	 * @param ident
	 * O identificador do símbolo a ser verificado.
	 *
	 * @return
	 * {@code true} caso exista alguma entidade declarada com o
	 * nome e o escopo especificados, ou em algum escopo mais amplo.
	 */
	public boolean hasDeclared(Symbol scope, Token ident) {
		Symbol s = new Symbol(scope, ident);
		while(!table.containsKey(s.getFullName())) {
			if(s.getScope() != null) {
				s.setScope(s.getScope().getScope());
			} else {
				return false;
			}
		}
		return true;
	}

	/**
	 * Obtém o símbolo identificado pelo nome e escopo especificados.
	 *
	 * @param scope
	 * O escopo do símbolo desejdo.
	 *
	 * @param ident
	 * O identificador do símbolo desejado.
	 *
	 * @return
	 * O símbolo correspondente na tabela.
	 *
	 * @throws SemanticException
	 * Lançada caso o símbolo não esteja declarado.
	 */
	public Symbol getElement(Symbol scope, Token ident)
			throws SemanticException {
		Symbol s = new Symbol(scope, ident);
		while(!table.containsKey(s.getFullName())) {
			if(s.getScope() != null) {
				s.setScope(s.getScope().getScope());
			} else {
				throw new SemanticException("Identificador não declarado.");
			}
		}
		return table.get(s.getFullName());
	}

	/**
	 * Obtém a instância do símbolo na tabela, através das
	 * informações do símbolo dado. Apenas o escopo e o
	 * identificador são usados para recuperar essa instância.
	 *
	 * @param symbol
	 * O símbolo desejdo.
	 *
	 * @return
	 * A instância do símbolo na tabela.
	 *
	 * @throws SemanticException
	 * Lançada caso o símbolo não esteja declarado.
	 */
	public Symbol getElement(Symbol symbol)
			throws SemanticException {
		return this.getElement(symbol.getScope(), symbol.getToken());
	}

	/**
	 * Obtém o parâmetro identificado pelo índice e escopo especificados.
	 *
	 * @param scope
	 * O escopo do procedimento que possui o parâmetro desejdo.
	 *
	 * @param procId
	 * O identificador do procedimento que possui o parâmetro desejdo.
	 *
	 * @param index
	 * O índice do procedimento na assinatura do procedimento.
	 *
	 * @return
	 * O símbolo correspondente na tabela.
	 *
	 * @throws SemanticException
	 * Lançada caso o símbolo não esteja declarado.
	 */
	public Symbol getParameter(Symbol scope, Token procId, int index)
			throws SemanticException {
		Symbol proc = this.getElement(scope, procId);
		String altName = proc.getFullName() + '/' + index;
		if(table.containsKey(altName)) {
			return table.get(altName);
		} else {
			throw new SemanticException("Parâmetro não declarado.");
		}
	}

	/**
	 * Conta o número de variáveis declaradas no escopo dado.
	 *
	 * @param scope
	 * O escopo onde buscar as variáveis.
	 *
	 * @return
	 * O número de variáveis declaradas no escopo dado.
	 */
	public int getVarCount(Symbol scope) {
		Set<Symbol> set = new HashSet<Symbol>();
		for(Symbol element : table.values()) {
			//"set" não permite elementos repetidos
			if(element.getCategory() == Category.VARIABLE
					&& element.getScope() == scope)
				set.add(element);
		}
		return set.size();
	}

	public void print() {
		System.out.println("[start]");
		for(String key : table.keySet()) {
			System.out.printf("%-20s| %s%n", key, table.get(key));
		}
		System.out.println("[end]");
	}

}
