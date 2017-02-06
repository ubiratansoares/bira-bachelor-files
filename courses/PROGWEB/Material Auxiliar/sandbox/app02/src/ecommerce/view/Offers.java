package ecommerce.view;

import ecommerce.template.*;

import java.net.URI;
import java.util.Date;
import java.util.LinkedList;
import java.util.List;
import java.util.Locale;
import java.util.Set;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

public class Offers extends PageFragment {
	
	/* == atributos e construtores == */
	
	private static final DateFormat VALIDITY_FORMAT = 
			new SimpleDateFormat("EEE, dd-MM-yyyy HH:mm zzz");
	
	private static final PageTemplate template = 
			new PageTemplate(Offers.class.getResource("Offers.html"));
	
	public Offers(Locale locale) {
		super(template, locale, "ecommerce.view.Offers");
		this.offers = new LinkedList<Entry>();
	}
	
	private String title;
	private List<Entry> offers;
	
	/* == construção da página == */
	
	protected void set() {
		this.setValue("title", title);
		for(Entry e : offers) {
			this.setTarget("image", e.image);
			this.setTarget("link", e.link);
			this.setValue("name", e.name);
			
			if(e.hasCounter()) {
				this.displayOptional("counter", true);
				this.setValue("counter_text", e.counter);
			} else
				this.displayOptional("counter", false);
			
			if(e.hasDiscount()) {
				this.setOption("price_tag", "special_offer");
				this.setValue("price", String.format("R$%.2f", e.price));
				double alt_price = e.price * (100 - e.discount) / 100;
				this.setValue("alt_price", String.format("R$%.2f", alt_price));
				this.setValue("discount", String.format("%d%%", e.discount));
				this.setValue("validity", VALIDITY_FORMAT.format(e.validity));
			} else {
				this.setOption("price_tag", "normal_offer");
				this.setValue("price", String.format("R$%.2f", e.price));
			}
			
			this.stepLoop("product");
		}
	}
	
	protected void set(String group) {
		this.set();
	}
	
	protected void set(Set<String> group) {
		this.set();
	}
	
	/* == setters == */
	
	public void setTitle(String title) {
		this.title = title;
	}
	
	public Entry addOffer(String name, URI link, Double price) {
		Entry e = new Entry(name, link, price);
		this.offers.add(e);
		return e;
	}
	
	/* == classes aninhadas == */
	
	public class Entry {
		private String name;
		private URI link;
		private Double price;
		
		private URI image;
		private String counter;
		private Integer discount;
		private Date validity;
		
		public Entry(String name, URI link, Double price) {
			this.name = name;
			this.link = link;
			this.price = price;
		}
		
		public void setImage(URI src) {
			this.image = src;
		}
		
		public void setCounter(String text) {
			this.counter = text;
		}
		
		public boolean hasCounter() {
			return (this.counter != null);
		}
		
		public void setDiscount(Integer value) {
			this.discount = value;
		}
		
		public boolean hasDiscount() {
			return (this.discount != null);
		}
		
		public void setValidity(Date time) {
			this.validity = time;
		}
		
	}
	
}
