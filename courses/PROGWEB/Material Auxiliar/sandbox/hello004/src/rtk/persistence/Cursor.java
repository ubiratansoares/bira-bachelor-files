package rtk.persistence;

/**
 * Uma classe que implemente esta interface deverá ser capaz de
 * iterar sobre uma lista de resultados de uma busca feita em uma
 * mídia de persistência.
 * 
 * @author Ricardo Takashi Kagawa
 */
public interface Cursor<T> extends Iterable<T> {
	
	/**
	 * Avança o cursor para a próxima posição, recuperando os dados
	 * da nova posição. Antes de chamar este método pela primeira vez,
	 * o cursor deverá estar posicionado antes da primeira linha.
	 * 
	 * @return
	 * {@code true} caso o método tenha recuperado um registro válido;
	 * {@code false} caso não haja mais registros a recuperar.
	 * 
	 * @throws PersistenceException
	 * Lançada caso haja um problema ao acessar a mídia de persistência.
	 */
	public boolean next() throws PersistenceException;

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
	 * @throws PersistenceException
	 * Lançada caso haja um problema ao acessar a mídia de persistência.
	 */
	public boolean fastForward(int n) throws PersistenceException;
	
	/**
	 * Fecha o cursor, liberando recursos. Não é necessário liberar
	 * todos os recursos associados à midia, mas deve liberar todos os 
	 * recursos associados exclusivamente ao cursor.
	 * 
	 * @throws PersistenceException
	 * Lançada caso haja um problema ao acessar a mídia de persistência.
	 */
	public void close() throws PersistenceException;

	/**
	 * Carrega todos os resultados remanescentes para a memória, formando
	 * uma lista. O método não precisa ser eficiente, e não deve estipular
	 * limites de tempo ou de memória.
	 * 
	 * @return
	 * Uma lista com todos os registros a partir do próximo ao atual, 
	 * carregados da mídia para a memória.
	 * 
	 * @throws PersistenceException
	 * Lançada caso haja um problema ao acessar a mídia de persistência.
	 */
	public java.util.List<T> load() throws PersistenceException;
	
}
