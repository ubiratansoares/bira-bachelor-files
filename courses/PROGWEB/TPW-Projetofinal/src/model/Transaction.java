package model;

import java.util.Date;
import javax.persistence.*;

/**
 * Representa uma transação financeira. Transações financeiras
 * sempre são representadas por um par de movimentações: um 
 * crédito em uma conta, e um débito em outra conta.
 */
@Entity
@Inheritance(strategy=InheritanceType.JOINED)
public class Transaction {

	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;
	
	@ManyToOne(optional=false)
	private Investor account;
	
	@OneToOne(cascade=CascadeType.ALL, orphanRemoval=true)
	private Transaction inverse;
	
	@Basic(optional=false)
	@Column(precision=15, scale=2)
	private Double value;
	
	@Basic(optional=false)
	@Temporal(TemporalType.TIMESTAMP)
	private Date tstamp;

	/**
	 * Cria uma transação com a data e a hora atuais.
	 */
	public Transaction() {
		this.value = 0.00;
		this.tstamp = new Date();
		this.inverse = new Transaction(this);
		this.update_inverse();
	}
	
	
	protected Transaction(Transaction t) {
		this.inverse = t;
	}
	
	
	/* == setters e getters == */

	/**
	 * Obtém o identificador da transação. O identificador não
	 * deve ser copiado para o lado inverso, pois é a chave primária.
	 */
	public Integer getId() {
		return id;
	}

	@SuppressWarnings("unused")
	private void setId(Integer id) {
		this.id = id;
	}

	/**
	 * Obtém o investidor cuja conta foi alterada.
	 * 
	 * @return
	 * O investidor creditado ou debitado por esta transação.
	 */
	public Investor getAccount() {
		return account;
	}

	public void setAccount(Investor account) {
		if(account == null)
			throw new IllegalArgumentException();
		this.account = account;
	}

	/**
	 * Obtém a transação do lado inverso. Essa transação possui
	 * um identificador diferente, um valor oposto e a mesma
	 * data e hora.
	 * 
	 * @return
	 */
	public Transaction getInverse() {
		return inverse;
	}

	/**
	 * Define a transação do lado inverso. O valor, a data e a
	 * hora desta transação serão alterados de acordo com os 
	 * dados presentes naquela especificada.
	 * 
	 * @param inverse
	 * A transação do lado oposto.
	 */
	public void setInverse(Transaction inverse) {
		if(inverse == null)
			throw new IllegalArgumentException();
		this.inverse = inverse;
		inverse.inverse = this;
		inverse.update_inverse();
	}

	/**
	 * Obtém o valor transferido pela transação. Um valor negativo
	 * indica que fundos foram retirados da conta correspondente,
	 * e um valor positivo indica que fundos foram acrescidos à
	 * conta correspondente.
	 * 
	 * @return
	 * O valor transferido pela transação.
	 */
	public Double getValue() {
		return value;
	}

	/**
	 * Define o valor movimentado pela transação. Caso a transação
	 * tenha um lado inverso definido, ele também será atualizado.
	 * 
	 * @param value
	 */
	public void setValue(Double value) {
		if(value == null || value.isNaN())
			throw new IllegalArgumentException();
		this.value = value;
		this.update_inverse();
	}

	/**
	 * Obtém a data e a hora em que a transação foi efetuada.
	 * 
	 * @return
	 * A data e a hora em que a transação foi efetuada.
	 */
	public Date getTimestamp() {
		return tstamp;
	}

	/**
	 * Define a data e a hora da transação. Caso a transação
	 * tenha um lado inverso definido, ele também será atualizado.
	 * 
	 * @param tstamp
	 */
	public void setTimestamp(Date tstamp) {
		if(tstamp == null)
			throw new IllegalArgumentException();
		this.tstamp = tstamp;
		this.update_inverse();
	}

	
	
	/* == outros métodos == */

	private void update_inverse() {
		if(inverse == null) return;
		inverse.tstamp = this.tstamp;
		inverse.value = -this.value;
		this.updateInverse();
	}
	
	/**
	 * Atualiza a transação inversa com os dados desta transação.
	 * Este método é um template method, então é seguro sobrescrevê-lo.
	 */
	protected void updateInverse() {
		return;
	}

	
	
	/* == teste == */
	
	@Override
	public String toString() {
		return String
				.format("Transaction [id=%s, account=%s, value=%s, tstamp=%s]",
						id, account, value, tstamp);
	}
	
}
