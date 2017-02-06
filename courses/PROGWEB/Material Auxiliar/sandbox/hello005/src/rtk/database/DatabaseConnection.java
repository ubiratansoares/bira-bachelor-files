package rtk.database;

import java.sql.*;
import java.util.*;

public class DatabaseConnection {
	
	/* ===== connection pooling ===== */
	
	public synchronized Connection getConnection() throws DatabaseException {
		try {
			if(pool.isEmpty()) {
				this.lastMiss = System.currentTimeMillis();
				if(watched.size() < upperBound || upperBound == 0) {
					Connection conn = DriverManager.getConnection(
							this.url, this.username, this.password);
					watched.add(conn);
					return new ProxyConnection(this, conn);
				} else
					throw new DatabaseException("Connection limit exceeded.");
			} else {
				return new ProxyConnection(this, pool.pop());
			}
		} catch(SQLException e) {
			throw new DatabaseException(e);
		}
	}
	
	public synchronized void close(Connection conn) throws SQLException {
		if(conn instanceof ProxyConnection)
			return; //proxies are not supposed to be pooled
		if(conn.isClosed()) {
			watched.remove(conn);
			return; //someone has closed this connection, throw away
		}
		pool.push(conn); //put it back in the pool
	}
	
	public synchronized void close() throws DatabaseException {
		Exception lastException = null;
		for(Connection conn : watched) {
			try {
				try {
					if(!conn.getAutoCommit())
						conn.rollback(); //cancela transações pendentes
				} catch(SQLException e) {
					e.printStackTrace();
				}
				conn.close();
				ConnectionManager.getInstance().addMessage("INFO: Closed pooled connection.");
			} catch(SQLException e) {
				ConnectionManager.getInstance().addMessage("ERROR: Failed to close pooled connection.");
				lastException = e;
			}
		}
		watched.clear();
		pool.clear();
		
		watcher.stop();
		
		if(lastException != null)
			throw new DatabaseException("Failed to close one or more pooled connections.", lastException);
	}
	
	private Set<Connection> watched = new HashSet<Connection>();
	private Deque<Connection> pool = new ArrayDeque<Connection>();
	private PoolManager watcher = new PoolManager();
	private long lastMiss = System.currentTimeMillis();
	
	/* ===== constructor, finalizer ===== */
	
	public DatabaseConnection(String key, String url) {
		this.key = key;
		this.url = url;
		new Thread(watcher).start();
	}
	
	@Override
	public void finalize() {
		try {
			this.close();
		} catch(DatabaseException e) {
			e.printStackTrace();
		}
	}
	
	/* ===== attributes, setters, getters ===== */
	
	private String key = null;
	private String dialect = null;
	
	private String url = null;
	private String username = null;
	private String password = null;
	
	private int lowerBound = 0;
	private int upperBound = 0;
	private long timeout = 0L;
	
	public String getIdentifier() {
		return key;
	}
	
	public void setUsername(String name) {
		this.username = name;
	}
	
	public void setPassword(String password) {
		this.password = password;
	}
	
	public void setDialect(String dialectID) {
		this.dialect = dialectID;
	}
	
	public void setMinimumPoolSize(int size) {
		this.lowerBound = size;
		if(this.lowerBound > this.upperBound && this.upperBound != 0)
			this.upperBound = size;
	}
	
	public void setMaximumPoolSize(int size) {
		this.upperBound = size;
		if(this.upperBound < this.lowerBound)
			this.lowerBound = size;
	}
	
	public void setIdleTimeout(int seconds) {
		this.timeout = 1000 * seconds;
	}
	
	/* ===== test ===== */
	
	@Override
	public String toString() {
		return String.format("%s:%s", this.url, this.username);
	}
	
	public static void main(String[] args) throws Exception {
		Random r = new Random(System.currentTimeMillis());
		
		Connection conn = null; int value;
		ConnectionManager.getInstance().open("cfg/db-config.xml");
		List<Connection> test = new ArrayList<Connection>();
		for(int i = 0; i < 50; i++) {
			try {
				switch(r.nextInt(3)) {
					case 0:
//						System.out.println("Opening connection " + test.size() + ".");
						conn = ConnectionManager.getInstance().getConnection("main");
						test.add(conn);
						break;
						
					case 1:
						value = r.nextInt(10);
//						System.out.println("Waiting " + value + " milliseconds.");
						Thread.currentThread().sleep(value);
						break;
						
					case 2:
						if(test.size() > 0) {
							value = r.nextInt(test.size());
//							System.out.println("Closing connection " + value + ".");
							conn = test.remove(value);
							conn.close();
						}
						break;
				}
				System.out.println(ConnectionManager.getInstance().connections.get("main").pool.size() +
					"/" + ConnectionManager.getInstance().connections.get("main").watched.size());
			} catch(DatabaseException e) {
				System.out.println(e.getMessage());
			}
		}
		for(Connection c : test)
			c.close();
		Thread.currentThread().sleep(5000);
		System.out.println("***");
		System.out.println(ConnectionManager.getInstance().connections.get("main").pool.size() +
			"/" + ConnectionManager.getInstance().connections.get("main").watched.size());
		conn.close();
		ConnectionManager.getInstance().close();
	}
	
	/* ===== nested classes ===== */
	
	private class PoolManager implements Runnable {
		
		private boolean stopFlag = false;
		private Thread thread = null;
		
		public synchronized void stop() {
			this.stopFlag = true;
			this.thread.interrupt();
		}
		
		@Override
		public void run() {
			boolean stop = false;
			this.thread = Thread.currentThread();
			do {
				synchronized(DatabaseConnection.this) {
					//throw away closed connections
					for(Connection conn : watched.toArray(new Connection[0])) {
						try {
							if(conn.isClosed()) {
								watched.remove(conn);
								pool.remove(conn);
							}
						} catch(SQLException e) {
							e.printStackTrace();
						}
					}
					
					//create minimum number of connections
					int tries = 0;
					while(watched.size() < lowerBound && tries < 3) {
						try {
							Connection conn = DriverManager.getConnection(
									url, username, password);
							watched.add(conn);
							pool.add(conn);
							tries = 0;
						} catch(SQLException e) {
							e.printStackTrace();
							tries++;
						}
					}
					
					//throw away idle connections
					long now = System.currentTimeMillis();
					if(watched.size() > lowerBound && 
							(now - lastMiss > timeout)) {
						try {
							Connection conn = pool.pop();
							watched.remove(conn);
							conn.close();
						} catch(SQLException e) {
							e.printStackTrace();
						} catch(NoSuchElementException e) {
							e.printStackTrace();
						}
					}
				}
				
				try {
					thread.sleep(100); //10ms
				} catch(InterruptedException e) {
				}
				
				synchronized(this) {
					stop = stopFlag;
				}
			} while(!stop);
		}
		
	}
	
}
