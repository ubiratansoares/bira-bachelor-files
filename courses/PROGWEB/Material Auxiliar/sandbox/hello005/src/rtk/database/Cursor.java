package rtk.database;

/**
 * A interface define uma forma de iterar através de uma lista de
 * resultados de uma busca na base de dados. A implementação deve
 * oferecer os dados recuperados na mesma instância do cursor.
 */
public interface Cursor<T> extends Iterable<T>, Cloneable {
	
	/**
	 * Lista todas as instâncias da entidade representada pelo cursor.
	 * O cursor deve ser inicializado em uma posição anterior ao 
	 * primeiro resultado.
	 * 
	 * @param conn
	 * A conexão com a base de dados do qual recuperar as instâncias.
	 * 
	 * @return 
	 * O próprio cursor.
	 * 
	 * @throws DatabaseException
	 * Lançada caso haja um problema ao acessar a base de dados, ou
	 * ao recuperar os dados da base.
	 */
	public T list(java.sql.Connection conn) throws DatabaseException;
	
	/**
	 * Avança o cursor para a próxima posição, recuperando os dados
	 * da nova posição. Antes de chamar este método pela primeira vez,
	 * o cursor deverá estar posicionado antes da primeira linha.
	 * 
	 * @return
	 * {@code true} caso o método tenha recuperado um registro válido;
	 * {@code false} caso não haja mais registros a recuperar.
	 * 
	 * @throws DatabaseException
	 * Lançada caso haja um problema ao acessar a base de dados.
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
	 * Lançada caso haja um problema ao acessar a base de dados.
	 */
	public boolean seek(int n) throws DatabaseException;
	
	/**
	 * Fecha o cursor, liberando recursos. Não é necessário liberar
	 * todos os recursos associados à conexão, basta liberar todos 
	 * os recursos associados exclusivamente ao cursor.
	 * 
	 * @throws DatabaseException
	 * Lançada caso haja um problema ao acessar a base de dados.
	 */
	public void close() throws DatabaseException;

	/**
	 * Carrega todos os resultados remanescentes para a memória, 
	 * formando uma lista. O método não precisa ser eficiente, e 
	 * não precisa estipular limites de tempo ou de memória. Caso 
	 * a implementação decida limitar o tempo ou a memória, uma
	 * exceção poderá ser lançada. 
	 * 
	 * <p>Este método pode ser conveniente para consultas que gerem 
	 * poucos resultados, mas será arriscado usá-lo em consultas que 
	 * gerem muitos resultados (note que a implementação não é obrigada 
	 * a limitar o uso de tempo ou memória!).
	 * 
	 * @return
	 * Uma lista com todos os registros a partir do próximo ao atual, 
	 * carregados da base para a memória.
	 * 
	 * @throws DatabaseException
	 * Lançada caso haja um problema ao acessar a base de dados.
	 */
	public java.util.List<T> load() throws DatabaseException;
	
	/**
	 * Encapsula o cursor em uma visão de {@link java.util.Iterator},
	 * permitindo que a iteração crie novas instâncias da entidade 
	 * iterada, além de permitir que o cursor seja usado em sentenças
	 * {@code "foreach"}. Como o cursor será encapsulado, iterar usando
	 * o iterador retornado também fará este cursor avançar.
	 * 
	 * @return
	 * Um iterador que lista todos os elementos a partir do próximo ao
	 * atual. Os elementos retornados serão novas instâncias do tipo
	 * iterado (não será a própria instância do cursor).
	 */
	@Override
	public java.util.Iterator<T> iterator();
	
	public T clone();
	
}
