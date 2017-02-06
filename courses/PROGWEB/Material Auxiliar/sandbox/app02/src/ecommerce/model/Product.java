package ecommerce.model;

import ecommerce.util.HibernateUtil;

import org.hibernate.Session;
import org.hibernate.annotations.Type;

import java.io.File;
import java.util.Date;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Locale;
import java.util.Set;

import javax.persistence.*;

/**
 * Modela um produto a ser vendido.
 */
@Entity
public class Product {
	
	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;
	
	@Basic(optional=false)
	@Temporal(TemporalType.TIMESTAMP)
	private Date release_on;
	
	@Basic
	@Temporal(TemporalType.TIMESTAMP)
	private Date updated_on;
	
	@Enumerated(EnumType.STRING)
	@Column(nullable=false, length=31)
	private Status status;
	
	@ManyToMany
	private Set<ProductCategory> categories;
	
	@Basic(optional=false)
	private String name;
	
	@Basic(optional=false)
	private String brand;
	
	private String model;
	private String version;
	private String manufacturer;
	
	@Basic(optional=false)
	private String short_description;
	
	@Type(type="text")
	private String long_description;
	
	@OneToMany(mappedBy="product")
	private Set<ProductImage> images;
	
	@Column(precision=15, scale=2)
	private Double price; //base selling price
	
	public Product() {
		this.release_on = new Date();
		this.categories = new HashSet<ProductCategory>();
		this.images = new HashSet<ProductImage>();
	}
	
	/* == getters e setters == */
	
	/**
	 * Define o identificador do produto.
	 */
	private void setId(Integer id) {
		this.id = id;
	}
	
	public Integer getId() {
		return id;
	}
	
	/**
	 * Define a data e a hora em que o produto foi (ou será) lançado. 
	 * O sistema poderá ocultar o produto dos clientes até o momento
	 * do lançamento.
	 */
	private void setReleaseTime(Date time) {
		this.release_on = time;
	}
	
	public Date getReleaseTime() {
		return release_on;
	}

	/**
	 * Define a data e a hora da última alteração nos dados do produto. 
	 * O sistema poderá listar produtos para os vendedores de acordo 
	 * com esta data para facilitar a atualização dos dados do produto.
	 */
	public void setLastModificationTime(Date time) {
		this.updated_on = time;
	}
	
	/**
	 * Define a data e a hora da última alteração para o 
	 * instante atual.
	 */
	public void setLastModificationTime() {
		this.updated_on = new Date();
	}
	
	public Date getLastModificationTime() {
		return updated_on;
	}

	/**
	 * Define a situação do produto.
	 */
	public void setStatus(Status value) {
		this.status = value;
	}
	
	public Status getStatus() {
		return this.status;
	}
	
	/**
	 * Define o nome do produto. Este nome deve ser mais específico que
	 * uma categoria, mas mais genérico do que uma marca e um modelo.
	 * É permitido especificar o nome do modelo com sua respectiva marca,
	 * mas note que alguém procurando um "processador" pode não encontrar 
	 * uma "intel core 5i" desse modo.
	 */
	public void setName(String name) {
		this.name = name;
	}
	
	public String getName() {
		return name;
	}
	
	/**
	 * Define a marca do produto. A marca do produto é a marca da 
	 * companhia que distribui o produto, a qual pode não ser a mesma
	 * daquela que fabrica o produto.
	 */
	public void setBrand(String name) {
		this.brand = name;
	}
	
	public String getBrand() {
		return brand;
	}
	
	/**
	 * Define o modelo do produto. O modelo não precisa incluir a 
	 * versão, o ano ou a série do produto, pois há um campo específico
	 * para isso (mas isso fica a critério do usuário).
	 */
	public void setModel(String name) {
		this.model = name;
	}
	
	public String getModel() {
		return model;
	}
	
	/**
	 * Define a versão, série ou ano do produto. Alguns produtos 
	 * apresentam diferenças mesmo em uma mesma marca e modelo, 
	 * de acordo com a data de lançamento ou uma configuração
	 * alternativa.
	 */
	public void setVersion(String version) {
		this.version = version;
	}
	
	public String getVersion() {
		return version;
	}
	
	/**
	 * Define o nome do fabricante do produto. Alguns produtos podem
	 * ser distribuídos sob uma marca diferente da marca do fabricante.
	 */
	public void setManufacturer(String name) {
		this.manufacturer = name;
	}
	
	public String getManufacturer() {
		return manufacturer;
	}
	
	/**
	 * Define uma breve descrição para o produto. Esta será a descrição
	 * exibida nas listagens.
	 */
	public void setShortDescription(String desc) {
		this.short_description = desc;
	}
	
	public String getShortDescription() {
		return short_description;
	}
	
	/**
	 * Define uma descrição longa para o produto. Esta será a descrição
	 * exibida nos detalhes do produto.
	 */
	public void setLongDescription(String desc) {
		this.long_description = desc;
	}
	
	public String getLongDescription() {
		return long_description;
	}
	
	/**
	 * Define as imagens para o produto.
	 */
	private void setImages(Set<ProductImage> images) {
		this.images = images;
	}
	
	public Set<ProductImage> getImages() {
		return images;
	}
	
	/**
	 * Define um preço de base para o produto.
	 */
	public void setBasePrice(Double value) {
		this.price = value;
	}
	
	public Double getBasePrice() {
		return price;
	}
	
	/**
	 * Define as categorias do produto.
	 */
	private void setCategories(Set<ProductCategory> categories) {
		this.categories = categories;
	}
	
	public Set<ProductCategory> getCategories() {
		return categories;
	}
	
	/* == outros métodos == */
	
	@Override
	public String toString() {
		return String.format("{class:'%s',id:'%d',name:'%s'}", 
				this.getClass().getName(), id, name);
	}
	
	/* == classes aninhadas e enumerações == */
	
	/**
	 * Enumera as situações possíveis de um produto.
	 */
	public enum Status {
		
		/** 
		 * Produto novo, disponivel mas 
		 * ainda não lançado no mercado. 
		 */
		NOT_RELEASED,
		
		/** Produto ativo. */
		AVAILABLE,
		
		/** Produto fora de circulação. */
		UNAVAILABLE,
		
		/**
		 * Produto com problemas cadastrais, requer revisão do vendedor.
		 * Será útil ao aumentar o modelo do sistema, mas pode ser
		 * usado também ao se detectar algum erro. O sistema poderá 
		 * bloquear o acesso ao produto até que o problema seja 
		 * resolvido, nesse caso.
		 */
		PENDING_UPDATE
		
	}
	
	/* == testes == */
	
	private static void testSave(Session session) {
		System.out.println("Testing save...");
		Product p = new Product();
		p.setStatus(Status.AVAILABLE);
		p.setName("processor");
		p.setBrand("intel");
		p.setShortDescription("just some dummy processor");
		System.out.println(p);
		session.save(p);
	}
	
	private static void testLoad(Session session) {
		System.out.println("Testing load...");
		Product p = (Product) session.load(Product.class, 1);
		System.out.println(p);
	}
	
	public static void main(String[] args) {
		try {
			Session session = HibernateUtil.getSession();
			session.beginTransaction();
			testSave(session);
			testLoad(session);
			session.getTransaction().commit();
		} catch(ArrayIndexOutOfBoundsException e) {
			//swallow
		} catch(Exception e) {
			e.printStackTrace();
		} finally {
			HibernateUtil.getSessionFactory().close();
		}
	}
	
}
