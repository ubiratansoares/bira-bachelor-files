package model;

import javax.persistence.*;

import model.Account.Privilege;
import util.HibernateUtil;

/**
 * Representa um pagamento de empréstimo. O total de todos
 * os pagamentos pode não corresponder ao valor inicial do
 * empréstimo, devido aos juros.
 */
@Entity
public class Repayment extends Transaction {

	/* == atributos e construtores == */
	
	@ManyToOne(optional=false)
	private Loan loan;

	/**
	 * Construtor vazio. 
	 */
	public Repayment() {
		super();
		this.setInverse(new Repayment(this));
	}
	
	protected Repayment(Repayment r) {
		super(r);
		this.setInverse(r);
	}

	/* == setters e getters == */
	
	/**
	 * Obtém o empréstimo quitado pelo pagamento.
	 * 
	 * @return
	 * O empréstimo ao qual se refere o pagamento.
	 */
	public Loan getLoan() {
		return loan;
	}

	public void setLoan(Loan loan) {
		if(loan == null)
			throw new IllegalArgumentException();
		this.loan = loan;
		this.updateInverse();
	}

	
	
	/* == outros métodos == */
	
	@Override
	protected void updateInverse() {
		Transaction t = this.getInverse();
		if(t == null) return;
		if(t instanceof Repayment) {
			Repayment r = (Repayment) t;
			if(this.loan == null) return;
			if(this.loan.getInverse() instanceof Loan)
				r.loan = (Loan) this.loan.getInverse();
		}
	}

	
	
	/* == teste == */
	
	@Override
	public String toString() {
		return String.format("Repayment [loan=%s, toString()=%s]", loan,
				super.toString());
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
		
		Repayment r = new Repayment();
		r.setAccount(acc);
		r.setValue(-100.00);
		r.setLoan(loan);
		r.getInverse().setAccount(b);
		HibernateUtil.getSession().save(r);
		System.out.println(r);
		System.out.println(r.getInverse());
	}
	
	private static void testLoad() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Repayment.class).list();
		System.out.println(list);
	}
	
	private static void testRemove() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Repayment.class).list();
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
