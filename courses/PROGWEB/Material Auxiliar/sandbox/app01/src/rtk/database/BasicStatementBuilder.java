package rtk.database;

import java.sql.*;
import java.util.*;

public class BasicStatementBuilder {
	
	/* == mapeamento do modelo == */
	
	/**
	 * Mapeia uma coluna com tipo (do programa) ao identificador.
	 * 
	 * @param id
	 * O identificador que será usado para associar valores à coluna.
	 * 
	 * @param name
	 * O nome da coluna na tabela da base de dados.
	 * 
	 * @param type
	 * O tipo do parâmetro no programa.
	 */
	public void setColumn(String id, String name, Class<?> type) {
		columns.put(id, new Column(name, type));
		return;
	}
	
	/**
	 * Define as colunas que serão geradas na inserção ou atualização. 
	 * Por padrão, nenhuma coluna é marcada como gerada.
	 * 
	 * @param keys
	 * Uma lista de identificadores usados no mapeamento.
	 * 
	 * @see #setColumn
	 */
	public void setGeneratedKeys(String... keys) {
		for(Column c : columns.values())
			c.generated = false;
		for(String key : keys)
			columns.get(key).generated = true;
		return;
	}
	
	/**
	 * Define se uma coluna será gerada na inserção ou atualização. 
	 * Por padrão, nenhuma coluna é marcada como gerada.
	 * 
	 * @param key
	 * Uma identificador usado no mapeamento.
	 * 
	 * @param value
	 * {@code true} faz o comando esperar um valor default para 
	 * esta coluna.
	 * 
	 * @see #setColumn
	 */
	public void setGeneratedKey(String key, boolean value) {
		columns.get(key).generated = value;
		return;
	}
	
	/**
	 * Define as colunas que serão usadas como chave primária
	 * para os comandos SELECT (de único registro), DELETE e 
	 * UPDATE gerados automaticamente. Essas colunas serão usadas
	 * nas cláusulas WHERE destas operações e não serão parte da 
	 * cláusula SET do comando UPDATE.
	 * 
	 * @param keys
	 * Uma lista de identificadores usados no mapeamento.
	 * 
	 * @see #setColumn
	 */
	public void setPrimaryKeys(String... keys) {
		for(Column c : columns.values())
			c.primaryKey = false;
		for(String key : keys)
			columns.get(key).primaryKey = true;
		return;
	}
	
	/**
	 * Marca uma coluna como chave primária. 
	 * Por padrão, nenhuma coluna é marcada como chave primária.
	 * 
	 * @param key
	 * Uma identificador usado no mapeamento.
	 * 
	 * @param value
	 * {@code true} evita que esta coluna seja atualizável, e força
	 * esta coluna a sempre ser usada nas cláusulas WHERE.
	 * 
	 * @see #setColumn
	 */
	public void setPrimaryKey(String key, boolean value) {
		columns.get(key).primaryKey = value;
		return;
	}
	
	/* == construção dos comandos == */
	
	/**
	 * Constrói um comando de INSERT simples.
	 */
	public SQLStatement buildInsert() {
		SQLStatement statement = new DMLStatement();
		
		StringBuilder cols = new StringBuilder();
		StringBuilder values = new StringBuilder();
		
		for(String key : columns.keySet()) {
			Column c = columns.get(key);
			statement.setParameter(key, c.type);
			if(c.generated) {
				statement.mapOutParameter(key, c.name);
			} else {
				cols.append(", " + c.name);
				values.append(", ?"); 
				statement.mapInParameter(key);
			}
		}
		
		if(cols.length() > 0) {
			cols.delete(0, 2);
			values.delete(0, 2);
		}
		
		if(values.length() == 0) return statement;
		
		statement.setTemplate(String.format("INSERT INTO %s (%s) VALUES (%s);", 
				table, cols.toString(), values.toString()));
		
		return statement;
	}
	
