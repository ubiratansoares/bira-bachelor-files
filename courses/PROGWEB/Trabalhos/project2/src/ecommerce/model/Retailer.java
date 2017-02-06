package ecommerce.model;

import javax.persistence.*;

/**
 * Modela um vendedor. O vendedor gerencia produtos e usuários
 * (administra o sistema).
 */
@Entity
public class Retailer extends Account {
	
	/* == atributos e construtores == */
	
	@Basic(optional=false)
	private String title;
	
	private String department;
	
	public Retailer() {
	}
	
	public Retailer(String department, String title) {
		this.department = department;
		this.title = title;
	}
	
	/* == getters e setters == */
	
	/**
	 * Define o cargo do vendedor.
	 */
	public void setTitle(String title) {
		if(title == null || title.trim().isEmpty())
			throw new IllegalArgumentException();
		this.title = title;
	}
	
	public String getTitle() {
		return title;
	}
	
	/**
	 * Define o departamento do vendedor.
	 */
	public void setDepartment(String dept) {
		if(title != null && title.trim().isEmpty())
			title = null;
		this.department = dept;
	}
	
	public String getDepartment() {
		return department;
	}
	
}
