package ecommerce.model;

import javax.persistence.*;

/**
 * Número de telefone de um usuário do sistema.
 * Cada usuário poderá registrar vários números.
 */
@Entity
@Table(
	uniqueConstraints=@UniqueConstraint(columnNames=
			{"owner_id", "country_code", "area_code", "number", "branch"})
)
public class PhoneNumber {
	
	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;	//conveniência
	
	@ManyToOne(optional=false)
	private Account owner;
	
	@Basic(optional=false)
	private Integer country_code;
	
	@Basic(optional=false)
	private Integer area_code;
	
	@Basic(optional=false)
	private Integer number;
	
	private Integer branch;
	
	@Basic(optional=false)
	@Column(unique=true)
	private Integer index; //for priority sorting
	
	public PhoneNumber() {
	}
	
	/* == getters e setters == */
	
	/**
	 * Define a chave primária da entidade.
	 */
	private void setId(Integer id) {
		this.id = id;
	}
	
	public Integer getId() {
		return id;
	}
	
	/**
	 * Define o código internacional do número de telefone.
	 */
	public void setCountryCode(Integer code) {
		if(code == null || code < 0)
			throw new IllegalArgumentException();
		this.country_code = code;
	}
	
	public Integer getCountryCode() {
		return country_code;
	}
	
	/**
	 * Define o código de área do número de telefone.
	 */
	public void setAreaCode(Integer code) {
		if(code == null || code < 0)
			throw new IllegalArgumentException();
		this.area_code = code;
	}
	
	public Integer getAreaCode() {
		return area_code;
	}
	
	/**
	 * Define o número de telefone.
	 */
	public void setNumber(Integer number) {
		if(number == null || number < 0)
			throw new IllegalArgumentException();
		this.number = number;
	}
	
	public Integer getNumber() {
		return number;
	}
	
	/**
	 * Define o ramal do número de telefone.
	 */
	public void setBranch(Integer branch) {
		if(branch != null && branch < 0)
			throw new IllegalArgumentException();
		this.branch = branch;
	}
	
	public Integer getBranch() {
		return branch;
	}
	
	/**
	 * Define o índice de prioridade do número. Números com 
	 * índices baixos têm maior preferência.
	 */
	public void setPriorityIndex(Integer index) {
		if(index == null || index < 0)
			throw new IllegalArgumentException();
		this.index = index;
	}
	
	public Integer getPriorityIndex() {
		return index;
	}
	
	/**
	 * Define o proprietário do número de telefone. Uma linha
	 * poderá ter vários usuários associados, mas cada entrada
	 * será armazenada separadamente como uma nova linha.
	 */
	public void setOwner(Account acc) {
		this.owner = acc;
	}
	
	public Account getOwner() {
		return owner;
	}
	
}
