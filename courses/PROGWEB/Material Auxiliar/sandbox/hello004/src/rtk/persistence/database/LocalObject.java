package rtk.persistence.database;

/**
 * Um objeto disposto a usar {@link DatabaseObject} deve implementar esta 
 * interface para receber ou oferecer dados de colunas.
 * 
 * @author Ricardo Takashi Kagawa
 */
public interface LocalObject {

	/**
	 * Define o valor de um campo mapeado pela constante enumerável.
	 * 
	 * @param key
	 * A constante que identifica o campo.
	 * 
	 * @param value
	 * O valor a ser atribuído ao campo.
	 */
	public void set(Enum key, Object value);
	
	/**
	 * Obtém o valor de um campo mapeado pela constante enumerável.
	 * 
	 * @param key
	 * A constante que identifica o campo.
	 * 
	 * @return
	 * O valor atribuído ao campo.
	 */
	public Object get(Enum key);
	
}
