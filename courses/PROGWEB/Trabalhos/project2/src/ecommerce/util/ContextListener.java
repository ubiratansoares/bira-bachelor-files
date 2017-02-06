package ecommerce.util;

import ecommerce.model.*;
import ecommerce.util.*;

import java.util.*;

import javax.servlet.*;

import org.hibernate.*;

public class ContextListener implements ServletContextListener {
	
	public ContextListener() {
	}
	
	/**
	 * Verifica se existe algum vendedor (administrador do sistema),
	 * e caso não haja, cria um padrão. Email: "root@default.org";
	 * senha: "root". Recomenda-se usá-lo apenas para criar os demais
	 * usuários, e deve ser excluído após cumprir essa função.
	 */
	@Override
	public void contextInitialized(ServletContextEvent ev) {
		Thread t = new Thread(new Runnable() {
			
			@Override
			public void run() {
				Session session = HibernateUtil.getSession();
				session.beginTransaction();
				List list = session.createCriteria(Retailer.class).list();
				if(list.isEmpty()) {
					Retailer root = new Retailer();
					root.setEmail("root@default.org");
					root.setPassword("root");
					root.getName().setFirstName("Default");
					root.getName().setLastName("Root");
					root.getAddress().setName("nowhere");
					root.getAddress().setCity("nowhere");
					root.getAddress().setCountry("nowhere");
					root.getAddress().setPostalCode("nowhere");
					root.setStatus(Account.Status.ACTIVE);
					root.setBirthDate(new java.util.Date());
					root.setLocale(Locale.ROOT);
					root.setTitle("Default System Administrator");
					session.save(root);
				}
				session.getTransaction().commit();
			}
			
		});
		try {
			t.start();
			t.join();
			System.out.println("INFO: SysAdmin check completed.");
		} catch(Exception e) {
			e.printStackTrace();
		}
		return;
	}
	
	@Override
	public void contextDestroyed(ServletContextEvent ev) {
		HibernateUtil.getSessionFactory().close();
	}
	
}
