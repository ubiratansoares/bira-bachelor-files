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
		if(value == null || value < 0)
			throw new IllegalArgumentException();
		this.price = value;
	}
	
	public Double getPrice() {
		return price;
	}
	
	/**
	 * Define o pedido de compra que agrega este item.
	 */
	public void setPurchase(Purchase order) {
		if(order == null)
			throw new IllegalArgumentException();
		this.order = order;
	}
	
	public Purchase getPurchase() {
		return order;
	}
	
	/**
	 * Define o produto comprado.
	 */
	public void setProduct(Product product) {
		if(product == null)
			throw new IllegalArgumentException();
		this.product = product;
	}
	
	public Product getProduct() {
		return product;
	}
	
	/**
	 * Define a quantidade comprada, em unidades.
	 */
	public void setAmount(Integer units) {
		if(units == null || units < 0)
			throw new IllegalArgumentException();
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
		
		//buyer
		Customer customer = (Customer) HibernateUtil.getSession()
				.createQuery("from Customer where email = 'dummy@test.org'")
				.uniqueResult();
		if(customer == null)
			customer = new Customer();
		customer.setEmail("dummy@test.org");
		customer.setPassword("test");
		customer.getName().setFirstName("Test");
		customer.getName().setLastName("Dummy");
		customer.getAddress().setName("nowhere");
		customer.getAddress().setCity("nowhere");
		customer.getAddress().setCountry("nowhere");
		customer.getAddress().setPostalCode("nowhere");
		customer.setStatus(Account.Status.ACTIVE);
		customer.setBirthDate(new java.util.Date());
		customer.setLocale(java.util.Locale.ROOT);
		HibernateUtil.getSession().saveOrUpdate(customer);
		
		//compra
		Purchase purchase = new Purchase();
		purchase.setBuyer(customer);
		purchase.setDueDate(new java.util.Date());
		purchase.setStatus(Purchase.Status.ORDERED);
		purchase.setDeliveryAddress(customer.getAddress());
		HibernateUtil.getSession().saveOrUpdate(purchase);
		
		//produto
		Product product = new Product();
		product.setStatus(Product.Status.AVAILABLE);
		product.setName("something");
		product.setBrand("something");
		product.setShortDescription("something");
		HibernateUtil.getSession().saveOrUpdate(product);
		
		//item de compra
		PurchaseItem item = new PurchaseItem();
		item.setProduct(product);
		item.setPurchase(purchase);
		item.setPrice(1.00);
		item.setAmount(12);
		HibernateUtil.getSession().saveOrUpdate(item);
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
