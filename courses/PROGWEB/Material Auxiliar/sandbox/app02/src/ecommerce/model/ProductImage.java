package ecommerce.model;

import ecommerce.util.HibernateUtil;

import org.hibernate.Session;
import org.hibernate.annotations.Type;

import java.io.File;
import java.util.Date;

import javax.persistence.*;

/**
 * Modela um produto a ser vendido.
 */
@Entity
@Table(
	uniqueConstraints=@UniqueConstraint(columnNames=
			{"product_id", "path"})
)
public class ProductImage {
	
	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;
	
	@Basic(optional=false)
	@Temporal(TemporalType.TIMESTAMP)
	@Column(updatable=false)
	private Date added;
	
	@Basic(optional=false)
	private String path;
	
	@Basic(optional=false)
	private String title;
	
	@Basic(optional=false)
	private String name; //alt name
	
	@ManyToOne
	private Product product;
	
	@ManyToOne
	private Account author;
	
	public ProductImage() {
		this.added = new Date();
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
	 * Define a data e a hora em que a imagem foi adicionada. 
	 */
	private void setTimestamp(Date time) {
		this.added = time;
	}
	
	public Date getTimestamp() {
		return added;
	}

	/**
	 * Define o nome alternativo da imagem. O nome alternativo é
	 * exibido quando a imagem não pode ser exibida por qualquer 
	 * razão.
	 */
	public void setAlternativeName(String name) {
		this.name = name;
	}
	
	public String getAlternativeName() {
		return name;
	}
	
	/**
	 * Define o título da imagem. O título da imagem será exibido
	 * próximo à imagem (como legenda), ou como dica (tooltip), a
	 * critério da interface.
	 */
	public void setTitle(String text) {
		this.title = text;
	}
	
	public String getTitle() {
		return title;
	}
	
	/**
	 * Define o caminho da imagem no sistema de arquivos.
	 */
	public void setPath(File file) {
		this.path = file.getPath();
	}
	
	public File getPath() {
		return new File(path);
	}
	
	/**
	 * Define o produto ao qual se refere a imagem.
	 */
	public void setProduct(Product p) {
		this.product = p;
	}
	
	public Product getProduct() {
		return product;
	}
	
	/**
	 * Define o usuário que disponibilizou a imagem.
	 */
	public void setAuthor(Account acc) {
		this.author = acc;
	}
	
	public Account getAuthor() {
		return author;
	}
	
	/* == outros métodos == */
	
	@Override
	public String toString() {
		return String.format("{class:'%s',id:'%d',name:'%s'}", 
				this.getClass().getName(), id, name);
	}
	
	/* == testes == */
	
	private static void testSave(Session session) {
		System.out.println("Testing save...");
	}
	
	private static void testLoad(Session session) {
		System.out.println("Testing load...");
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
