package rtk.persistence.database;

import rtk.persistence.*;

import java.lang.reflect.*;
import java.sql.*;
import java.util.*;

public class DatabaseObject<T extends LocalObject> implements Persistable {
	
	private static final long serialVersionUID = 1L;
	
	private String table;
	private Map<Enum, Column> columns;
	private Set<Column> usedKeys;
	private Set<Column> usedColumns;
	
	private T base;
	private Class<T> baseClass;
	
	private Connection conn;
	
	public DatabaseObject(String table, T obj, Class<T> baseClass) {
		this.base = obj;
		this.baseClass = baseClass;
		this.table = table;
		this.columns = new HashMap<Enum, Column>();
		this.usedKeys = new HashSet<Column>();
		this.usedColumns = new HashSet<Column>();
	}
	
	public void setConnection(Connection conn) {
		this.conn = conn;
	}
	
	public Connection getConnection() {
		return this.conn;
	}
	
	public void bind(Enum key, String name, ColumnType type) {
		Column c = new Column();
		c.key = key;
		c.name = name;
		c.type = type;
		columns.put(key, c);
	}
	
	public void useKeys(Enum... keys) {
		usedKeys.clear();
		for(Enum key : keys)
			usedKeys.add(columns.get(key));
	}
	
	public void useColumns(Enum... keys) {
		usedColumns.clear();
		for(Enum key : keys)
			usedColumns.add(columns.get(key));
	}
	
	private static void set(LocalObject obj, Column c, ResultSet cursor) throws SQLException {
		switch(c.type) {
			case STRING:
				obj.set(c.key, cursor.getString(c.name));
				break;
				
			case INTEGER:
				int i = cursor.getInt(c.name);
				if(cursor.wasNull())
					obj.set(c.key, null);
				else
					obj.set(c.key, Integer.valueOf(i));
				break;
		}
	}
	
	public void add() throws PersistenceException {
		StringBuilder names = new StringBuilder();
		StringBuilder values = new StringBuilder();
		for(Column c : usedColumns) {
			names.append(c.name + ", ");
			
			String value = "";
			switch(c.type) {
				case INTEGER:
					value = base.get(c.key).toString();
					break;
					
				case STRING:
					value = String.format("'%s'", base.get(c.key).toString());
					break;
			}
			values.append(value + ", ");
		}
		
		if(names.length() > 0 && values.length() > 0) {
			names.delete(names.length() - ", ".length(), names.length());
			values.delete(values.length() - ", ".length(), values.length());
			String query = String.format("INSERT INTO %s (%s) VALUES (%s)", this.table, names, values);
			
			Statement st = null;
			try {
				st = conn.createStatement();
				st.executeUpdate(query);
			} catch(SQLException e) {
				throw new DatabaseException(e);
			} finally {
				try {
					if(st != null) st.close();
				} catch(SQLException e2) {
					throw new DatabaseException(e2);
				}
			}
		}
	}
	
	public boolean remove() throws DatabaseException {
		StringBuilder filter = new StringBuilder();
		for(Column c : usedKeys) {
			filter.append(c.name);
			
			if(base.get(c.key) == null) {
				filter.append(" IS NULL");
			} else {
				switch(c.type) {
					case INTEGER:
						filter.append(" = " + base.get(c.key).toString());
						break;
						
					case STRING:
						filter.append(String.format(" = '%s'", base.get(c.key).toString()));
						break;
				}
			}
			filter.append(" AND ");
		}
		
		if(filter.length() > 0) {
			filter.delete(filter.length() - " AND ".length(), filter.length());
			String query = String.format("DELETE FROM %s WHERE %s", this.table, filter);
			
			Statement st = null;
			try {
				st = conn.createStatement();
				int r = st.executeUpdate(query);
				return (r == 1);
			} catch(SQLException e) {
				throw new DatabaseException(e);
			} finally {
				try {
					if(st != null) st.close();
				} catch(SQLException e2) {
					throw new DatabaseException(e2);
				}
			}
		}
		return false;
	}
	
	public boolean set() throws DatabaseException {
		StringBuilder values = new StringBuilder();
		for(Column c : usedColumns) {
			values.append(c.name + " = ");
			
			switch(c.type) {
				case INTEGER:
					values.append(base.get(c.key).toString());
					break;
					
				case STRING:
					values.append(String.format("'%s'", base.get(c.key).toString()));
					break;
			}
			values.append(", ");
		}
		
		StringBuilder filter = new StringBuilder();
		for(Column c : usedKeys) {
			filter.append(c.name);
			
			if(base.get(c.key) == null) {
				filter.append(" IS NULL");
			} else {
				switch(c.type) {
					case INTEGER:
						filter.append(" = " + base.get(c.key).toString());
						break;
						
					case STRING:
						filter.append(String.format(" = '%s'", base.get(c.key).toString()));
						break;
				}
			}
			filter.append(" AND ");
		}
		
		if(values.length() > 0 && filter.length() > 0) {
			values.delete(values.length() - ", ".length(), values.length());
			filter.delete(filter.length() - " AND ".length(), filter.length());
			String query = String.format("UPDATE %s SET %s WHERE %s", this.table, values, filter);
			
			Statement st = null;
			try {
				st = conn.createStatement();
				int r = st.executeUpdate(query);
				return (r == 1);
			} catch(SQLException e) {
				throw new DatabaseException(e);
			} finally {
				try {
					if(st != null) st.close();
				} catch(SQLException e2) {
					throw new DatabaseException(e2);
				}
			}
		}
		return false;
	}
	
