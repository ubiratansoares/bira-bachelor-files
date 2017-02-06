package rtk.persistence;

import java.io.Serializable;

/**
 * Representa um objeto persistível (em banco de dados, arquivo binário,
 * arquivo xml, etc).
 * 
 * @author Ricardo Takashi Kagawa
 */
public interface Persistable extends Serializable {
	
	//create, retrieve, update, delete
	
	/**
	 * Persiste o objeto na mídia de persistência associada.
	 * Um novo registro deve ser criado, ou a operação deve falhar.
	 * Caso já exista um registro com a mesma chave, ela não deve
	 * ser sobrescrita (a operação também deve falhar nesse caso).
	 * 
	 * @throws PersistenceException
	 * Lançada caso haja um problema ao acessar a mídia de persistência
	 * ou caso haja uma violação de qualquer restrição aos dados (como
	 * de chave primária, por exemplo).
	 */
	public void add() throws PersistenceException;
	
	/**
	 * Remove o objeto da mídia de persistência associada.
	 * A remoção deve ocorrer pela chave primária ou por um atributo 
	 * com restrição de unicidade. Caso o registro não seja encontrado,
	 * o método não deve lançar uma exceção.
	 * 
	 * @return
	 * {@code true} caso um registro tenha sido encontrado e removido;
	 * {@code false} caso o registro não tenha sido encontrado.
	 * 
	 * @throws PersistenceException
	 * Lançada caso haja um problema ao acessar a mídia de persistência
	 * ou caso haja uma violação de qualquer restrição aos dados (como
	 * de chave estrangeira, por exemplo).
	 */
	public boolean remove() throws PersistenceException;

	/**
	 * Obtém os dados de um objeto persistido na mídia de persistência
	 * associada. O objeto será recuperado pela chave primária, ou por
	 * um de seus atributos com restrição de unicidade.
	 * 
	 * @return
	 * {@code true} caso algum registro corresponda à chave buscada.
	 * 
	 * @throws PersistenceException
	 * Lançada caso haja um problema ao acessar a mídia de persistência.
	 */
	public boolean get() throws PersistenceException;
	
	/**
	 * Persiste as alterações feitas a um objeto persistido na mídia de
	 * persistência associada. O registro será associado ao objeto pela
	 * chave primária. Caso não exista um registro correspondente, a
	 * operação deverá falhar.
	 * 
	 * @return
	 * {@code true} caso um registro tenha sido encontrado e atualizado;
	 * {@code false} caso o registro não tenha sido encontrado.
	 * 
	 * @throws PersistenceException
	 * Lançada caso haja um problema ao acessar a mídia de persistência
	 * ou caso haja uma violação de qualquer restrição aos dados (como
	 * de chave estrangeira, por exemplo).
	 */
	public boolean set() throws PersistenceException;
	
	//retrieve lists
	
	/**
	 * Lista (através de um cursor) todos os registros encontrados na
	 * mídia de persistência associada que correspondam a esta classe.
	 * 
	 * @throws PersistenceException
	 * Lançada caso haja um problema ao acessar a mídia de persistência.
	 */
	public Cursor list() throws PersistenceException;
	
}