	/**
	 * Constrói um comando de DELETE simples.
	 */
	public SQLStatement buildDelete() {
		SQLStatement statement = new DMLStatement();
		
		StringBuilder where = new StringBuilder();
		
		for(String key : columns.keySet()) {
			Column c = columns.get(key);
			statement.setParameter(key, c.type);
			if(c.primaryKey) {
				where.append(String.format(" AND %s = ?", c.name));
				statement.mapInParameter(key);
			}
		}
		
		if(where.length() > 0) where.delete(0, 5);
		if(where.length() == 0) return statement;
		
		statement.setTemplate(String.format(
				"DELETE FROM %s WHERE %s;", table, where.toString()));
		
		return statement;
	}
	
	/**
	 * Constrói um comando de UPDATE simples.
	 */
	public SQLStatement buildUpdate() {
		SQLStatement statement = new DMLStatement();
		
		//separar colunas comuns de chaves primárias
		Set<String> setCols = new HashSet<String>();
		Set<String> whereCols = new HashSet<String>();
		for(String key : columns.keySet()) {
			Column c = columns.get(key);
			statement.setParameter(key, c.type);
			if(c.primaryKey)
				whereCols.add(c.name);
			else
				setCols.add(c.name);
		}
		
		//construir cláusula SET
		StringBuilder set = new StringBuilder();
		for(String key : setCols) {
			Column c = columns.get(key);
			set.append(String.format(", %s = ?", c.name));
			statement.mapInParameter(key);
			if(c.generated)
				statement.mapOutParameter(key, c.name);
		}
		
		//construir cláusula WHERE
		StringBuilder where = new StringBuilder();
		for(String key : whereCols) {
			Column c = columns.get(key);
			where.append(String.format(" AND %s = ?", c.name));
			statement.mapInParameter(key);
		}
		
		if(set.length() > 0 && where.length() > 0) {
			set.delete(0, 2);
			where.delete(0, 5);
		} else
			return statement;
		
		//construir comando UPDATE
		statement.setTemplate(String.format("UPDATE %s SET %s WHERE %s;", 
				table, set.toString(), where.toString()));
		
		return statement;
	}
	
	/**
	 * Constrói um comando de SELECT (por chave primária) simples.
	 */
	public SQLStatement buildSelect() {
		SQLStatement statement = new SelectStatement();
		
		//construir cláusula WHERE
		StringBuilder where = new StringBuilder();
		for(String key : columns.keySet()) {
			Column c = columns.get(key);
			statement.setParameter(key, c.type);
			if(c.primaryKey) {
				where.append(String.format(" AND %s = ?", c.name));
				statement.mapInParameter(key);
			}
			statement.mapOutParameter(key, c.name);
		}
		
		if(where.length() > 0)
			where.delete(0, 5);
		else
			return statement;
		
		//construir comando SELECT
		statement.setTemplate(String.format(
				"SELECT * FROM %s WHERE %s;", table, where.toString()));
		
		return statement;
	}
	
	/**
	 * Constrói um comando de SELECT (tudo) simples.
	 */
	public SQLStatement buildSelectAll() {
		SQLStatement statement = new SelectStatement();
		
		for(String key : columns.keySet()) {
			Column c = columns.get(key);
			statement.setParameter(key, c.type);
			statement.mapOutParameter(key, c.name);
		}
		
		statement.setTemplate(String.format("SELECT * FROM %s;", table));
		
		return statement;
	}
	
	/* == outros métodos == */
	
	/**
	 * Exibe o comando na forma de um {@link java.sql.PreparedStatement},
	 * seguido dos parâmetros que serão usados nos respectivos marcadores.
	 */
	@Override
	public String toString() {
		return String.format("%s:%s", this.table, this.columns);
	}
	
	/* == atributos/construtores == */
	
	private String table;
	private Map<String, Column> columns;
	
	/**
	 * Cria um comando vazio associado a uma tabela.
	 */
	public BasicStatementBuilder(String tableName) {
		this.table = tableName;
		this.columns = new HashMap<String, Column>();
	}
	
	/* == classes aninhadas == */
	
	private class Column {
		public String name;
		public Class<?> type;
		public boolean generated;
		public boolean primaryKey;
		
		public Column(String name, Class<?> type) {
			this.name = name;
			this.type = type;
			this.generated = false;
			this.primaryKey = false;
		}
		
		@Override
		public String toString() {
			return String.format("%s:{type=%s,gen=%s,pk=%s}",
					name, type, generated, primaryKey);
		}
	}
	
}