	public boolean get() throws DatabaseException {
		StringBuilder filter = new StringBuilder();
		for(Column c : usedKeys) {
			filter.append(c.name);
			
			if(base.get(c.key) == null) {
				filter.append(" IS NULL");
			} else {
				switch(c.type) {
					case INTEGER:
						filter.append(" = " + base.get(c.key).toString());
						break;
						
					case STRING:
						filter.append(String.format(" = '%s'", base.get(c.key).toString()));
						break;
				}
			}
			filter.append(" AND ");
		}
		
		if(filter.length() > 0) {
			filter.delete(filter.length() - " AND ".length(), filter.length());
			String query = String.format("SELECT * FROM %s WHERE %s", this.table, filter);
			
			Statement st = null;
			try {
				st = conn.createStatement();
				ResultSet cursor = st.executeQuery(query);
				boolean found = cursor.next();
				if(found) {
					for(Column c : columns.values())
						this.set(base, c, cursor);
				}
				return found;
			} catch(SQLException e) {
				throw new DatabaseException(e);
			} finally {
				try {
					if(st != null) st.close();
				} catch(SQLException e2) {
					throw new DatabaseException(e2);
				}
			}
		}
		return false;
	}
	
	public Cursor list() throws DatabaseException {
		StringBuilder names = new StringBuilder();
		for(Column c : usedColumns)
			names.append(c.name + ", ");
		
		if(names.length() > 0) {
			names.delete(names.length() - ", ".length(), names.length());
			String query = String.format("SELECT %s FROM %s", names, this.table);
			
			Statement st = null;
			try {
				st = conn.createStatement();
				ResultSet cursor = st.executeQuery(query);
				Column[] cols = usedColumns.toArray(new Column[0]);
				return new Cursor(baseClass, cols, cursor);
			} catch(SQLException e) {
				throw new DatabaseException(e);
			} catch(InstantiationException e) {
				throw new DatabaseException(e);
			} catch(IllegalAccessException e) {
				throw new DatabaseException(e);
			}
		}
		return null;
	}
	
	//static properties
	
	private static class Column {
		public Enum key = null;
		public String name = "";
		public ColumnType type = null;
		
		public boolean foreignKey = false;
		public Object oldValue = null;
	}
	
	public static enum ColumnType {
		INTEGER, STRING
	}
	
	public class Cursor implements rtk.persistence.Cursor<T> {
		
		private static final long serialVersionUID = 1L;
		
		private ResultSet cursor;
		private Column[] columns;
		private T base;
		private Class<T> baseClass;
		
		public Cursor(Class<T> baseClass, Column[] columns, ResultSet rs) 
				throws InstantiationException, IllegalAccessException {
			this.cursor = rs;
			this.baseClass = baseClass;
			this.base = baseClass.newInstance();
			this.columns = columns;
		}
		
		public boolean next() throws DatabaseException {
			try {
				if(cursor.next()) {
					for(Column c : columns)
						DatabaseObject.set((LocalObject) base, c, cursor);
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
				if(cursor != null && !cursor.isClosed() && cursor.getStatement() != null)
					cursor.getStatement().close();
			} catch(SQLException e) {
				throw new DatabaseException(e);
			}
			return;
		}
	
		@Override
		public java.util.Iterator<T> iterator() {
			return new Iterator(this);
		}
		
		public List<T> load() throws DatabaseException {
			List<T> l = new LinkedList<T>();
			for(T item : this)
				l.add(item);
			return l;
		}
		
		private T createInstance() throws SQLException, InstantiationException, IllegalAccessException {
			T clone = baseClass.newInstance();
			for(Column c : columns)
				DatabaseObject.set(clone, c, cursor);
			return clone;
		}
		
	}
	
	public class Iterator implements java.util.Iterator<T> {
		
		private Cursor cursor;
		private T nextItem;
		
		public Iterator(Cursor c) {
			this.cursor = c;
			try {
				if(cursor.next())
					nextItem = cursor.createInstance();
				else
					nextItem = null;
			} catch(Exception e) {
				e.printStackTrace();
			}
		}
		
		@Override
		public boolean hasNext() {
			return (nextItem != null);
		}
		
		@Override
		public T next() {
			T r = nextItem;
			try {
				if(cursor.next())
					nextItem = cursor.createInstance();
				else
					this.nextItem = null;
			} catch(Exception e) {
			}
			return r;
		}
		
		@Override
		public void remove() {
			throw new UnsupportedOperationException();
		}
		
		public void close() throws Exception {
			if(cursor != null)
				cursor.close();
			return;
		}
	
	}
	
}
