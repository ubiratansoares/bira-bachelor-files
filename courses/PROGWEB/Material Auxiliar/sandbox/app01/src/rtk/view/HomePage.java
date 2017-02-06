package rtk.view;

import rtk.template.*;

import java.util.*;

public class HomePage extends PageFragment {
	
	private static final PageTemplate template = 
		new PageTemplate(HomePage.class.getResource("HomePage.html"));
	
	public HomePage(Locale locale) {
		super(template, locale);
	}
	
}
