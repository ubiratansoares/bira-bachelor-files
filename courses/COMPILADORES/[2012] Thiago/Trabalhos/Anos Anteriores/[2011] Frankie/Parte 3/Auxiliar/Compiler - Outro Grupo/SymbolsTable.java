import java.util.ArrayList;

public class SymbolsTable {
	
	private ArrayList<Symbol> symbols; // A lista de simbolos.
	
	/**
	 * O construtor padrao.
	 */
	public SymbolsTable() {
		
		this.symbols = new ArrayList<Symbol>();
		
		Symbol symbol = new Symbol();
		symbol.setId("int");
		symbol.setLevel(0);
		symbol.setType(Type.INTEGER);
		symbol.setBytesNumber(2);
		symbol.setCategory(Category.TYPE);
		
		this.symbols.add(symbol);
		
		symbol = new Symbol();
		symbol.setId("boolean");
		symbol.setLevel(0);		
		symbol.setType(Type.BOOLEAN);
		symbol.setBytesNumber(1);
		symbol.setCategory(Category.TYPE);
		
		this.symbols.add(symbol);
		
		symbol = new Symbol();
		symbol.setId("true");
		symbol.setLevel(0);		
		symbol.setType(Type.BOOLEAN);
		symbol.setBytesNumber(1);
		symbol.setCategory(Category.CONSTANT);
		
		this.symbols.add(symbol);
		
		symbol = new Symbol();
		symbol.setId("false");
		symbol.setLevel(0);		
		symbol.setType(Type.BOOLEAN);
		symbol.setBytesNumber(1);
		symbol.setCategory(Category.CONSTANT);
		
		this.symbols.add(symbol);
		
		symbol = new Symbol();
		symbol.setId("read");
		symbol.setLevel(0);		
		symbol.setCategory(Category.PROCEDURE);
		symbol.setParametersNumber(1);
		
		this.symbols.add(symbol);
		
		symbol = new Symbol();
		symbol.setId("write");
		symbol.setLevel(0);		
		symbol.setCategory(Category.PROCEDURE);
		symbol.setParametersNumber(1);
		
		this.symbols.add(symbol);
	}
	
	/**
	 * Verifica se o simbolo cujo id e igual a "id" ja esta presente na tabela de simbolos.
	 * 
	 * @param id o id to simbolo a ser procurado.
	 * @param nivelCorrente o nivel do simbolo a ser procurado.
	 * @return "true" se o simbolo ja estiver presente e "false" caso contrario.
	 */
	public boolean isDeclared(String id, Integer level) {

		boolean FOUND = false;
		
		for (int i = 0; i < this.symbols.size() && FOUND == false; i++) {
			
			Symbol symbol = this.symbols.get(i);
			
			if (symbol.getId().equals(id) && symbol.getLevel() == level) {
				FOUND = true;
			}
		}

		return FOUND;
	}

	/**
	 * Inseri um novo simbolo com id "id" na tabela de simbolos.
	 *  
	 * @param id
	 * @param ref
	 */
	public void insertSymbol(String id) {
		
		Symbol symbol = new Symbol();
		symbol.setId(id);
		
		this.symbols.add(symbol);
	}

	/**
	 * Atualiza os campos "level", "category" e "type" de todos os simbolos cuja categoria ainda nao foi definida.
	 * 
	 * @param nivelCorrente o nivel do simbolo.
	 * @param categoria a categoria do simbolo.
	 * @param type o tipo do simbolo. 
	 */
	public void updateSymbol(Integer level, Category category, Type type) {

		for (int i = 0; i < this.symbols.size(); i++) {

			if (this.symbols.get(i).getCategory() == null) {
				
				this.symbols.get(i).setLevel(level);
				this.symbols.get(i).setCategory(category);
				this.symbols.get(i).setType(type);
			}
		}
	}

	/**
	 * Atualiza os campos nivel e categoria do ultimo simbolo da lista.
	 * 
	 * @param level o nivel do simbolo.
	 * @param category a categoria do simbolo.
	 */
	public void updateSymbol(Integer level, Category category){
		
		this.symbols.get(this.symbols.size() - 1).setLevel(level);
		this.symbols.get(this.symbols.size() - 1).setCategory(category);
	}

	/**
	 * Atualiza os campos nivel, categoria e classe de transferencia do ultimo simbolo da lista.
	 * 
	 * @param level o nivel do simbolo.
	 * @param category a categoria do symbolo.
	 * @param transferenceClass a classe de transferencia do simbolo.
	 */
	public void updateSymbol(Integer level, Category category, ParameterPassingMode transferenceClass){

		this.symbols.get(this.symbols.size() - 1).setLevel(level);
		this.symbols.get(this.symbols.size() - 1).setCategory(category);
		this.symbols.get(this.symbols.size() - 1).setTransferenceClass(transferenceClass);
	}

	/**
	 * Remove todos os simbols cujo nivel for igual a "level".
	 * 
	 * @param level o nivel dos simbolos que serao removidos
	 */
	public void removeSymbols(Integer level) {

		for (int i = 0; i < this.symbols.size(); i++) {

			if (this.symbols.get(i).getLevel() == level) {

				this.symbols.get(i).setId("");
				this.symbols.get(i).setLevel(-1);
				this.symbols.get(i).setBytesNumber(0);
				this.symbols.get(i).setParametersNumber(0);
				this.symbols.get(i).setCategory(null);
			}
		}
	}

	/**
	 * Busca pelo id "id" na tabela de simbolos e retorna a sua posicao.
	 * Caso o id nao seja encontrado, o metodo retorna NULL.
	 * 
	 * @param id o "id" do simbolo a ser procurado.
	 * @return a posicao do simbolo se ele for encontrado, caso contrario "null".
	 */
	public Integer findId(String id) {

		for (int i = 0; i < this.symbols.size(); i++) {

			if (this.symbols.get(i).getId().equals(id)) {
				
				return new Integer(i);
			}
		}
		
		return null;
	}	
	
	/**
	 * Retorna o simbolo que esta na posicao "index".
	 * 
	 * @param index a posicao do simbolo.
	 * @return symbol o simbolo.
	 */
	public Symbol getSymbol(int index) {
		
		return this.symbols.get(index);
	}
	
	/**
	 * Retorna a posicao do ultimo simbolo na tabela de simbolos
	 * 
	 * @return int a posicao do ultimo simbolo na tabela de simbolos.
	 */
	public int getLastSymbolPosition() {
		
		return this.symbols.size() - 1;
	}
}
