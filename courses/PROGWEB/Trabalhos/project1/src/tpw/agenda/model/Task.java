package tpw.agenda.model;

import tpw.agenda.database.*;

import java.sql.*;
import java.text.*;
import java.util.*;

/**
 * Representa as tarefas.
 */
public class Task implements DatabaseObject<Task> {
	
	public static final DateFormat DATE_FORMAT = new SimpleDateFormat("dd/MM/yyyy HH:mm");
	
	private Integer id = null;
	private Account owner = null;
	private String shortDescription = null;
	private String longDescription = null;
	private Boolean busy = null;
	private String location = null;
	private java.util.Date createdOn = null;
	private java.util.Date startTime = null;
	private java.util.Date endTime = null;
	private Map<String, TaskType> taskTypes = new HashMap<String, TaskType>();
	
	private boolean reloadOwner = false;
	private boolean updateOwner = true;
	private boolean reloadTaskTypes = false;
	private boolean updateTaskTypes = true;
	
	private Connection conn = null;
	
	//construtores
	
	public Task() {
	}
	
	public Task(Connection conn) {
		this.conn = conn;
	}
	
	//setters e getters
	
	public void setId(Integer id) {
		this.id = id;
		this.updateOwner = true;
		this.reloadOwner = true;
		this.updateTaskTypes = true;
		this.reloadTaskTypes = true;
	}
	
	public void setId(String id) {
		try {
			this.id = Integer.valueOf(id);
		} catch(NumberFormatException e) {
			this.id = null;
		}
		this.updateOwner = true;
		this.reloadOwner = true;
		this.updateTaskTypes = true;
		this.reloadTaskTypes = true;
	}
	
	public void setOwner(Account owner) {
		this.owner = null;
		if(owner != null && owner.getId() != null)
			this.owner = owner;
		this.updateOwner = true;
		this.reloadOwner = true;
	}
	
	public void setOwner(String email) throws DatabaseException {
		this.owner = null;
		if(email != null && !email.trim().isEmpty()) {
			this.owner = new Account();
			this.owner.setEmail(email);
			this.owner.get();
		}
		this.updateOwner = true;
		this.reloadOwner = false;
	}
	
	public void setShortDescription(String desc) {
		this.shortDescription = null;
		if(desc != null && !desc.trim().isEmpty())
			this.shortDescription = desc.trim();
	}
	
	public void setLongDescription(String desc) {
		this.longDescription = null;
		if(desc != null && !desc.trim().isEmpty())
			this.longDescription = desc.trim();
	}
	
	public void setBusy(Boolean busy) {
		this.busy = busy;
	}
	
	public void setBusy(String busy) {
		if(busy == null)
			this.busy = Boolean.valueOf(false);
		else {
			busy = busy.trim();
			if(busy.equalsIgnoreCase("on"))
				this.busy = Boolean.valueOf(true);
			else
				this.busy = Boolean.valueOf(busy);
		}
	}
	
	public void setLocation(String location) {
		this.location = null;
		if(location != null && !location.trim().isEmpty())
			this.location = location.trim();
	}
	
	public void setCreationTime(java.util.Date timestamp) {
		this.createdOn = timestamp;
	}
	
	public void setCreationTime(String timestamp) {
		try {
			this.createdOn = DATE_FORMAT.parse(timestamp);
		} catch(ParseException e) {
			this.createdOn = null;
		}
	}
	
	public void setStartTime(java.util.Date time) {
		this.startTime = time;
	}
	
	public void setStartTime(String time) {
		try {
			this.startTime = null;
			if(time != null)
				this.startTime = DATE_FORMAT.parse(time);
		} catch(ParseException e) {
			this.startTime = null;
		}
	}
	
	public void setEndTime(java.util.Date time) {
		this.endTime = time;
	}
	
	public void setEndTime(String time) {
		try {
			this.endTime = null;
			if(time != null)
				this.endTime = DATE_FORMAT.parse(time);
		} catch(ParseException e) {
			this.endTime = null;
		}
	}
	
	public Integer getId() {
		return this.id;
	}
	
