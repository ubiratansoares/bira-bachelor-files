package ecommerce.model;

import ecommerce.util.HibernateUtil;

import org.hibernate.Session;
import org.hibernate.annotations.Type;

import javax.persistence.*;

/**
 * Modela uma compra.
 */
@Entity
public class PurchaseItem {
	
	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;
	
	@ManyToOne(optional=false)
	private Purchase order;
	
	@ManyToOne(optional=false)
	private Product product;
	
	@Basic(optional=false)
	private Integer amount;
	
	@Basic(optional=false)
	@Column(precision=15, scale=2)
	private Double price; //at the time
	
	public PurchaseItem() {
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
	 * Define o preço do produto no momento da compra.
	 * Este preço inclui descontos para o produto.
	 */
	public void setPrice(Double value) {
		this.price = value;
	}
	
	public Double getPrice() {
		return price;
	}
	
	/**
	 * Define o pedido de compra que agrega este item.
	 */
	public void setPurchase(Purchase order) {
		this.order = order;
	}
	
	public Purchase getPurchase() {
		return order;
	}
	
	/**
	 * Define o produto comprado.
	 */
	public void setProduct(Product product) {
		this.product = product;
	}
	
	public Product getProduct() {
		return product;
	}
	
	/**
	 * Define a quantidade comprada, em unidades.
	 */
	public void setAmount(Integer units) {
		this.amount = units;
	}
	
	public Integer getAmount() {
		return amount;
	}
	
	/* == outros métodos == */
	
	@Override
	public String toString() {
		return String.format("{class:'%s',purchase:%s,product:%s}", 
				this.getClass().getName(), order, product);
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
