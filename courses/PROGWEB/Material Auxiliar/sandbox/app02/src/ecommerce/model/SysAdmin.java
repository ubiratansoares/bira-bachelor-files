package ecommerce.model;

import ecommerce.util.HibernateUtil;

import org.hibernate.Session;

import java.util.HashSet;
import java.util.Set;

import javax.persistence.*;

/**
 * Modela um Administrador do Sistema.
 */
@Entity
public class SysAdmin extends Account {
	
	/* == atributos e construtores == */
	
	@Basic(optional=false)
	private String title;
	
	@Basic
	private String department;
	
	@OneToMany(mappedBy="supervisor")
	private Set<Retailer> supervised;
	
	public SysAdmin() {
		this.supervised = new HashSet<Retailer>();
	}
	
	public SysAdmin(String title, String dept) {
		this.title = title;
		this.department = dept;
		this.supervised = new HashSet<Retailer>();
	}
	
	/* == setter e getters == */
	
	/**
	 * Define o cargo do administrador.
	 */
	public void setTitle(String title) {
		this.title = title;
	}
	
	public String getTitle() {
		return title;
	}
	
	/**
	 * Define o departamento do administrador.
	 */
	public void setDepartment(String dept) {
		this.department = dept;
	}
	
	public String getDepartment() {
		return department;
	}
	
	/**
	 * Define as contas supervisionadas por este adminstrador.
	 */
	private void setSupervisedRetailers(Set<Retailer> retailers) {
		this.supervised = retailers;
	}
	
	public Set<Retailer> getSupervisedRetailers() {
		return supervised;
	}
	
	/* == testes == */
	
	private static void testSave(Session session) {
		System.out.println("Testing save...");
		
		SysAdmin sa = new SysAdmin();
		sa.setEmail("test@test.org");
		sa.setPassword("password");
		sa.setTitle("System Analyst");
		sa.getName().setFirstName("First");
		sa.getName().setLastName("Test");
		sa.getAddress().setName("Nowhere");
		sa.getAddress().setCity("Springville");
		sa.getAddress().setCountry("USA");
		sa.getAddress().setPostalCode("00000");
		sa.setStatus(Status.ACTIVE);
		sa.setBirthDate(new java.util.Date());
		System.out.println(sa);
		
		session.save(sa);
	}
	
	private static void testLoad(Session session) {
		System.out.println("Testing load...");
		SysAdmin a = (SysAdmin) session.load(SysAdmin.class, 1);
		System.out.println(a);
	}
	
	public static void main(String[] args) {
		try {
			Session session = HibernateUtil.getSession();
			session.beginTransaction();
			testSave(session);
			testLoad(session);
			session.getTransaction().commit();
		} catch(ArrayIndexOutOfBoundsException e) {
			//swallow
		} catch(Exception e) {
			e.printStackTrace();
		} finally {
			HibernateUtil.getSessionFactory().close();
		}
	}
	
}