	public Account getOwner() throws DatabaseException {
		if(this.owner != null && this.owner.getId() != null && this.reloadOwner) {
			this.owner.get();
		}
		return this.owner;
	}
	
	public String getShortDescription() {
		return this.shortDescription;
	}
	
	public String getLongDescription() {
		return this.longDescription;
	}
	
	public String getLocation() {
		return this.location;
	}
	
	public Boolean isBusy() {
		return this.busy;
	}
	
	public java.util.Date getCreationTime() {
		return this.createdOn;
	}
	
	public java.util.Date getStartTime() {
		return this.startTime;
	}
	
	public java.util.Date getEndTime() {
		return this.endTime;
	}
	
	public void addTaskType(Integer id) throws DatabaseException {
		TaskType type = new TaskType(this.conn);
		type.setId(id);
		if(type.get())
			this.taskTypes.put(type.getName(), type);
		this.updateTaskTypes = true;
		this.reloadTaskTypes = true;
	}
	
	public void addTaskType(String name) throws DatabaseException {
		TaskType type = new TaskType(this.conn);
		type.setName(name);
		this.taskTypes.put(name, type);
		this.updateTaskTypes = true;
		this.reloadTaskTypes = true;
	}
	
	public void removeTaskType(String name) throws DatabaseException {
		this.taskTypes.remove(name);
		this.updateTaskTypes = true;
		this.reloadTaskTypes = true;
	}
	
	public Set<TaskType> getTaskTypes() throws DatabaseException {
		if(this.reloadTaskTypes) {
			this.taskTypes.clear();
			for(TaskType t : new TaskType(this.conn).list(this))
				this.taskTypes.put(t.getName(), t);
		}
		this.updateTaskTypes = false;
		this.reloadTaskTypes = false;
		return new HashSet<TaskType>(this.taskTypes.values());
	}
	
	//other methods
	
	public boolean hasTaskType(String name) throws DatabaseException {
		if(this.reloadTaskTypes && this.id != null) this.getTaskTypes();
		return this.taskTypes.containsKey(name);
	}
	
	//persistence
	
	public void setConnection(Connection conn) {
		this.conn = conn;
	}
	
	public void revive(ResultSet cursor) throws DatabaseException {
		try {
			this.reviveKeys(cursor);
			
			Integer owner_id = cursor.getInt("owner");
			if(cursor.wasNull())
				owner_id = null;
			if(owner_id != null) {
				this.owner = new Account();
				this.owner.setId(owner_id);
				this.updateOwner = false;
				this.reloadOwner = true;
			}
			
			this.shortDescription = cursor.getString("short_desc");
			if(this.shortDescription != null)
				this.shortDescription = this.shortDescription.trim();
			
			this.longDescription = cursor.getString("long_desc");
			if(this.longDescription != null)
				this.longDescription = this.longDescription.trim();
			
			this.location = cursor.getString("location");
			if(this.location != null)
				this.location = this.location.trim();
			
			this.busy = cursor.getBoolean("busy");
			if(cursor.wasNull())
				this.busy = null;
			
			this.createdOn = cursor.getTimestamp("created_on");
			this.startTime = cursor.getTimestamp("start_time");
			this.endTime = cursor.getTimestamp("end_time");
			
			this.taskTypes.clear();
			this.updateTaskTypes = false;
			this.reloadTaskTypes = true;
		} catch(SQLException e) {
			throw new DatabaseException(e);
		}
	}
	
	public void reviveKeys(ResultSet cursor) throws DatabaseException {
		try {
			this.id = cursor.getInt("id");
			if(cursor.wasNull())
				this.id = null;
		} catch(SQLException e) {
			throw new DatabaseException(e);
		}
	}
	
