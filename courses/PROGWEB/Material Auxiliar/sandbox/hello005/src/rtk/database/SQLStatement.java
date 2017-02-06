package rtk.database;

import java.sql.*;
import java.util.*;

public abstract class SQLStatement implements Cloneable {
	
	/* == métodos de mapeamento == */
	
	/**
	 * Declara um parâmetro com nome e tipo.
	 * 
	 * @param name
	 * Uma string que identifica o parâmetro no programa.
	 * 
	 * @param type
	 * O tipo do parâmetro no programa.
	 */
	public void setParameter(String name, Class<?> type) {
		if(type == null || name == null) 
			throw new NullPointerException();
		
		args.put(name, new Argument(type));
		return;
	}
	
	/**
	 * Define o parâmetro dado como parâmetro de saída. O parâmetro
	 * será mapeado para uma coluna, e seu valor poderá ser obtido
	 * pelo identificador do programa.
	 * 
	 * @param parname
	 * O nome do parâmetro cujo valor será obtido do comando.
	 * Pode ser o nome de um parâmetro de entrada.
	 * 
	 * @param colname
	 * O nome da coluna que retornará o valor gerado.
	 */
	public void mapOutParameter(String parname, String colname) {
		if(parname == null || colname == null) 
			throw new NullPointerException();
		if(parname.isEmpty() || colname.isEmpty()) 
			throw new IllegalArgumentException();
		
		out.put(parname, colname);
		return;
	}
	
	/**
	 * Define o parâmetro dado como parâmetro de entrada. O parâmetro
	 * será mapeado para a próxima posição do molde.
	 * 
	 * @param name
	 * O nome do parâmetro cujo valor será passado para o comando.
	 * Pode ser o nome de um parâmetro de saída.
	 */
	public void mapInParameter(String name) {
		if(name == null) throw new NullPointerException();
		if(name.isEmpty()) throw new IllegalArgumentException();
		
		pos.add(name);
		return;
	}
	
	/**
	 * Define o molde do comando a ser executado. O molde deve estar no
	 * formato aceito pelo {@link java.sql.PreparedStatement}.
	 */
	public void setTemplate(String template) {
		if(template == null) throw new NullPointerException();
		
		this.sql = template;
		return;
	}
	
	/* == métodos para valores == */
	
	/**
	 * Limpa os valores dos argumentos.
	 * 
	 * @see #set(java.lang.String, java.lang.Object)
	 */
	public void clear() {
		for(Argument a : args.values())
			a.value = null;
		return;
	}
	
	/**
	 * Associa um valor ao argumento.
	 * 
	 * @param key
	 * O nome do parâmetro.
	 * 
	 * @param value
	 * O valor que será associado ao parâmetro.
	 */
	public void setValue(String name, Object value) {
		Argument a = args.get(name);
		if(value != null) {
			if(!a.type.isAssignableFrom(value.getClass()))
				throw new ClassCastException();
		}
		a.value = value;
		return;
	}
	
	/**
	 * Associa um valor ao argumento.
	 * 
	 * @param key
	 * O nome do parâmetro.
	 * 
	 * @param type
	 * O tipo (classe) associado ao parâmetro. Pode ser
	 * uma superclasse do tipo realmente associado.
	 */
	public <T> T getValue(String name, Class<T> type) {
		Argument a = args.get(name);
		if(!type.isAssignableFrom(a.type))
			throw new ClassCastException();
		return type.cast(a.value);
	}
	
	/* == execução do comando == */
	
