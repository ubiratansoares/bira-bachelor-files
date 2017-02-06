package model;

import javax.persistence.*;

import model.Account.Privilege;

import util.HibernateUtil;

/**
 * Representa um item da carteira de investimentos.
 */
@Entity
@Table(uniqueConstraints=@UniqueConstraint(
		columnNames={"company_id", "owner_id"}))
public class Stock {

	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;
	
	@ManyToOne(optional=false)
	private Company company;
	
	@ManyToOne(optional=false)
	private Investor owner;
	
	@Basic(optional=false)
	private Integer amount;
	
	/**
	 * Construtor vazio.
	 */
	public Stock() {
		this.amount = 0;
	}

	
	
	/* == setters e getters == */
	
	/**
	 * Obtém o identificador do sistema. Não é utilizado na
	 * aplicação (serve apenas como chave primária), pois a
	 * chave natural é a associação entre investidor e 
	 * companhia.
	 */
	public Integer getId() {
		return id;
	}

	@SuppressWarnings("unused")
	private void setId(Integer id) {
		this.id = id;
	}

	/**
	 * Obtém a companhia à qual estas ações se referem.
	 *  
	 * @return
	 * A companhia associada a estas ações.
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
	 * Obtém o investidor que possui estas ações.
	 * 
	 * @return
	 * O investidor que possui estas ações.
	 */
	public Investor getOwner() {
		return owner;
	}

	public void setOwner(Investor owner) {
		if(owner == null)
			throw new IllegalArgumentException();
		this.owner = owner;
	}

	/**
	 * Obtém a quantidade de ações que o investidor possui
	 * da companhia correspondente.
	 *  
	 * @return
	 * A quantidade de ações.
	 */
	public Integer getAmount() {
		return amount;
	}

	public void setAmount(Integer amount) {
		if(amount == null || amount < 0)
			throw new IllegalArgumentException();
		this.amount = amount;
	}
	
	
	
	/* == teste == */
	
	@Override
	public String toString() {
		return String.format("Stock [company=%s, owner=%s, amount=%s]",
				company, owner, amount);
	}
	
	private static void testSave() {
		Company c = new Company();
		c.setCode("TEST4");
		c.setName("Test Company");
		HibernateUtil.getSession().save(c);
		System.out.println(c);
		
		Account acc = new Account();
		acc.setName("Test User");
		acc.setUsername("test");
		acc.setPassword("test");
		acc.setPrivilege(Privilege.NORMAL);
		HibernateUtil.getSession().save(acc);
		System.out.println(acc);
		
		Stock s = new Stock();
		s.setOwner(acc);
		s.setCompany(c);
		s.setAmount(123);
		HibernateUtil.getSession().save(s);
		System.out.println(s);
	}
	
	private static void testLoad() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Stock.class).list();
		System.out.println(list);
	}
	
	private static void testRemove() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Stock.class).list();
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
