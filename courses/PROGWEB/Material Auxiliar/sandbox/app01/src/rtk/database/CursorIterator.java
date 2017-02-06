package rtk.database;

import java.util.*;

public class CursorIterator<T> implements Iterator<T> {
	
	private Cursor<T> cursor;
	private T nextElement;
	
	public CursorIterator(Cursor<T> cursor) {
		try {
			this.cursor = cursor;
			this.nextElement = cursor.next()? cursor.clone(): null;
		} catch(DatabaseException e) {
			this.nextElement = null;
		}
	}
	
	public boolean hasNext() {
		return (nextElement != null);
	}
	
	public T next() {
		try {
			T rv = this.nextElement;
			this.nextElement = cursor.next()? cursor.clone(): null;
			return rv;
		} catch(DatabaseException e) {
			return null;
		}
	}
	
	public void remove() {
		throw new UnsupportedOperationException("Operation not supported.");
	}
	
}
