package ecommerce.view;

import ecommerce.template.*;

import java.net.URI;
import java.util.Locale;

public class Header extends PageFragment {
	
	/* == atributos e construtores == */
	
	private static final PageTemplate template = 
			new PageTemplate(Header.class.getResource("Header.html"));
	
	public Header(Locale locale) {
		super(template, locale, "ecommerce.view.Header");
	}
	
	private boolean signup;
	
	/* == construção da página == */
	
	protected void set() {
		try {
			this.setTarget("home", new URI("index.html"));
			this.setTarget("signup", new URI("signup"));
		} catch(java.net.URISyntaxException e) {
			throw new RuntimeException(e);
		}
		this.displayOptional("signup_link", signup);
	}
	
	/* == setters == */
	
	public void displaySignUpLink(boolean display) {
		this.signup = display;
	}
	
}
