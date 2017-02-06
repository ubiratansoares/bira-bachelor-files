package util;

import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.List;

import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

import model.Account;
import model.Company;

import org.hibernate.Session;

public class ContextListener implements ServletContextListener {
	
	public ContextListener() {
	}
	
	/**
	 * Verifica se existe algum administrador do sistema, e caso não haja, 
	 * cria um padrão. Email: "root@default.org"; senha: "root". Recomenda-se 
	 * usá-lo apenas para criar os demais usuários, e deve ser excluí-do após 
	 * cumprir essa função.
	 */
	@Override
	public void contextInitialized(ServletContextEvent ev) {
		Thread t = new Thread(new Runnable() {
			
			@Override
			public void run() {
				Session session = HibernateUtil.getSession();
				session.beginTransaction();
				
				Account owner = (Account) session.createQuery(
						"from Account where privilege = :p")
						.setParameter("p", Account.Privilege.OWNER)
						.uniqueResult();
				
				if(owner == null) {
					owner = new Account();
					owner.setName("Default Owner");
					owner.setUsername("root");
					owner.setPassword("root");
					owner.setPrivilege(Account.Privilege.OWNER);
					owner.setBalance(Double.POSITIVE_INFINITY);
					session.save(owner);
				}
				
				session.getTransaction().commit();
			}
			
		});
		
		Thread t2 = new Thread(new Runnable() {
			public void run(){
				Session session = HibernateUtil.getSession();
				session.beginTransaction();
				
				/*
				 * FIXME Apaga isso aqui (soh para testes)
				 */
				/*Company c = new Company();
				c.setCode("PETR4.SA");
				c.setName("Petrobras");
				session.save(c);
				
				c = new Company();
				c.setCode("BBAS3.SA");
				c.setName("Banco do Brasil");
				session.save(c);
				
				c = new Company();
				c.setCode("ELPL4.SA");
				c.setName("Eletropaulo");
				session.save(c);*/
				
				
				//END FIXME
				
				List list = session.createCriteria(Company.class).list();
				for (Object o: list){
					Company co = (Company) o;
					
					Calendar cal = new GregorianCalendar();
					
					YFinanceBot.populateStocks("1", "1", "1900", String.valueOf(cal.get(Calendar.DAY_OF_MONTH)), 
							String.valueOf(cal.get(Calendar.MONTH)+1), String.valueOf(cal.get(Calendar.YEAR)), co);
				}
				session.getTransaction().commit();
			}
		});
		
		try {
			t.start();
			t.join();
			t2.start();
			t2.join();
			System.out.println("INFO: System owner existence checked.");
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
