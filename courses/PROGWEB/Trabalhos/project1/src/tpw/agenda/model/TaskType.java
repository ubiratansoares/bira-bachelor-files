package tpw.agenda.model;

import tpw.agenda.database.*;

import java.sql.*;
import java.util.*;

/**
 * Representa os tipos de tarefas.
 */
public class TaskType implements DatabaseObject<TaskType> {
	
	private Integer id = null;
	private String name = null;
	private String description = null;
	
	private Connection conn = null;
	
	//construtores
	
	public TaskType() {
	}
	
	public TaskType(Connection conn) {
		this.conn = conn;
	}
	
	//setters e getters
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	public void setId(String id) {
		try {
			this.id = Integer.valueOf(id);
		} catch(NumberFormatException e) {
			this.id = null;
		}
	}
	
	public void setName(String name) {
		this.name = null;
		if(name != null && !name.trim().isEmpty())
			this.name = name.trim();
	}
	
	public void setDescription(String desc) {
		this.description = null;
		if(desc != null && !desc.trim().isEmpty())
			this.description = desc.trim();
	}
	
	public Integer getId() {
		return this.id;
	}
	
	public String getName() {
		return this.name;
	}
	
	public String getDescription() {
		return this.description;
	}
	
	//other methods
	
	@Override
	public boolean equals(Object o) {
		if(o == null) {
			return false;
		} else if(o instanceof String) {
			String str = (String) o;
			return str.trim().equals(this.getName());
		} else if(o instanceof TaskType) {
			TaskType p = (TaskType) o;
			if(this.id != null && p.id != null)
				return (p.id == this.id);
			else
				return (this.name == null)? 
					(p.name == null): 
					this.name.equals(p.name);
		} else
			return false;
	}
	
	//persistence
	
	public void setConnection(Connection conn) {
		this.conn = conn;
	}
	
	public void reviveKeys(ResultSet cursor) throws DatabaseException {
		try {
			this.id = cursor.getInt("id");
			if(cursor.wasNull())
				this.id = null;
		} catch(SQLException e) {
			throw new DatabaseException();
		}
	}
	
	public void revive(ResultSet cursor) throws DatabaseException {
		try {
			this.reviveKeys(cursor);
			
			this.name = cursor.getString("name");
			if(this.name != null)
				this.name = this.name.trim();
			
			this.description = cursor.getString("description");
			if(this.description != null)
				this.description = this.description.trim();
		} catch(SQLException e) {
			throw new DatabaseException();
		}
	}
	
	public void add() throws DatabaseException {
		if(this.name == null)
			throw new DatabaseException("Name cannot be null nor empty.");
		
		String query = String.format(
			"INSERT INTO task_type (name, description) " + 
			"VALUES ('%s', '%s')", this.name, this.description);
		DatabaseUtilities.executeInsert(this.conn, query, this, "id");
	}
	
	public boolean remove() throws DatabaseException {
		if(this.id == null)
			throw new DatabaseException("Need an id for this operation.");
		
		String query = String.format(
			"DELETE FROM task_type WHERE id = %d", this.id);
		return DatabaseUtilities.executeUpdate(this.conn, query);
	}
	
	public boolean get() throws DatabaseException {
		if(this.id == null && this.name == null)
			throw new DatabaseException("Need an id or a name for this operation.");
		
		String filter = (this.name != null)?
			String.format("name = '%s'", this.name):
			String.format("id = %d", this.id);
		
		String query = String.format("SELECT * FROM task_type WHERE %s", filter);
		return DatabaseUtilities.getSingleRecord(this.conn, query, this);
	}
	
	public boolean set() throws DatabaseException {
		if(this.id == null)
			throw new DatabaseException("Need an id for this operation.");
		
		String query = String.format(
				"UPDATE task_type SET name = '%s', description = '%s' WHERE id = %d", 
				this.name, this.description, this.id);
		return DatabaseUtilities.executeUpdate(this.conn, query);
	}
	
	//data sets
	
	public Cursor list() throws DatabaseException {
		String query = String.format("SELECT * FROM task_type ORDER BY name");
		ResultSet cursor = DatabaseUtilities.executeQuery(this.conn, query);
		return new Cursor(cursor);
	}
	
	/**
	 * Lista todos os tipos associadas a uma tarefa. Requer o
	 * identificador da tarefa.
	 */
	public Cursor list(Task task) throws DatabaseException {
		if(task.getId() == null)
			throw new DatabaseException("A task ID is required for this operation.");
		
		String query = String.format("SELECT t.* FROM task_type t " +
			"JOIN task_task_type r ON (t.id = r.type_id) " + 
			"WHERE r.task_id = %d", task.getId());
		
		ResultSet cursor = DatabaseUtilities.executeQuery(this.conn, query);
		return new Cursor(cursor);
	}
	
	public class Cursor extends TaskType implements DatabaseObject.Cursor<TaskType> {
		
		private ResultSet cursor;
		
		public Cursor(ResultSet cursor) {
			if(cursor == null)
				throw new NullPointerException();
			
			this.cursor = cursor;
		}
		
		public boolean next() throws DatabaseException {
			try {
				if(cursor.next()) {
					this.revive(cursor);
					return true;
				} else {
					cursor.close();
					return false;
				}
			} catch(SQLException e) {
				throw new DatabaseException(e);
			}
		}
		
		public boolean fastForward(int n) throws DatabaseException {
			try {
				return (n >= 0)? cursor.relative(n): false;
			} catch(SQLException e) {
				throw new DatabaseException(e);
			}
		}
		
		public void close() throws DatabaseException {
			try {
				if(cursor.getStatement() != null)
					cursor.getStatement().close();
			} catch(SQLException e) {
				throw new DatabaseException(e);
			}
		}
	
		public java.util.Iterator<TaskType> iterator() {
			return new Iterator(this);
		}
		
		public List<TaskType> load() throws DatabaseException {
			List<TaskType> l = new LinkedList<TaskType>();
			for(TaskType a : this)
				l.add(a);
			return l;
		}
		
	}
	
	// Não está sendo usado!!!!
	
	public class Iterator implements java.util.Iterator<TaskType> {
		
		private Cursor cursor;
		private TaskType nextItem;
		
		public Iterator(Cursor c) {
			if(c == null)
				throw new NullPointerException();
			
			try {
				this.cursor = c;
				this.nextItem = c.next()? (TaskType) c.clone(): null;
			} catch(DatabaseException e) {
				this.nextItem = null;
				this.close();
			} catch(CloneNotSupportedException e) {
				this.nextItem = null;
				this.close();
			}
		}
		
		@Override
		public boolean hasNext() {
			return (nextItem != null);
		}
		
		@Override
		public TaskType next() {
			TaskType r = nextItem;
			try {
				this.nextItem = cursor.next()? (TaskType) cursor.clone(): null;
			} catch(DatabaseException e) {
				this.nextItem = null;
				this.close();
			} catch(CloneNotSupportedException e) {
				this.nextItem = null;
				this.close();
			}
			return r;
		}
		
		@Override
		public void remove() {
			throw new UnsupportedOperationException();
		}
		
		public void close() {
			try {
				cursor.close();
			} catch(DatabaseException e) {
			}
			return;
		}
	
	}
	
}
