package model;

import java.util.*;
import javax.persistence.*;

/**
 * Representa um investidor no simulador. Um investidor pode
 * participar de transações e possuir uma carteira de ações.
 */
@Entity
@Inheritance(strategy=InheritanceType.JOINED)
public class Investor {

	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;
	
	@Basic(optional=false)
	private String name;
	
	@Basic(optional=false)
	@Column(precision=15, scale=2)
	private Double balance;
	
	@OneToMany(mappedBy="account", cascade=CascadeType.ALL, orphanRemoval=true)
	private Set<Transaction> transactions;
	
	@OneToMany(mappedBy="owner")
	private Set<Stock> portfolio;

	/**
	 * Cria um investidor sem saldo, transações ou ações.
	 */
	public Investor() {
		this.transactions = new HashSet<Transaction>();
		this.portfolio = new HashSet<Stock>();
		this.balance = 0.00;
	}
	
	
	
	/* == setters e getters == */

	/**
	 * Obtém o identificador do sistema. Esse identificador
	 * é gerado automaticamente pela base de dados.
	 * 
	 * @return
	 * O identificador do sistema.
	 */
	public Integer getId() {
		return id;
	}
	
	@SuppressWarnings("unused")
	private void setId(Integer id) {
		this.id = id;
	}
	
	/**
	 * Obtém o nome de exibição do investidor.
	 * Não é necessariamente único.
	 * 
	 * @return
	 * O nome de exibição do investidor.
	 */
	public String getName() {
		return name;
	}

	public void setName(String name) {
		if(name == null || name.isEmpty())
			throw new IllegalArgumentException();
		this.name = name;
	}

	/**
	 * Obtém o saldo do investidor. É possível que
	 * alguns investidores possuam saldo infinito
	 * (como bancos) ou negativo (indicando dívida).
	 * 
	 * @return
	 * O saldo bancário do investidor.
	 */
	public Double getBalance() {
		return balance;
	}

	public void setBalance(Double balance) {
		if(balance == null || balance.isNaN())
			throw new IllegalArgumentException();
		this.balance = balance;
	}

	/**
	 * Obtém todas as transações efetuadas pelo investidor.
	 * 
	 * @return
	 * As transações efetuadas pelo investidor.
	 */
	public Set<Transaction> getTransactions() {
		return transactions;
	}

	@SuppressWarnings("unused")
	private void setTransactions(Set<Transaction> transactions) {
		this.transactions = transactions;
	}

	/**
	 * Obtém a carteira de investimentos do investidor. 
	 * 
	 * @return
	 * Todas as ações que este investidor possui.
	 */
	public Set<Stock> getPortfolio() {
		return portfolio;
	}

	@SuppressWarnings("unused")
	private void setPortfolio(Set<Stock> portfolio) {
		this.portfolio = portfolio;
	}

	
	
	/* == teste == */

	@Override
	public String toString() {
		return String.format("Investor [id=%s, name=%s, balance=%s]", id, name,
				balance);
	}

}
