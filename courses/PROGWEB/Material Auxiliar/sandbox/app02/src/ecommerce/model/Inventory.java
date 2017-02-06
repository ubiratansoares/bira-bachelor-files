package ecommerce.model;

import ecommerce.util.HibernateUtil;

import org.hibernate.Session;
import org.hibernate.annotations.Type;

import java.util.Date;
import java.util.LinkedList;
import java.util.List;

import javax.persistence.*;

/**
 * Modela produto em estoque.
 */
@Entity
public class Inventory {
	
	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;
	
	@Basic(optional=false)
	@Temporal(TemporalType.TIMESTAMP)
	private Date admission_time;
	
	@Temporal(TemporalType.DATE)
	private Date validity_date;
	
	@Temporal(TemporalType.DATE)
	private Date start_date;
	
	@Temporal(TemporalType.DATE)
	private Date end_date;
	
	@Enumerated(EnumType.STRING)
	@Column(nullable=false, length=31)
	private Status status;
	
	@ManyToOne(optional=false)
	private Product product;
	
	@Basic(optional=false)
	private String batch_id; //or serial number
	
	@Basic(optional=false)
	private Integer batch_size;
	
	@Column(precision=15, scale=2)
	private Double batch_price; //supplier price
	
	@Type(type="text")
	private String notes;
	
	// TODO - associar fornecedor
	
	public Inventory() {
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
	 * Define a data e a hora em que o lote do produto foi
	 * adicionado ao estoque. 
	 */
	private void setAdmissionTime(Date time) {
		this.admission_time = time;
	}
	
	public Date getAdmissionTime() {
		return admission_time;
	}

	/**
	 * Define a data de vencimento do lote. Após esta data 
	 * os itens restantes do lote podem ser descartados.
	 */
	public void setValidityDate(Date date) {
		this.validity_date = date;
	}
	
	public Date getValidityDate() {
		return validity_date;
	}

	/**
	 * Define a data de envio do primeiro item do lote. Esta data pode
	 * ajudar a rastrear a distribuição de um lote, ao cruzar com as
	 * datas de envio dos pedidos de compras. Lotes defeituosos podem
	 * ser sofrer recall (a cargo dos respectivos fabricantes).
	 */
	public void setStartDate(Date date) {
		this.start_date = date;
	}
	
	public Date getStartDate() {
		return start_date;
	}

	/**
	 * Define a data de envio do último item do lote. Esta data pode
	 * ajudar a rastrear a distribuição de um lote, ao cruzar com as
	 * datas de envio dos pedidos de compras. Lotes defeituosos podem
	 * ser sofrer recall (a cargo dos respectivos fabricantes).
	 */
	public void setEndDate(Date date) {
		this.end_date = date;
	}
	
	public Date getEndDate() {
		return end_date;
	}

	/**
	 * Define a situação do lote do produto.
	 */
	public void setStatus(Status value) {
		this.status = value;
	}
	
	public Status getStatus() {
		return this.status;
	}
	
	/**
	 * Define o número de série do lote do produto.
	 */
	public void setBatchId(String id) {
		this.batch_id = id;
	}
	
	public String getBatchId() {
		return batch_id;
	}
	
	/**
	 * Define o tamanho do lote do produto, em unidades.
	 */
	public void setBatchSize(Integer size) {
		this.batch_size = size;
	}
	
	public Integer getBatchSize() {
		return batch_size;
	}
	
	/**
	 * Define o preço do lote do produto. Este é o preço
	 * pago pelo lote, não seu preço de venda.
	 */
	public void setBatchPrice(Double value) {
		this.batch_price = value;
	}
	
	public Double getBatchPrice() {
		return batch_price;
	}
	
	/**
	 * Define uma obervação ou anotação adicional 
	 * para o lote do produto.
	 */
	public void setNotes(String notes) {
		this.notes = notes;
	}
	
	public String getNotes() {
		return notes;
	}
	
	/**
	 * Define o produto relativo ao lote.
	 */
	public void setProduct(Product p) {
		this.product = p;
	}
	
	public Product getProduct() {
		return product;
	}
	
	/* == outros métodos == */
	
	@Override
	public String toString() {
		return String.format("{class:'%s',id:'%d',product:%s}", 
				this.getClass().getName(), id, product);
	}
	
	/* == classes aninhadas e enumerações == */
	
	/**
	 * Enumera as situações possíveis de um pedido de compra.
	 */
	public enum Status {
		
		/** Lote novo, nenhum item ainda vendido. */
		STOCKED,
		
		/** Lote ativo, alguns itens foram vendidos. */
		SELLING,
		
		/** Lote esgotado. */
		SOLD_OUT,
		
		/** 
		 * Lote descartado. Um lote pode ser descartado ao passar
		 * da validade, ou por qualquer outra razão. Mesmo que 
		 * alguns itens tenham sido vendidos, um lote é marcado 
		 * como descartado se o restante dos itens é descartado
		 * (mesmo que seja apenas um item).
		 */
		DISCARDED,
		
		/** 
		 * Lote recolhido ou sendo recolhido. Um lote pode ser 
		 * recolhido de volta a pedido do fabricante, por qualquer
		 * razão que não está no escopo do sistema.
		 */
		RECALL
		
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