	public void add() throws DatabaseException {
		if(this.owner == null || this.owner.getId() == null)
			throw new DatabaseException("Owner cannot be null.");
		if(this.shortDescription == null)
			throw new DatabaseException("Short description cannot be null nor empty.");
		if(this.startTime == null)
			throw new DatabaseException("Start time cannot be null.");
		if(this.endTime == null)
			throw new DatabaseException("End time cannot be null.");
		
		SimpleDateFormat df = new SimpleDateFormat("''yyyy-MM-dd HH:mm:ss''");
		String longDesc = (this.longDescription == null)? "NULL": "'" + this.longDescription + "'";
		String busyStr = (this.busy == null)? "NULL": this.busy.toString();
		String loc = (this.location == null)? "NULL": "'" + this.location + "'";
		String created = (this.createdOn == null)? "DEFAULT": df.format(this.createdOn);
		
		try {
			this.conn.setTransactionIsolation(Connection.TRANSACTION_READ_COMMITTED);
			this.conn.setAutoCommit(false);
			
			String query = String.format(
				"INSERT INTO task (owner, short_desc, long_desc, busy, location, " +
				"created_on, start_time, end_time) VALUES (%d, '%s', %s, %s, %s, %s, %s, %s)", 
				this.owner.getId(), this.shortDescription, longDesc, busyStr, loc,
				created, df.format(this.startTime), df.format(this.endTime));
			DatabaseUtilities.executeInsert(this.conn, query, this, "id", "created_on");
			
			if(this.id == null)
				throw new DatabaseException("Could not retrieve generated task id.");
			
			for(TaskType t : this.taskTypes.values()) {
				if((t.getId() != null) || (t.getId() == null && t.get())) {
					query = String.format(
						"INSERT INTO task_task_type (task_id, type_id) " + 
						"VALUES (%d, %d)", this.id, t.getId());
					DatabaseUtilities.executeInsert(this.conn, query, this);
				}
			}
			
			this.updateOwner = false;
			this.updateTaskTypes = false;
		} catch(SQLException sqle) {
			try {
				this.conn.rollback();
			} catch(SQLException e) {
			}
			throw new DatabaseException(sqle);
		} catch(DatabaseException dbe) {
			try {
				this.conn.rollback();
			} catch(SQLException e) {
			}
			throw dbe;
		} finally {
			try {
				this.conn.setAutoCommit(true);
			} catch(SQLException e) {
				throw new DatabaseException(e);
			}
		}
	}
	
	public boolean remove() throws DatabaseException {
		if(this.id == null)
			throw new DatabaseException("Need an id for this operation.");
		
		this.reloadOwner = false;
		this.updateOwner = true;
		this.reloadTaskTypes = false;
		this.updateTaskTypes = true;
		
		String query = String.format(
			"DELETE FROM task WHERE id = %d", this.id); //cascades
		return DatabaseUtilities.executeUpdate(this.conn, query);
	}
	
	public boolean get() throws DatabaseException {
		if(this.id == null && this.shortDescription == null)
			throw new DatabaseException("Need an id or a name for this operation.");
		
		String filter = (this.shortDescription != null)?
			String.format("short_desc = '%s'", this.shortDescription):
			String.format("id = %d", this.id);
		
		String query = String.format("SELECT * FROM task WHERE %s", filter);
		return DatabaseUtilities.getSingleRecord(this.conn, query, this);
	}
	
