package model;

import javax.persistence.*;

import model.Account.Privilege;
import util.HibernateUtil;

/**
 * Representa uma oferta de ações no mercado.
 */
@Entity
public class Offer {

	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;
	
	@ManyToOne(optional=false)
	private Company company;
	
	@Column(precision=15, scale=2)
	private Double price;
	
	@Basic(optional=false)
	private Integer amount;

	@ManyToOne(optional=false)
	private Investor owner;
	
	@Basic(optional=false)
	@Enumerated(EnumType.STRING)
	private Type type;
	
	/**
	 * Cria uma oferta gratuita sem ações.
	 */
	public Offer() {
		this.amount = 0;
		this.price = 0.00;
	}
	
	
	
	/* == setters e getters == */

	/**
	 * Obtém o identiicador da oferta.
	 * 
	 * @return
	 * O identificador da oferta.
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
	 * A companhia ao qual se referem as ações.
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
	 * Obtém o preço esperado pelo vendedor.
	 * 
	 * @return
	 * O preço pelo qual as ações são oferecidas. Caso
	 * o preço seja nulo, considera-se o preço de mercado.
	 */
	public Double getPrice() {
		return price;
	}

	public void setPrice(Double price) {
		if(price.isNaN())
			throw new IllegalArgumentException();
		this.price = price;
	}

	/**
	 * Obtém a quantidade de ações oferecidas. O comprador
	 * não precisa comprar todas as ações oferecidas, mas
	 * o vendedor não pode vender mais do que possui.
	 * 
	 * @return
	 * A quantidade de ações oferecidas pelo vendedor.
	 */
	public Integer getAmount() {
		return amount;
	}

	public void setAmount(Integer amount) {
		if(amount == null || amount < 0)
			throw new IllegalArgumentException();
		this.amount = amount;
	}

	/**
	 * Obtém o atual proprietário destas ações. 
	 * 
	 * @return
	 * O proprietário das ações oferecidas.
	 */
	public Investor getOwner() {
		return owner;
	}

	public void setOwner(Investor owner) {
		this.owner = owner;
	}
	
	/**
	 * Obtém o tipo da oferta, se é uma oferta de compra ou
	 * venda de ações.
	 * 
	 * @return
	 * Se a oferta é de compra ou venda.
	 */
	public Type getType() {
		return type;
	}

	public void setType(Type type) {
		if(type == null)
			throw new IllegalArgumentException();
		this.type = type;
	}


	
	/* == enumerações == */
	
	public static enum Type {
		PURCHASE, SELL
	}
	
	/* == teste == */
	
	@Override
	public String toString() {
		return String.format(
				"Offer [company=%s, price=%s, amount=%s, owner=%s, type=%s]",
				company, price, amount, owner, type);
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
		
		Offer offer = new Offer();
		offer.setOwner(acc);
		offer.setCompany(c);
		offer.setAmount(111);
		offer.setPrice(13.00);
		offer.setType(Type.PURCHASE);
		HibernateUtil.getSession().save(offer);
		System.out.println(offer);
	}
	
	private static void testLoad() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Offer.class).list();
		System.out.println(list);
	}
	
	private static void testRemove() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Offer.class).list();
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
