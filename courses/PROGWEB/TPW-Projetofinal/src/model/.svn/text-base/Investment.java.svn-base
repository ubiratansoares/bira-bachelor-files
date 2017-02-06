package model;

import javax.persistence.*;

import model.Account.Privilege;
import util.HibernateUtil;

/**
 * Representa um investimento em ações. O investimento é
 * a aqusição de uma certa quantidade de ações a um certo
 * preço.
 */
@Entity
public class Investment extends Transaction {

	/* == atributos e construtores == */
	
	@ManyToOne(optional=false)
	private Quotation purchase; //share and price
	
	@Basic(optional=false)
	private Integer amount; //shares

	/**
	 * Construtor vazio.
	 */
	public Investment() {
		super();
		this.amount = 0;
		new Investment(this);
	}
	
	protected Investment(Investment i) {
		super(i);
		this.setInverse(i);
	}
	
	
	
	/* == setters e getters == */

	/**
	 * Obtém a cotação do instante da transação. A cotação 
	 * inclui o nome da empresa e o preço de cada ação.
	 * 
	 * @return
	 * A cotação usada na transação.
	 */
	public Quotation getPurchase() {
		return purchase;
	}

	public void setPurchase(Quotation purchase) {
		if(purchase == null)
			throw new IllegalArgumentException();
		this.purchase = purchase;
		this.updateInverse();
	}

	/**
	 * Obtém o número de ações negociadas na transação.
	 * 
	 * @return
	 * A quantidade de ações transferida.
	 */
	public Integer getAmount() {
		return amount;
	}

	public void setAmount(Integer amount) {
		if(amount == null)
			throw new IllegalArgumentException();
		this.amount = amount;
		this.updateInverse();
	}

	
	
	/* == outros métodos == */

	@Override
	protected void updateInverse() {
		Transaction t = this.getInverse();
		if(t == null) return;
		if(t instanceof Investment) {
			Investment i = (Investment) t;
			i.amount = -this.amount;
			i.purchase = this.purchase;
		}
	}
	
	
	
	/* == teste == */
	
	@Override
	public String toString() {
		return String.format(
				"Investment [purchase=%s, amount=%s, toString()=%s]", purchase,
				amount, super.toString());
	}
	
	private static void testSave() {
		Account acc = new Account();
		acc.setName("Test User");
		acc.setUsername("test");
		acc.setPassword("test");
		acc.setPrivilege(Privilege.NORMAL);
		HibernateUtil.getSession().save(acc);
		System.out.println(acc);
		
		Bank b = new Bank();
		b.setName("Test Bank");
		b.setStrategy(0.3);
		b.setInterest(0.002);
		HibernateUtil.getSession().save(b);
		System.out.println(b);
		
		Company c = new Company();
		c.setCode("TEST1");
		c.setName("Test Company");
		HibernateUtil.getSession().save(c);
		System.out.println(c);
		
		Quotation q = new Quotation();
		q.setCompany(c);
		q.setValue(10.00);
		HibernateUtil.getSession().save(q);
		System.out.println(q);
		
		Investment i = new Investment();
		i.setAccount(acc);
		i.setValue(-100.00);
		i.setPurchase(q);
		i.setAmount(10);
		i.getInverse().setAccount(b);
		HibernateUtil.getSession().save(i);
		System.out.println(i);
		System.out.println(i.getInverse());
	}
	
	private static void testLoad() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Investment.class).list();
		System.out.println(list);
	}
	
	private static void testRemove() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Investment.class).list();
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