	public boolean set() throws DatabaseException {
		if(this.id == null)
			throw new DatabaseException("Need an id for this operation.");
		if(this.owner == null || this.owner.getId() == null)
			throw new DatabaseException("Owner cannot be null.");
		if(this.shortDescription == null)
			throw new DatabaseException("Short description cannot be null nor empty.");
		if(this.startTime == null)
			throw new DatabaseException("Start time cannot be null.");
		if(this.endTime == null)
			throw new DatabaseException("End time cannot be null.");
		
		SimpleDateFormat df = new SimpleDateFormat("''yyyy-MM-dd HH:mm:ss''");
		String longDesc = (this.longDescription == null)? "NULL": "'" + this.longDescription + "'";
		String busyStr = (this.busy == null)? "NULL": this.busy.toString();
		String loc = (this.location == null)? "NULL": "'" + this.location + "'";
		
		try {
			this.conn.setTransactionIsolation(Connection.TRANSACTION_READ_COMMITTED);
			this.conn.setAutoCommit(false);
			
			String query = String.format(
				"UPDATE task SET owner = %d, short_desc = '%s', long_desc = %s, " +
				"busy = %s, location = %s, start_time = %s, end_time = %s " + 
				"WHERE id = %d", this.owner.getId(), this.shortDescription, longDesc, 
				busyStr, loc, df.format(this.startTime), df.format(this.endTime), this.id);
			boolean r = DatabaseUtilities.executeUpdate(this.conn, query);
			
			//clear all previous task types
			query = String.format("DELETE FROM task_task_type WHERE task_id = %d", this.id);
			DatabaseUtilities.executeUpdate(this.conn, query);
			
			//re-insert new task types
			for(TaskType t : this.taskTypes.values()) {
				if((t.getId() != null) || (t.getId() == null && t.get())) {
					query = String.format(
						"INSERT INTO task_task_type (task_id, type_id) " + 
						"VALUES (%d, %d)", this.id, t.getId());
					DatabaseUtilities.executeInsert(this.conn, query, this);
				}
			}
			this.updateOwner = false;
			this.updateTaskTypes = false;
			return r;
		} catch(SQLException sqle) {
			try {
				this.conn.rollback();
			} catch(SQLException e) {
			}
			throw new DatabaseException(sqle);
		} catch(DatabaseException dbe) {
			try {
				this.conn.rollback();
			} catch(SQLException e) {
			}
			throw dbe;
		} finally {
			try {
				this.conn.setAutoCommit(true);
			} catch(SQLException e) {
				throw new DatabaseException(e);
			}
		}
	}
	
	//data sets
	
	public Cursor list() throws DatabaseException {
		String query = String.format("SELECT * FROM task");
		ResultSet cursor = DatabaseUtilities.executeQuery(this.conn, query);
		return new Cursor(cursor);
	}
	
	public Cursor list(TaskType t) throws DatabaseException {
		if(t.getId() == null)
			return this.list();
		
		String query = String.format("SELECT t.* FROM task t " +
			"JOIN task_task_type r ON (t.id = r.task_id) " + 
			"WHERE r.type_id = %d", t.getId());
		
		ResultSet cursor = DatabaseUtilities.executeQuery(this.conn, query);
		return new Cursor(cursor);
	}
	
	public Cursor list(Account acc) throws DatabaseException {
		if(acc.getId() == null)
			return this.list();
		
		String query = String.format("SELECT t.* FROM task t " +
			"JOIN account a ON (a.id = t.owner) " + 
			"WHERE a.id = %d ORDER BY start_time DESC, end_time, busy DESC, short_desc", 
			acc.getId());
		
		ResultSet cursor = DatabaseUtilities.executeQuery(this.conn, query);
		return new Cursor(cursor);
	}
	
	public Cursor list(Account acc, TaskType type) throws DatabaseException {
		if(acc.getId() == null)
			return this.list(type);
		if(type.getId() == null)
			return this.list(acc);
		
		String query = String.format("SELECT t.* FROM task t " +
			"JOIN account a ON (a.id = t.owner) " + 
			"JOIN task_task_type r ON (t.id = r.task_id) " + 
			"WHERE a.id = %d AND r.type_id = %d " +
			"ORDER BY start_time DESC, end_time, busy DESC, short_desc", 
			acc.getId(), type.getId());
		
		ResultSet cursor = DatabaseUtilities.executeQuery(this.conn, query);
		return new Cursor(cursor);
	}
	
	public class Cursor extends Task implements DatabaseObject.Cursor<Task> {
		
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
	
		public java.util.Iterator<Task> iterator() {
			return new Iterator(this);
		}
		
		public List<Task> load() throws DatabaseException {
			List<Task> l = new LinkedList<Task>();
			for(Task a : this)
				l.add(a);
			return l;
		}
		
	}
	
	public class Iterator implements java.util.Iterator<Task> {
		
		private Cursor cursor;
		private Task nextItem;
		
		public Iterator(Cursor c) {
			if(c == null)
				throw new NullPointerException();
			
			try {
				this.cursor = c;
				this.nextItem = c.next()? (Task) c.clone(): null;
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
		public Task next() {
			Task r = nextItem;
			try {
				this.nextItem = cursor.next()? (Task) cursor.clone(): null;
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
