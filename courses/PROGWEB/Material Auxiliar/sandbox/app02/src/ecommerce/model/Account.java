package ecommerce.model;

import java.util.Date;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Locale;
import java.util.Set;

import javax.persistence.*;

/**
 * Modela uma conta de usuário.
 */
@Entity
@Inheritance(strategy=InheritanceType.JOINED)
public abstract class Account {
	
	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;
	
	@Basic(optional=false)
	@Column(unique=true, length=254)
	private String email; //username
	
	@Basic(optional=false)
	@Column(length=12)
	private String password;
	
	@Basic(optional=false)
	@Temporal(TemporalType.TIMESTAMP)
	@Column(updatable=false)
	private Date created_on;
	
	@Basic
	@Temporal(TemporalType.TIMESTAMP)
	private Date last_update;
	
	@Basic
	@Temporal(TemporalType.TIMESTAMP)
	private Date last_access;
	
	@Embedded
	private PersonName name;
	
	@Embedded
	private Address address;
	
	@Enumerated(EnumType.STRING)
	@Column(nullable=false, length=31)
	private Status status;
	
	@Basic(optional=false)
	@Temporal(TemporalType.DATE)
	private Date birth_date; //--> age
	
	@OneToMany(mappedBy="owner", cascade=CascadeType.ALL, orphanRemoval=true)
	private Set<IdDocument> documents;
	
	@OneToMany(mappedBy="owner", cascade=CascadeType.ALL, orphanRemoval=true)
	private List<PhoneNumber> phoneNumbers;
	
	@Basic(optional=false)
	private Locale locale;
	
	protected Account() {
		this.created_on = new Date();
		this.name = new PersonName();
		this.address = new Address();
		this.documents = new HashSet<IdDocument>();
		this.phoneNumbers = new LinkedList<PhoneNumber>();
		this.locale = Locale.US; //default
	}
	
	/* == getters e setters == */
	
	/**
	 * Obtém o identificador do usuário. Usuários de todos os tipos
	 * compartilham o mesmo espaço de identificadores (gerados pelo
	 * sistema). Este é um identificador interno, usado pelo sistema
	 * para identificar os usuários de forma eficiente.
	 */
	public Integer getId() {
		return id;
	}
	
	private void setId(Integer id) {
		this.id = id;
	}
	
	/**
	 * Obtém o endereço de e-mail do usuário. O endereço de e-mail 
	 * é usado como identificador externo (isto é, será usado pela 
	 * interface com o usuário para identificá-lo). O espaço de nomes
	 * é compartilhado por todos os usuários, e ele deve ser único.
	 */
	public String getEmail(){
		return email;
	}
	
	public void setEmail(String email) {
		this.email = email;
	}
	
	/**
	 * Obtém a senha do usuário. Poderá estar criptografada.
	 */
	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		// TODO criptografar a senha (na aplicação)
		this.password = password;
	}
	
	/**
	 * Obtém a data e a hora em que a conta foi criada. O sistema
	 * poderá usar a informação para calcular o aniversário da conta.
	 */
	public Date getCreationTime() {
		return created_on;
	}

	private void setCreationTime(Date time) {
		this.created_on = time;
	}
	
	/**
	 * Obtém a data e a hora da última alteração na conta. O sistema
	 * poderá pedir ao usuário que revise seus dados cadastrais
	 * periodicamente.
	 */
	public Date getLastModificationTime() {
		return last_update;
	}

	/**
	 * Define a data e a hora da última alteração para o 
	 * instante atual.
	 */
	public void setLastModificationTime() {
		this.last_update = new Date();
	}
	
	public void setLastModificationTime(Date time) {
		this.last_update = time;
	}
	
	/**
	 * Obtém a data e a hora do último acesso da conta. O sistema
	 * poderá calcular o nível de atividade dos usuários (para
	 * saber quem está abandonando o produto, ou quem está acessando 
	 * com frequência).
	 */
	public Date getLastAccessTime() {
		return last_access;
	}

	/**
	 * Define a data e a hora do último acesso da conta 
	 * para o instante atual.
	 */
	public void setLastAccessTime() {
		this.last_access = new Date();
	}
	
	public void setLastAccessTime(Date time) {
		this.last_access = time;
	}
	
	/**
	 * Define o nome real do usuário.
	 */
	private void setName(PersonName name) {
		this.name = name;
	}
	
	public PersonName getName() {
		return name;
	}
	
	/**
	 * Define o endereço de contato.
	 */
	private void setAddress(Address a) {
		this.address = a;
	}
	
	public Address getAddress() {
		return this.address;
	}
	
	/**
	 * Define a situação da conta.
	 */
	public void setStatus(Status value) {
		this.status = value;
	}
	
	public Status getStatus() {
		return this.status;
	}
	
	/**
	 * Define a data de nascimento do usuário.
	 * É obrigatório, para poder verificar sua idade.
	 */
	public void setBirthDate(Date date) {
		this.birth_date = date;
	}
	
	public Date getBirthDate() {
		return this.birth_date;
	}
	
	/**
	 * Obtém os documentos associados ao usuário da conta.
	 * Alguns países não obrigam o porte de documentos de identificação,
	 * e muitos nem expedem qualquer documento (geralmente outros 
	 * documentos de entidades privadas são usados no lugar). Mesmo
	 * assim um usuário do sistema não é obrigado a fornecer 
	 * documentação, dependendo do país de origem.
	 */
	public Set<IdDocument> getDocuments() {
		return documents;
	}
	
	private void setDocuments(Set<IdDocument> docs) {
		this.documents = docs;
	}
	
	/**
	 * Obtém os números de telefone associados ao usuário da conta.
	 */
	public List<PhoneNumber> getPhoneNumbers() {
		return phoneNumbers;
	}
	
	private void setPhoneNumbers(List<PhoneNumber> numbers) {
		this.phoneNumbers = numbers;
	}
	
	/**
	 * Obtém a localização (idioma) preferida pelo usuário.
	 */
	public Locale getLocale() {
		return locale;
	}
	
	public void setLocale(Locale locale) {
		this.locale = locale;
	}
	
	/* == outros métodos == */
	
	@Override
	public String toString() {
		return String.format("{class:'%s',id:'%d',email:'%s'}", 
				this.getClass().getName(), id, email);
	}
	
	/* == classes aninhadas e enumerações == */
	
	/**
	 * Enumera as situações possíveis de uma conta.
	 */
	public enum Status {
		
		/** Conta nova, requer ativação ou aprovação. */
		PENDING_ACTIVATION,
		
		/** Conta ativa. */
		ACTIVE,
		
		/** 
		 * Conta sob suspeita de atividade ilegal ou má conduta. 
		 * A conta ficará sob vigilância administrativa, o que
		 * significa que relatórios mais detalhados podem ser 
		 * gerados para este usuário.
		 */
		UNDER_SURVEILLANCE,
		
		/** Conta suspensa. */
		SUSPENDED,
		
		/**
		 * Conta com erro detectado, requer revisão técnica ou 
		 * administrativa. O sistema poderá impedir seu acesso 
		 * temporariamente para evitar propagar erros.
		 */
		UNDER_MAINTENANCE,
		
		/**
		 * Conta com problemas cadastrais, requer revisão do usuário.
		 * Será útil ao aumentar o modelo do sistema, mas pode ser
		 * usado também ao se detectar alguma irregularidade.
		 */
		PENDING_UPDATE
		
	}
	
}