	/**
	 * Executa o comando SQL. O comando criado por este método não é
	 * fechado após a execução para permitir que resultados sejam 
	 * obtidos posteriormente. Quando terminar de recuperar todos os
	 * resultados, é recomendado fechar este comando com o método
	 * {@link #close}.
	 * 
	 * @param conn
	 * A conexão com a base de dados por onde executar o comando.
	 */
	public void execute(Connection conn) throws DatabaseException {
		try {
			this.close(); //fecha o anterior
			
			statement = this.createStatement(conn, this.sql, 
					out.values().toArray(new String[0]));
			statement.setQueryTimeout(timeout);
			
			//substituir todos os argumentos
			for(int i = 0; i < pos.size(); i++) {
				Argument a = args.get(pos.get(i));
				if(Integer.class.isAssignableFrom(a.type)) {
					if(a.value == null)
						statement.setNull(i + 1, Types.INTEGER);
					else
						statement.setInt(i + 1, (Integer) a.value);
				} else if(String.class.isAssignableFrom(a.type)) {
					if(a.value == null)
						statement.setNull(i + 1, Types.VARCHAR);
					else
						statement.setString(i + 1, (String) a.value);
				} else if(Boolean.class.isAssignableFrom(a.type)) {
					if(a.value == null)
						statement.setNull(i + 1, Types.BOOLEAN);
					else
						statement.setBoolean(i + 1, (Boolean) a.value);
				}
				// TODO adicionar mais tipos
			}
			
			statement.execute();
			result = this.getResults(statement);
		} catch(SQLException e) {
			throw new DatabaseException(e);
		}
	}
	
	/**
	 * Cria um {@link java.sql.PreparedStatement} adequado para a
	 * operação desejada. Um comando de INSERT ou UPDATE poderá usar
	 * a lista de nomes de colunas para obter os valores gerados. 
	 * Comandos de SELECT, DELETE e DDL podem ignorar a lista.
	 * 
	 * @param conn
	 * A conexão com a base de dados envolvida na operação.
	 * 
	 * @param sql
	 * O comando SQL que será criado.
	 * 
	 * @param keys
	 * Os nomes das colunas cujos valores serão gerados pelo comando.
	 */
	protected abstract PreparedStatement createStatement(
			Connection conn, String sql, String[] keys) throws SQLException;
	
	/**
	 * Obtém o cursor de resultados adequado para a operação desejada.
	 * Comandos de INSERT ou UPDATE podem retornar valores DEFAULT 
	 * gerados para colunas. Comandos de DELETE e DDL não geram 
	 * resultados e podem retornar {@code null}. Comandos de SELECT
	 * podem retornar o cursor resultante da consulta.
	 * 
	 * @param st
	 * O {@link java.sql.PreparedStatement} que gerou o resultado. Ele
	 * já foi executado e está pronto para retornar um cursor (caso 
	 * haja algum para retornar).
	 */
	protected abstract ResultSet getResults(PreparedStatement st)
			throws SQLException;
	
	/**
	 * Fecha o comando associado, liberando recursos. Chamar este 
	 * método sobre um comando já fechado não faz nada. Fechar o
	 * comando não garante que a conexão associada tembém feche, mas
	 * fechar a conexão certamente fechará este comando.
	 */
	public void close() throws DatabaseException {
		try {
			if(statement != null) {
				statement.close();
				statement = null;
				result = null;
			}
		} catch(SQLException e) {
			throw new DatabaseException(e);
		}
	}
	
	/**
	 * Verifica se o comando já está fechado.
	 */
	public boolean isClosed() throws DatabaseException {
		try {
			return (statement == null || statement.isClosed());
		} catch(SQLException e) {
			throw new DatabaseException(e);
		}
	}
	
	/**
	 * Obtém a contagem de registros alterados após a execução de 
	 * um comando DML.
	 * 
	 * @return
	 * A contagem de registros alterados ou -1 se o comando não 
	 * era DML.
	 */
	public int getUpdateCount() throws DatabaseException {
		try {
			if(statement == null)
				throw new DatabaseException("Statement is already closed.");
			return statement.getUpdateCount();
		} catch(SQLException e) {
			throw new DatabaseException(e);
		}
	}
	
