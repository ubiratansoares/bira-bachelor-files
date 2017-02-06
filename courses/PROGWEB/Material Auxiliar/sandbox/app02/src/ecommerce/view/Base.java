package ecommerce.view;

import ecommerce.template.*;

import java.util.Locale;

public class Base extends PageFragment {
	
	/* == atributos e construtores == */
	
	private static final PageTemplate template = 
			new PageTemplate(Base.class.getResource("Base.html"));
	
	public Base(Locale locale) {
		super(template, locale, "ecommerce.view.Base");
		this.header = new Header(locale);
		this.footer = new Footer(locale);
	}
	
	private Header header;
	private Footer footer;
	
	private PageFragment left;
	private PageFragment right;
	private PageFragment center;
	
	/* == construção da página == */
	
	protected void set() {
		this.setValue("header", header);
		this.setValue("footer", footer);
		this.setValue("left_area", left);
		this.setValue("right_area", right);
		this.setValue("main_area", center);
	}
	
	/* == setters e getters == */
	
	public Header getHeader() {
		return header;
	}
	
	public Footer getFooter() {
		return footer;
	}
	
	public void setLeftContent(PageFragment content) {
		this.left = content;
	}
	
	public void setRightContent(PageFragment content) {
		this.right = content;
	}
	
	public void setMainContent(PageFragment content) {
		this.center = content;
	}
	
}
