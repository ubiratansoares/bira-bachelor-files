package model;

import java.util.*;
import javax.persistence.*;

import model.Account.Privilege;
import util.HibernateUtil;

/**
 * Representa um empréstimo.
 */
@Entity
public class Loan extends Transaction {

	//o valor inicial é dado pelo valor da transação
	
	/* == atributos e construtores == */
	
	@Basic(optional=false)
	private Double interest;
	
	@Basic(optional=false)
	private Boolean discharged;
	
	@OneToMany(mappedBy="loan")
	private Set<Repayment> payments;

	/**
	 * Cria um empréstimo pendente sem juros.
	 */
	public Loan() {
		super();
		this.interest = 0.0;
		this.discharged = false;
		this.payments = new HashSet<Repayment>();
		this.setInverse(new Loan(this));
	}
	
	public Loan(Loan l) {
		super(l);
		this.payments = new HashSet<Repayment>();
		this.setInverse(l);
	}

	
	
	/* == setters e getters == */

	/**
	 * Obtém a taxa diária de juros.
	 * 
	 * @return
	 * A taxa diária de juros.
	 */
	public Double getInterest() {
		return interest;
	}

	public void setInterest(Double interest) {
		if(interest == null || interest.isNaN())
			throw new IllegalArgumentException();
		this.interest = interest;
		this.updateInverse();
	}

	/**
	 * Verifica se o empréstimo está pago.
	 * 
	 * @return
	 * {@code true} caso o empréstimo já esteja pago.
	 */
	public Boolean isDischarged() {
		return discharged;
	}

	public void setDischarged(boolean discharged) {
		this.discharged = discharged;
		this.updateInverse();
	}

	/**
	 * Obtém os pagamentos referentes a este empréstimo.
	 * 
	 * @return
	 * Todos os pagamentos efetuados para quitação deste
	 * empréstimo.
	 */
	public Set<Repayment> getPayments() {
		return payments;
	}
	
	@SuppressWarnings("unused")
	private void setPayments(Set<Repayment> payments) {
		this.payments = payments;
	}
	
	
	
	/* == outros métodos == */
	
	@Override
	protected void updateInverse() {
		Transaction t = this.getInverse();
		if(t == null) return;
		if(t instanceof Loan) {
			Loan l = (Loan) t;
			l.interest = this.interest;
			l.discharged = this.discharged;
		}
	}

	
	
	/* == teste == */
	
	@Override
	public String toString() {
		return String.format(
				"Loan [interest=%s, discharged=%s, toString()=%s]", interest,
				discharged, super.toString());
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
		
		Loan loan = new Loan();
		loan.setAccount(acc);
		loan.setValue(1000.00);
		loan.setInterest(b.getInterest());
		loan.getInverse().setAccount(b);
		HibernateUtil.getSession().save(loan);
		System.out.println(loan);
		System.out.println(loan.getInverse());
	}
	
	private static void testLoad() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Loan.class).list();
		System.out.println(list);
	}
	
	private static void testRemove() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Loan.class).list();
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