	/**
	 * Avança o cursor de resultados para o próximo registro.
	 * 
	 * @return
	 * {@code true} caso o método {@link #getValue} vá retornar 
	 * valores válidos.
	 */
	public boolean next() throws DatabaseException {
		try {
			if(statement == null)
				throw new DatabaseException("Statement is already closed.");
			if(result == null) return false;
			
			if(result.next()) {
				this.retrieveData();
				return true;
			} else {
				result.close();
				result = null;
				return false;
			}
		} catch(SQLException e) {
			throw new DatabaseException(e);
		}
	}
	
	public boolean seek(int n) throws DatabaseException {
		try {
			if(result != null && !result.isClosed() && n >= 0) {
				if(result.relative(n)) {
					this.retrieveData();
					return true;
				} else {
					result.close();
					result = null;
					return false;
				}
			} else
				return false;
		} catch(SQLException e) {
			throw new DatabaseException(e);
		}
	}
	
	private void retrieveData() throws SQLException {
		for(String key : out.keySet()) {
			Argument a = args.get(key);
			if(a.type.isAssignableFrom(Integer.class)) {
				Integer value = result.getInt(out.get(key));
				if(result.wasNull())
					value = null;
				this.setValue(key, value);
			} else if(a.type.isAssignableFrom(Boolean.class)) {
				Boolean value = result.getBoolean(out.get(key));
				if(result.wasNull())
					value = null;
				this.setValue(key, value);
			} else if(a.type.isAssignableFrom(String.class)) {
				this.setValue(key, result.getString(out.get(key)));
			}
			// TODO adicionar mais tipos
		}
		return;
	}
	
	/* == outros métodos == */
	
	/**
	 * Define o limite de tempo para que um comando execute. Se o
	 * tempo limite for atingido, o método {@link #execute} lançará
	 * uma exceção.
	 * 
	 * @param timeout
	 * O tempo limite para executar um comando, em segundos.
	 */
	public void setTimeout(int timeout) {
		this.timeout = timeout;
	}
	
	/**
	 * Cria uma cópia do comando. Apenas o modelo é copiado, os valores
	 * e o cursor são inicializados vazios.
	 */
	public SQLStatement clone() {
		SQLStatement copy = this.createClone();
		copy.sql = this.sql;
		copy.timeout = this.timeout;
		
		for(String key : this.args.keySet()) {
			Argument a = this.args.get(key);
			copy.args.put(key, new Argument(a.type));
		}
		
		for(String key : this.out.keySet())
			copy.out.put(key, this.out.get(key));
		for(String id : this.pos)
			copy.pos.add(id);
			
		copy.statement = null;
		copy.result = null;
		return copy;
	}
	
	/**
	 * Cria uma cópia do comando. Subclasses devem implementar este
	 * método para criar uma nova instância da classe concreta. Além
	 * disso, subclasses terão a chance de copiar seus próprios 
	 * atributos para a cópia antes de esta classe copiar seus 
	 * atributos.
	 */
	protected abstract SQLStatement createClone();
	
	/**
	 * Exibe o comando na forma de um {@link java.sql.PreparedStatement},
	 * seguido dos parâmetros que serão usados nos respectivos marcadores.
	 */
	@Override
	public String toString() {
		return String.format("\"%s\":%s->%s", sql, pos, out.values());
	}
	
	/* == atributos/construtores == */
	
	private String sql;
	private Map<String, Argument> args;
	private Map<String, String> out;
	private List<String> pos;
	
	private int timeout;
	private PreparedStatement statement;
	private ResultSet result;
	
	/**
	 * Cria um comando vazio.
	 */
	protected SQLStatement() {
		this.sql = "";
		this.args = new HashMap<String, Argument>();
		this.pos = new ArrayList<String>();
		this.out = new HashMap<String, String>();
		
		this.timeout = 0;
		this.statement = null;
		this.result = null;
	}
	
	/* == classes aninhadas == */
	
	private class Argument {
		public Class<?> type;
		public Object value;
		
		public Argument(Class<?> type) {
			this.type = type;
			this.value = null;
		}
		
		@Override
		public String toString() {
			return String.valueOf(this.value);
		}
		
	}
	
}
