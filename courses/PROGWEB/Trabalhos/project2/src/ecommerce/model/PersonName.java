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
		if(name == null || name.trim().isEmpty())
			throw new IllegalArgumentException();
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
		if(name == null || name.trim().isEmpty())
			throw new IllegalArgumentException();
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
		if(names != null && names.trim().isEmpty())
			names = null;
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
		if(name != null && name.trim().isEmpty())
			name = null;
		this.nickname = name;
	}
	
	public String getNickname() {
		return nickname;
	}
	
	/**
	 * Obtém o nome completo.
	 * 
	 * @param includeMiddleNames
	 * {@code true} inclui os nomes do meio.
	 * 
	 * @param inFull
	 * {@code false} abrevia os nomes do meio.
	 * 
	 * @param appendNickname
	 * {@code true} inclui o apelido no final.
	 * 
	 * @return 
	 * O nome completo, o que pode estar abreviado e pode
	 * incluir o apelido.
	 */
	public String getFullName(boolean includeMiddleNames, 
			boolean inFull, boolean appendNickname) {
		String full = (first_name == null)? "": first_name;
		if(includeMiddleNames && middle_names != null) {
			if(inFull) {
				full += " " + middle_names;
			} else {
				full += " " + middle_names.replaceAll("(\\w)\\w*", "$1");
			}
		}
		full += (last_name == null)? "": " " + last_name;
		if(appendNickname) {
			full += (nickname == null)? "": 
					String.format(" (\"%s\")", nickname);
		}
		return full.replaceAll("\\s+", " ").trim();
	}
	
	/**
	 * Obtém o nome completo, sem apelido e com os nomes do meio.
	 * 
	 * @param inFull
	 * {@code false} abrevia os nomes do meio.
	 * 
	 * @return 
	 * O nome completo, que pode estar abreviado.
	 */
	public String getFullName(boolean inFull) {
		return this.getFullName(true, inFull, false);
	}
	
	/**
	 * Obtém o nome completo apenas com o primeiro e último nomes.
	 * 
	 * @return 
	 * O nome completo, sem nomes do meio e sem apelido.
	 */
	public String getFullName() {
		return this.getFullName(false, false, false);
	}
	
}
