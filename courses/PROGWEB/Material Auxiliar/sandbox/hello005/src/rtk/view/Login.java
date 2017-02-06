package rtk.view;

import rtk.template.*;

import java.util.*;

public class Login extends PageFragment {
	
	private static final PageTemplate template = 
			new PageTemplate(Login.class.getResource("Login.html"));

	public Login() {
		super(template);
	}
	
}
