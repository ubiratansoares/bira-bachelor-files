package ecommerce.view;

import ecommerce.template.*;

import java.util.Locale;

public class Footer extends PageFragment {
	
	private static final PageTemplate template = 
			new PageTemplate(Footer.class.getResource("Footer.html"));
	
	public Footer(Locale locale) {
		super(template, locale, "ecommerce.view.Footer");
	}
	
}
