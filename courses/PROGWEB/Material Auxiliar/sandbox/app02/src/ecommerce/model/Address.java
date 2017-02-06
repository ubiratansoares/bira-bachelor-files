package ecommerce.model;

import javax.persistence.*;

/**
 * Endereço de correspondência de um usuário do sistema.
 * Clientes receberão entregas neste endereço; demais usuários
 * poderão ser contatados pessoalmente ou receber documentos
 * neste endereço. Cada usuário poderá registrar apenas um
 * endereço.
 */
@Embeddable
public class Address {
	
	/* == atributos e construtores == */
	
	@Column(length=15)
	private String addr_type;
	
	@Column(nullable=false)
	private String addr_name;
	
	private Integer addr_number;
	
	private String addr_complement;
	
	@Column(length=63)
	private String district;
	
	@Column(nullable=false, length=63)
	private String city;
	
	@Column(length=63)
	private String state;
	
	@Column(nullable=false, length=63)
	private String country;
	
	@Column(nullable=false, length=12)
	private String postal_code;
	
	public Address() {
	}
	
	/* == setters e getters == */
	
	/**
	 * Define o tipo do local ao qual se refere o endereço.
	 * O tipo pode ser rua, avenida, rodovia, etc.
	 */
	public void setType(String type) {
		this.addr_type = type;
	}
	
	public String getType() {
		return addr_type;
	}
	
	/**
	 * Define o nome do local (rua, avenida, etc) ao qual
	 * se refere este endereço.
	 */
	public void setName(String name) {
		this.addr_name = name;
	}
	
	public String getName() {
		return addr_name;
	}
	
	/**
	 * Define o número do local ao qual se refere este endereço.
	 * O número pode estar em metros, quilômetros, etc dependendo
	 * do tipo de endereço.
	 */
	public void setNumber(Integer value) {
		this.addr_number = value;
	}
	
	public Integer getNumber() {
		return addr_number;
	}
	
	/**
	 * Define o complemento do local (apartamento, quadra, lote) 
	 * ao qual se refere este endereço.
	 */
	public void setComplement(String value) {
		this.addr_complement = value;
	}
	
	public String getComplement() {
		return addr_complement;
	}
	
	/**
	 * Define o bairro ou distrito do local 
	 * ao qual se refere este endereço.
	 */
	public void setDistrict(String name) {
		this.district = name;
	}
	
	public String getDistrict() {
		return district;
	}
	
	/**
	 * Define a cidade ou vila do local
	 * ao qual se refere este endereço.
	 */
	public void setCity(String name) {
		this.city = name;
	}
	
	public String getCity() {
		return city;
	}
	
	/**
	 * Define o estado ou província do local
	 * ao qual se refere este endereço.
	 */
	public void setState(String name) {
		this.state = name;
	}
	
	public String getState() {
		return state;
	}
	
	/**
	 * Define o país do local ao qual se refere este endereço.
	 */
	public void setCountry(String name) {
		this.country = name;
	}
	
	public String getCountry() {
		return country;
	}
	
	/**
	 * Define o código postal do local ao qual se refere este endereço.
	 */
	public void setPostalCode(String value) {
		this.postal_code = value;
	}
	
	public String getPostalCode() {
		return postal_code;
	}
	
}
