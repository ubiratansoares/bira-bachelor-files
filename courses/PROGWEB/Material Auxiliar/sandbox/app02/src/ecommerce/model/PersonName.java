package ecommerce.model;

import javax.persistence.*;

/**
 * Nome próprio de um usuário. Cada usuário poderá 
 * cadastrar apenas um nome real e um apelido.
 */
@Embeddable
public class PersonName {
	
	/* == atributos e construtores == */
	
	@Basic(optional=false)
	@Column(length=31)
	private String first_name;
	
	@Basic
	private String middle_names;
	
	@Basic(optional=false)
	@Column(length=63)
	private String last_name;
	
	@Basic
	@Column(length=31)
	private String nickname;
	
	public PersonName() {
	}
	
	public PersonName(String firstName, String lastName) {
		this.first_name = firstName;
		this.last_name = lastName;
	}
	
	/* == getters e setters == */
	
	/**
	 * Define o primeiro nome. O primeiro nome (ou nome próprio) poderá 
	 * ser composto, como "João Paulo" ou "Jean-Baptise". Isso fica ao
	 * critério do usuário.
	 */
	public void setFirstName(String name) {
		this.first_name = name;
	}
	
	public String getFirstName() {
		return first_name;
	}
	
	/**
	 * Define o sobrenome. O sobrenome (ou nome de família) também 
	 * poderá ser composto da mesma maneira que o primeiro nome.
	 */
	public void setLastName(String name) {
		this.last_name = name;
	}
	
	public String getLastName() {
		return last_name;
	}
	
	/**
	 * Define os nomes do meio. Os nomes do meio poderão ser abreviados
	 * ou omitidos, e poderão representar uma parte de um sobrenome 
	 * extenso ou um nome próprio alternativo (típico de descendentes
	 * de famílias estrangeiras).
	 */
	public void setMiddleNames(String names) {
		this.middle_names = names;
	}
	
	public String getMiddleNames() {
		return middle_names;
	}
	
	/**
	 * Define um apelido. Note que este não é o apelido usado para
	 * se identificar no sistema, mas um nome alternativo pelo qual
	 * a pessoa pode ser identificada no mundo real (e frequentemente
	 * é o nome pelo qual a pessoa é melhor identificada).
	 */
	public void setNickname(String name) {
		this.nickname = name;
	}
	
	public String getNickname() {
		return nickname;
	}
	
}
