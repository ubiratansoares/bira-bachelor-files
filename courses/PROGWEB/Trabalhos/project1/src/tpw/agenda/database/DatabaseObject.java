package tpw.agenda.database;

/**
 * Representa um objeto em banco de dados. Define operações básicas
 * de inserção, remoção, atualização e busca de tuplas. A interface
 * {@link java.lang.Cloneable} é necessária para que iteradores possam
 * ser facilmente gerados dos cursores.
 */
public interface DatabaseObject<T> extends Cloneable {
	
	//create, retrieve, update, delete
	
	/**
	 * Executa um comando de INSERT. O método não deve enviar valores 
	 * para campos gerados automaticamente (como sequências), e deve 
	 * coletá-los no próprio objeto após a execução do comando.
	 * 
	 * @throws DatabaseException
	 * Lançada caso haja um problema ao acessar a base ou caso haja uma 
	 * violação de qualquer restrição (como de chave primária, por exemplo).
	 */
	public void add() throws DatabaseException;
	
	/**
	 * Executa um comando de DELETE usando uma chave primária ou campo
	 * com restrição de unicidade. O método não lança exceção caso o
	 * registro não seja encontrado.
	 * 
	 * @return
	 * {@code true} caso um registro tenha sido encontrado e removido;
	 * {@code false} caso o registro não tenha sido encontrado.
	 * 
	 * @throws DatabaseException
	 * Lançada caso haja um problema ao acessar a base ou caso haja uma 
	 * violação de qualquer restrição (como de chave estrangeira, por 
	 * exemplo).
	 */
	public boolean remove() throws DatabaseException;

	/**
	 * Executa um comando de SELECT usando uma chave primária ou campo
	 * com restrição de unicidade. Como apenas um objeto deve ser retornado, 
	 * os dados obtidos devem ser coletados no próprio objeto, ao invés de
	 * em um cursor à parte.
	 * 
	 * @return
	 * {@code true} caso algum registro corresponda à chave buscada.
	 * 
	 * @throws DatabaseException
	 * Lançada caso haja um problema ao acessar a base.
	 */
	public boolean get() throws DatabaseException;
	
	/**
	 * Executa um comando de UPDATE usando uma chave primária ou campo
	 * com restrição de unicidade. Esse tipo de campo não deve ser 
	 * alterado por este método. O método não deve lançar exceção caso
	 * o registro não seja encontrado.
	 * 
	 * @return
	 * {@code true} caso um registro tenha sido encontrado e atualizado;
	 * {@code false} caso o registro não tenha sido encontrado.
	 * 
	 * @throws DatabaseException
	 * Lançada caso haja um problema ao acessar a mídia de persistência
	 * ou caso haja uma violação de qualquer restrição aos dados (como
	 * de chave estrangeira, por exemplo).
	 */
	public boolean set() throws DatabaseException;
	
	//retrieve lists
	
	/**
	 * Lista (através de um cursor) todos os registros encontrados na
	 * base que correspondam a esta classe.
	 * 
	 * @throws DatabaseException
	 * Lançada caso haja um problema ao acessar a base.
	 */
	public Cursor<T> list() throws DatabaseException;
	
	//helper methods
	
	/**
	 * Recupera todos os dados do objeto a partir de um cursor JDBC
	 * devidamente posicionado. Implementações do cursor poderão usar
	 * este método para contornar os métodos setters da superclasse
	 * (que poderão fazer verificações extras de segurança).
	 * 
	 * @param cursor
	 * O cursor JDBC já posicionado na tupla.
	 * 
	 * @throws DatabaseException
	 * Lançada caso haja um problema ao acessar a base.
	 */
	public void revive(java.sql.ResultSet cursor) throws DatabaseException;
	
	/**
	 * Recupera chaves automáticas do objeto a partir de um cursor JDBC 
	 * devidamente posicionado. As colunas correspondentes já serão 
	 * conhecidas pela implementação. A implementação deste método não 
	 * é obrigatória, a não ser que pretenda usar a classe {@link 
	 * DatabaseUtilities}.
	 * 
	 * @param cursor
	 * O cursor JDBC já posicionado na tupla.
	 * 
	 * @throws DatabaseException
	 * Lançada caso haja um problema ao acessar a base.
	 */
	public void reviveKeys(java.sql.ResultSet cursor) throws DatabaseException;
	
	//inner interfaces
	
	/**
	 * Permite iterar sobre uma lista de resultados de uma busca feita 
	 * na base. Não implementa a interface {@link java.util.Iterator}
	 * por si, mas exige a implementação da interface {@link java.lang.Iterable}, 
	 * que requer a implementação de um iterador e permite o uso do 'foreach'.
	 * 
	 * Além disso, o cursor não deve criar novas instâncias para cada item 
	 * da iteração. Ao invés disso, ele deve estender a classe dos dados 
	 * obtidos e oferecê-los pelos métodos getters da superclasse.
	 * 
	 * Por outro lado, o iterador gerado pelo cursor deve criar novas 
	 * instâncias para cada item recuperado. Obviamente, a lista gerada
	 * pelo método {@link #load()} também deve criar novas instâncias 
	 * para cada item recuperado.
	 */
	
	public interface Cursor<T> extends Iterable<T> {
		
		/**
		 * Avança o cursor para a próxima posição, recuperando os dados
		 * do novo registro. Antes de chamar este método pela primeira vez,
		 * o cursor deverá estar posicionado antes da primeira linha.
		 * 
		 * @return
		 * {@code true} caso o método tenha recuperado um registro válido;
		 * {@code false} caso não haja mais registros a recuperar.
		 * 
		 * @throws DatabaseException
		 * Lançada caso haja um problema ao acessar a base.
		 */
		public boolean next() throws DatabaseException;

		/**
		 * Avança o cursor algumas posições à frente. A operação não 
		 * precisa ser suportada pela implementação para todos os valores
		 * de entrada.
		 * 
		 * @return
		 * {@code true} caso o método tenha recuperado um registro válido;
		 * {@code false} caso o cursor tenha chegado a uma posição inválida
		 * (antes do início da lista ou após o fim da lista).
		 * 
		 * @throws DatabaseException
		 * Lançada caso haja um problema ao acessar a base.
		 */
		public boolean fastForward(int n) throws DatabaseException;
		
		/**
		 * Fecha o cursor, liberando recursos. Não é necessário liberar
		 * todos os recursos associados à base, mas deve liberar todos os 
		 * recursos associados exclusivamente ao cursor (statements ou
		 * result sets, mas não connections, por exemplo).
		 * 
		 * @throws DatabaseException
		 * Lançada caso haja um problema ao acessar a base.
		 */
		public void close() throws DatabaseException;

		/**
		 * Carrega todos os resultados remanescentes para a memória, formando
		 * uma lista. O método não precisa ser eficiente, e não deve estipular
		 * limites de tempo ou de memória.
		 * 
		 * @return
		 * Uma lista com todos os registros a partir do próximo ao atual, 
		 * carregados da base para a memória.
		 * 
		 * @throws DatabaseException
		 * Lançada caso haja um problema ao acessar a base.
		 */
		public java.util.List<T> load() throws DatabaseException;
		
	}
}
