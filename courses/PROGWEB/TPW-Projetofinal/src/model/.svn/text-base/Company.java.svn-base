package model;

import java.util.*;
import javax.persistence.*;

import util.HibernateUtil;

/**
 * Representa uma empresa oferecendo ações.
 */
@Entity
public class Company {

	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;
	
	@Basic(optional=false)
	@Column(unique=true, length=15)
	private String code;
	
	@Basic(optional=false)
	private String name;
	
	@OneToMany(mappedBy="company")
	private List<Quotation> quotations;

	/**
	 * Construtor vazio.
	 */
	public Company() {
		this.quotations = new LinkedList<Quotation>();
	}
	
	
	
	/* == setters e getters == */

	/**
	 * Obtém o identificador da companhia. Este é o identificador
	 * alocado pela base de dados, e usado como chave primária.
	 * Não representa uma chave natural.
	 */
	public Integer getId() {
		return id;
	}

	@SuppressWarnings("unused")
	private void setId(Integer id) {
		this.id = id;
	}

	/**
	 * Obtém o código da empresa na bolsa de valores.
	 * Esta é uma chave natural.
	 * 
	 * @return
	 * O código da empresa.
	 */
	public String getCode() {
		return code;
	}

	public void setCode(String code) {
		this.code = code;
	}

	/**
	 * Obtém o nome da empresa.
	 * 
	 * @return
	 * O nome da empresa.
	 */
	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	/**
	 * Obtém as cotações da empresa.
	 * 
	 * @return
	 * As cotações da empresa.
	 */
	public List<Quotation> getQuotations() {
		return quotations;
	}

	@SuppressWarnings("unused")
	private void setQuotations(List<Quotation> quotations) {
		this.quotations = quotations;
	}
	
	
	
	/* == teste == */
	
	@Override
	public String toString() {
		return String.format("Company [code=%s, name=%s]", code, name);
	}
	
	private static void testSave() {
		Company c = new Company();
		c.setCode("TEST3");
		c.setName("Test Company");
		HibernateUtil.getSession().save(c);
		System.out.println(c);
	}
	
	private static void testLoad() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Company.class).list();
		System.out.println(list);
	}
	
	private static void testRemove() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Company.class).list();
		HibernateUtil.getSession().delete(list.get(0));
	}
	
	public static void main(String[] args) {
		try {
			HibernateUtil.getSession().beginTransaction();
			testSave();
			testLoad();
			testRemove();
			testLoad();
			HibernateUtil.getSession().getTransaction().commit();
		} catch(Exception e) {
			HibernateUtil.getSession().getTransaction().rollback();
			e.printStackTrace();
		}
	}
	
}
