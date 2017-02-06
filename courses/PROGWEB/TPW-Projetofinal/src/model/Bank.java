package model;

import javax.persistence.*;

import util.HibernateUtil;

/**
 * Representa um banco (simulado). A simulação possui
 * capital infinito disponível para emprestar, e uma
 * quantidade infinita de ações de todas as empresas
 * disponíveis na simulação. A taxa de juros será 
 * calculada em função da demanda instantânea por
 * empréstimos.
 */
@Entity
public class Bank extends Investor {

	//for simplicity, banks have an infinite amount 
	//of money and shares of any available business

	/* == atributos e construtores == */
	
	@Basic(optional=false)
	@Column(precision=7, scale=6)
	private Double strategy;
	
	@Basic(optional=false)
	@Column(precision=7, scale=6)
	private Double interest; //current
	
	@Basic(optional=false)
	@Column(precision=7, scale=6)
	private Double totalLoans; //current

	/**
	 * Cria um banco com estratégia conservadora (0,1) e
	 * taxa de juros de 0,1% ao dia.
	 */
	public Bank() {
		super();
		this.setBalance(Double.POSITIVE_INFINITY);
		this.strategy = 0.1;
		this.interest = 0.001;
		this.setTotalLoans(0.00);
	}

	
	
	/* == setters e getters == */
	
	/**
	 * Obtém a taxa diária de juros atual do banco.
	 * 
	 * @return
	 * A taxa de juros atual do banco.
	 */
	public Double getInterest() {
		return interest;
	}

	public void setInterest(Double interest) {
		if(interest == null || interest.isNaN())
			throw new IllegalArgumentException();
		this.interest = interest;
	}

	/**
	 * Obtém o parâmetro de agressividade na estratégia
	 * de ajuste dos juros.
	 * 
	 * @return
	 * A agressividade do banco com relação ao ajuste dos juros.
	 */
	public Double getStrategy() {
		return strategy;
	}
	
	public void setStrategy(Double strategy) {
		if(strategy == null || strategy.isNaN())
			throw new IllegalArgumentException();
		this.strategy = strategy;
	}


	
	/* == teste == */

	@Override
	public String toString() {
		return String.format("Bank [strategy=%s, interest=%s, toString()=%s]",
				strategy, interest, super.toString());
	}

	private static void testSave() {
		Bank b = new Bank();
		b.setName("Test Bank");
		b.setStrategy(0.3);
		b.setInterest(0.002);
		HibernateUtil.getSession().save(b);
		System.out.println(b);
	}
	
	private static void testLoad() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Bank.class).list();
		System.out.println(list);
	}
	
	private static void testRemove() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Bank.class).list();
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



	public void setTotalLoans(Double totalLoans) {
		
		if(totalLoans == null) throw new IllegalArgumentException();

		this.totalLoans = totalLoans;
	}



	public Double getTotalLoans() {
		return totalLoans;
	}
	
}
