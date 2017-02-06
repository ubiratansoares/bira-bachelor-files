package ecommerce.model;

import javax.persistence.*;

/**
 * Modela um vendedor (gerencia produtos, mas não tem
 * privilégios para gerenciar usuários).
 */
@Entity
public class Retailer extends Account {
	
	/* == atributos e construtores == */
	
	@Basic(optional=false)
	private String title;
	
	private String department;
	
	@ManyToOne(optional=false)
	private SysAdmin supervisor;
	
	public Retailer() {
	}
	
	public Retailer(SysAdmin supervisor, String title) {
		this.title = title;
		this.department = supervisor.getDepartment();
		this.supervisor = supervisor;
	}
	
	/* == getters e setters == */
	
	/**
	 * Define o cargo do vendedor.
	 */
	public void setTitle(String title) {
		this.title = title;
	}
	
	public String getTitle() {
		return title;
	}
	
	/**
	 * Define o departamento do vendedor.
	 */
	public void setDepartment(String dept) {
		this.department = dept;
	}
	
	public String getDepartment() {
		return department;
	}
	
	/**
	 * Define o supervisor do vendedor.
	 */
	public void setSupervisor(SysAdmin admin) {
		this.supervisor = admin;
	}
	
	public SysAdmin getSupervisor() {
		return supervisor;
	}
	
}
