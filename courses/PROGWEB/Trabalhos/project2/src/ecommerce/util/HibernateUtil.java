package ecommerce.util;

import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.cfg.Configuration;

public class HibernateUtil {
	
	private static final SessionFactory sf = buildSessionFactory();
	
	private static SessionFactory buildSessionFactory() {
		try {
			return new Configuration().configure().buildSessionFactory();
		} catch(Exception e) {
			throw new ExceptionInInitializerError(e);
		}
	}
	
	public static SessionFactory getSessionFactory() {
		return sf;
	}
	
	/**
	 * Método de conveniência. Apenas invoca 
	 * {@link org.hibernate.SessionFactory#getCurrentSession()}.
	 */
	public static Session getSession() {
		return sf.getCurrentSession();
	}
	
}
