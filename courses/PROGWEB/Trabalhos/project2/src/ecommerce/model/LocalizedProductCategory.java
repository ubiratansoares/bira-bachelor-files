package ecommerce.model;

import ecommerce.util.HibernateUtil;

import org.hibernate.Session;

import java.util.HashSet;
import java.util.Locale;
import java.util.Set;

import javax.persistence.*;

/**
 * Modela uma tradução de nome de categoria de produto.
 */
@Entity
public class LocalizedProductCategory {
	
	/* == atributos e construtores == */
	
	@Id @GeneratedValue
	private Integer id;
	
	@ManyToOne(optional=false)
	private ProductCategory base;
	
	@Basic(optional=false)
	private Locale locale;
		
	@Basic(optional=false)
	private String name;
	
	public LocalizedProductCategory() {
	}
	
	public LocalizedProductCategory(
			ProductCategory c, Locale l, String locName) {
		this.setBaseCategory(c);
		this.setLocalizedName(locName);
		this.setLocale(l);
	}
	
	/* == getters e setters == */
	
	/**
	 * Define a categoria básica da categoria traduzida. A categoria
	 * básica possui um nome no idioma padrão do sistema, e todas as
	 * suas traduções são associadas a ela.
	 */
	public void setBaseCategory(ProductCategory base) {
		if(base == null)
			throw new IllegalArgumentException();
		this.base = base;
	}
	
	public ProductCategory getBaseCategory() {
		return base;
	}
	
	/**
	 * Define a localização da tradução.
	 */
	public void setLocale(Locale l) {
		if(l == null)
			throw new IllegalArgumentException();
		this.locale = l;
	}

	public Locale getLocale() {
		return locale;
	}
	
	/**
	 * Define o nome traduzido da categoria.
	 */
	public void setLocalizedName(String name) {
		if(name == null || name.trim().isEmpty())
			throw new IllegalArgumentException();
		this.name = name;
	}
	
	public String getLocalizedName() {
		return name;
	}
	
	/* == outros métodos == */
	
	@Override
	public String toString() {
		return String.format("{class:'%s',category:%s,locale:'%s',locname:'%s'}", 
				this.getClass().getName(), base, locale, name);
	}
	
}
