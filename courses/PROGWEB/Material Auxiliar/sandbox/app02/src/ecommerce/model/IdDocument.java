package ecommerce.model;

import javax.persistence.*;

/**
 * Modela um documento de identificação. Como muitos países 
 * não possuem qualquer documento desse tipo, ele não deve 
 * ser necessário para todos os países.
 */
@Entity
@Table(
	uniqueConstraints=@UniqueConstraint(columnNames=
			{"country", "doctype", "identifier", "expediter"})
)
public class IdDocument {
	
	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;
	
	@ManyToOne(optional=false)
	private Account owner;
	
	@Basic(optional=false)
	@Column(length=63)
	private String country;
	
	@Basic(optional=false)
	private String doctype;
	
	@Basic(optional=false)
	private String identifier;
	
	@Basic(optional=false)
	private String expediter;
	
	public IdDocument() {
	}
	
	public IdDocument(Account owner, String country, String doctype, 
			String identifier, String expetiter) {
		this.owner = owner;
		this.country = country;
		this.doctype = doctype;
		this.identifier = identifier;
		this.expediter = expediter;
	}
	
	/* == getters e setters == */
	
	/**
	 * Define o identificador da instância do documento. Não é
	 * realmente utilizado, apenas facilita a modelagem.
	 */
	private void setId(Integer id) {
		this.id = id;
	}
	
	public Integer getId() {
		return id;
	}
	
	/**
	 * Define o portador do documento.
	 */
	public void setOwner(Account acc) {
		this.owner = acc;
	}
	
	public Account getOwner() {
		return owner;
	}
	
	/**
	 * Define o país que expediu o documento. Note que nem todos
	 * os (governos dos) países expedem documentos de identificação,
	 * e mesmo aqueles que expedem documentos podem não obrigar seu 
	 * porte. Algumas pessoas podem portar documentos de um país
	 * diferente daquele em que moram (podem portar passaportes para
	 * morar em país estrangeiro, por exemplo).
	 */
	public void setCountry(String name) {
		this.country = name;
	}
	
	public String getCountry() {
		return country;
	}
	
	/**
	 * Define o nome do documento. A língua do nome do documento
	 * não é especificada, mas é esperado que o sistema limite os
	 * documentos aceitos dependendo do país emissor.
	 */
	public void setType(String name) {
		this.doctype = name;
	}
	
	public String getType() {
		return doctype;
	}
	
	/**
	 * Define o identificador do documento. O identificador geralmente
	 * é um número de vários dígitos, mas é possível que alguns documentos
	 * também usem letras e outros símbolos separadores.
	 */
	public void setIdentifier(String value) {
		this.identifier = value;
	}
	
	public String getIdentifier() {
		return identifier;
	}
	
	/**
	 * Define o órgão expedidor do documento. Alguns documentos (como
	 * o RG brasileiro) podem não expedir um identificador único a 
	 * nível de país, mas certamente ele será único a nível de órgão
	 * expedidor.
	 */
	public void setExpediter(String name) {
		this.expediter = name;
	}
	
	public String getExpediter() {
		return expediter;
	}
	
}
