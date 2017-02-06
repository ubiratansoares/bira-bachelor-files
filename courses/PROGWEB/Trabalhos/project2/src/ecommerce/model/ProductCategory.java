package ecommerce.model;

import ecommerce.util.HibernateUtil;

import org.hibernate.Session;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Locale;
import java.util.Map;

import javax.persistence.*;

/**
 * Modela uma categoria de produto.
 */
@Entity
public class ProductCategory {
	
	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;
	
	@Basic(optional=false)
	@Column(unique=true)
	private String name;
	
	@OneToMany(mappedBy="base", cascade=CascadeType.ALL, orphanRemoval=true)
	@MapKey(name="locale")
	private Map<Locale, LocalizedProductCategory> alt_names;
	
	@ManyToMany(mappedBy="categories")
	private List<Product> products;
	
	public ProductCategory() {
		this.products = new LinkedList<Product>();
		this.alt_names = new HashMap<Locale, LocalizedProductCategory>();
	}
	
	public ProductCategory(String name) {
		this.name = name;
		this.products = new LinkedList<Product>();
		this.alt_names = new HashMap<Locale, LocalizedProductCategory>();
	}
	
	/* == getters e setters == */
	
	/**
	 * Define o identificador da categoria.
	 */
	private void setId(Integer id) {
		this.id = id;
	}
	
	public Integer getId() {
		return id;
	}
	
	/**
	 * Define o nome da categoria. Este nome será o nome padrão (isto
	 * é, na linguagem padrão do sistema), e todas as traduções serão
	 * baseadas neste nome.
	 */
	public void setName(String name) {
		if(name == null || name.trim().isEmpty())
			throw new IllegalArgumentException();
		this.name = name;
	}
	
	public String getName() {
		return name;
	}

	/**
	 * Define os nomes traduzidos da categoria.
	 */
	private void setAlternativeNames(
			Map<Locale, LocalizedProductCategory> names) {
		this.alt_names = names;
	}
	
	public Map<Locale, LocalizedProductCategory> getAlternativeNames() {
		return alt_names;
	}

	/**
	 * Define os produtos pertencentes à categoria.
	 */
	private void setProducts(List<Product> products) {
		this.products = products;
	}
	
	public List<Product> getProducts() {
		return products;
	}

	/* == outros métodos == */
	
	@Override
	public String toString() {
		return name;
	}
	
	@Override
	public boolean equals(Object o) {
		if(o instanceof ProductCategory) {
			ProductCategory c = (ProductCategory) o;
			return (this.name != null) && this.name.equals(c.name);
		}
		return false;
	}
	
	@Override
	public int hashCode() {
		return (this.name == null)? 0: this.name.hashCode();
	}
	
}
