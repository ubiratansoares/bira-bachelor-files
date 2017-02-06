package ecommerce.model;

import ecommerce.util.HibernateUtil;

import org.hibernate.Session;
import org.hibernate.annotations.Type;

import java.util.Date;
import java.util.LinkedList;
import java.util.List;

import javax.persistence.*;

/**
 * Modela uma compra.
 */
@Entity
public class Purchase {
	
	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;
	
	@Basic(optional=false)
	@Temporal(TemporalType.TIMESTAMP)
	@Column(updatable=false)
	private Date order_time;
	
	@Basic(optional=false)
	@Temporal(TemporalType.DATE)
	private Date due_date;
	
	@Basic
	@Temporal(TemporalType.DATE)
	private Date payment_date;
	
	@Basic
	@Temporal(TemporalType.DATE)
	private Date dispatch_date;
	
	@Enumerated(EnumType.STRING)
	@Column(nullable=false, length=31)
	private Status status;
	
	@ManyToOne(optional=false)
	private Customer buyer;
	
	@OneToMany(mappedBy="order", cascade=CascadeType.ALL, orphanRemoval=true)
	private List<PurchaseItem> items;
	
	@Basic(optional=false)
	@Column(precision=15, scale=2)
	private Double total; //including discounts
	
	@Embedded
	private Address address;
	
	@Type(type="text")
	private String notes;
	
	public Purchase() {
		this.order_time = new Date();
		this.total = 0.00;
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
	 * Define a data e a hora em que o pedido de compra foi lançado. 
	 */
	private void setOrderTime(Date time) {
		if(time == null)
			throw new IllegalArgumentException();
		this.order_time = time;
	}
	
	public Date getOrderTime() {
		return order_time;
	}

	/**
	 * Define a data de vencimento. Após esta data 
	 * o pedido pode ser multado ou cancelado.
	 */
	public void setDueDate(Date date) {
		if(date == null)
			throw new IllegalArgumentException();
		this.due_date = date;
	}
	
	public Date getDueDate() {
		return due_date;
	}

	/**
	 * Define a data de entrega. A data do pagamento é a data 
	 * em que o pagamento foi efetuado, conforme informado pelas 
	 * instituições bancárias.
	 */
	public void setPaymentDate(Date date) {
		this.payment_date = date;
	}
	
	public Date getPaymentDate() {
		return payment_date;
	}

	/**
	 * Define a data de envio do produto.
	 */
	public void setDispatchDate(Date date) {
		this.dispatch_date = date;
	}
	
	public Date getDispatchDate() {
		return dispatch_date;
	}

	/**
	 * Define a situação da compra.
	 */
	public void setStatus(Status value) {
		if(value == null)
			throw new IllegalArgumentException();
		this.status = value;
	}
	
	public Status getStatus() {
		return this.status;
	}
	
	/**
	 * Define o preço total do pedido. O preço 
	 * total deve incluir todos os descontos.
	 */
	public void setTotalPrice(Double value) {
		if(value == null || value < 0)
			throw new IllegalArgumentException();
		this.total = value;
	}
	
	public Double getTotalPrice() {
		return total;
	}
	
	/**
	 * Define uma obervação ou anotação adicional para a compra do
	 * produto.
	 */
	public void setNotes(String notes) {
		if(notes != null && notes.trim().isEmpty())
			notes = null;
		this.notes = notes;
	}
	
	public String getNotes() {
		return notes;
	}
	
	/**
	 * Define o comprador dos produtos.
	 */
	public void setBuyer(Customer c) {
		if(c == null)
			throw new IllegalArgumentException();
		this.buyer = c;
	}
	
	public Customer getBuyer() {
		return buyer;
	}
	
	/**
	 * Define o endereço de entrega dos produtos.
	 */
	public void setDeliveryAddress(Address addr) {
		if(addr == null)
			throw new IllegalArgumentException();
		this.address = addr;
	}
	
	public Address getDeliveryAddress() {
		return address;
	}
	
	/**
	 * Define os itens comprados.
	 */
	private void setItems(List<PurchaseItem> items) {
		this.items = items;
	}
	
	public List<PurchaseItem> getItems() {
		return items;
	}
	
	/* == outros métodos == */
	
	@Override
	public String toString() {
		return String.format("{class:'%s',id:'%d',buyer:%s}", 
				this.getClass().getName(), id, buyer);
	}
	
	/* == classes aninhadas e enumerações == */
	
	/**
	 * Enumera as situações possíveis de um pedido de compra.
	 */
	public enum Status {
		
		/** Pedido lançado, aguardando pagamento. */
		ORDERED,
		
		/** 
		 * Pedido cancelado. O pedido pode ser cancelado 
		 * ativamente pelo usuário antes da data de vencimento,
		 * ou automaticamente pelo sistema quando o pedido
		 * não é pago até o dia do vencimento.
		 */
		CANCELLED,
		
		/** O pedido foi pago, e os produtos estão aguardando envio. */
		DISPATCHING,
		
		/**
		 * Os produtos referentes ao pedido de compra foram enviados
		 * ao cliente. O recebimento dos produtos ou o extravio deles 
		 * não está no escopo do sistema.
		 */
		DISPATCHED
		
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
