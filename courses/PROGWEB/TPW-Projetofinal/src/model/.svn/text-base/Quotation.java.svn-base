package model;

import java.util.Date;
import javax.persistence.*;

import util.HibernateUtil;

/**
 * Representa o preço instantâneo de uma ação. O preço
 * instantâneo é calculado a cada transação envolvendo
 * ações.
 */
@Entity
public class Quotation {

	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;
	
	@ManyToOne(optional=false)
	private Company company;
	
	@Basic(optional=false)
	@Temporal(TemporalType.TIMESTAMP)
	private Date tstamp;
	
	@Basic(optional=false)
	@Column(precision=15, scale=2)
	private Double value;

	/**
	 * Cria uma cotação com a data e a hora atuais, e
	 * com valor zero.
	 */
	public Quotation() {
		this.tstamp = new Date();
		this.value = 0.00;
	}
	
	
	
	/* == setters e getters == */

	/**
	 * Obtém o identificador da cotação.
	 */
	public Integer getId() {
		return id;
	}

	@SuppressWarnings("unused")
	private void setId(Integer id) {
		this.id = id;
	}

	/**
	 * Obtém a companhia que lançou as ações.
	 * 
	 * @return
	 * A companhia que lançou as ações.
	 */
	public Company getCompany() {
		return company;
	}

	public void setCompany(Company company) {
		if(company == null)
			throw new IllegalArgumentException();
		this.company = company;
	}

	/**
	 * Obtém a data e a hora da cotação.
	 * 
	 * @return
	 * A data e a hora da cotação.
	 */
	public Date getTimestamp() {
		return tstamp;
	}

	public void setTimestamp(Date tstamp) {
		if(tstamp == null)
			throw new IllegalArgumentException();
		this.tstamp = tstamp;
	}

	/**
	 * Obtém o valor da cotação (por ação).
	 * 
	 * @return
	 * O valor unitário das ações no momento indicado.
	 */
	public Double getValue() {
		return value;
	}

	public void setValue(Double value) {
		if(value == null || value.isNaN())
			throw new IllegalArgumentException();
		this.value = value;
	}

	
	
	/* == teste == */
	
	@Override
	public String toString() {
		return String.format("Quotation [company=%s, tstamp=%s, value=%s]",
				company, tstamp, value);
	}
	
	private static void testSave() {
		Company c = new Company();
		c.setCode("TEST2");
		c.setName("Test Company");
		HibernateUtil.getSession().save(c);
		System.out.println(c);
		
		Quotation q = new Quotation();
		q.setCompany(c);
		q.setValue(12.00);
		HibernateUtil.getSession().save(q);
		System.out.println(q);
	}
	
	private static void testLoad() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Quotation.class).list();
		System.out.println(list);
	}
	
	private static void testRemove() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Quotation.class).list();
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
