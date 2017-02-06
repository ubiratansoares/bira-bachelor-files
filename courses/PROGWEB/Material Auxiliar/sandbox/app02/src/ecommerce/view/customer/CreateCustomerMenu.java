package ecommerce.view.customer;

import ecommerce.template.*;

import java.net.URI;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

import static ecommerce.view.customer.AccountPage.View.*;

public class CreateCustomerMenu extends PageFragment {
	
	/* == atributos e construtores == */
	
	private static final PageTemplate template = 
			new PageTemplate(CreateCustomerMenu.class
			.getResource("CreateCustomerMenu.html"));
	
	public CreateCustomerMenu(Locale locale) {
		super(template, locale, "ecommerce.view.customer.CreateCustomerMenu");
	}
	
	private static final Map<String, AccountPage.View> map =
			new HashMap<String, AccountPage.View>();
	
	/* == construção da página == */
	
	protected void set() {
		try {
			this.setTarget("account_info", new URI("signup?page=account"));
			this.setTarget("personal_info", new URI("signup?page=person"));
			this.setTarget("phone_numbers", new URI("signup?page=phone"));
			this.setTarget("documents", new URI("signup?page=documents"));
			this.setTarget("address", new URI("signup?page=address"));
			this.setTarget("saving", new URI("signup?page=saving"));
		} catch(java.net.URISyntaxException e) {
			throw new RuntimeException(e);
		}
	}
	
	protected boolean isAllowed(String group) {
		return false;
	}
	
	protected boolean isAllowed(Set<String> group) {
		return false;
	}
	
	/* == outros métodos == */
	
	static {
		map.put("account", ACCOUNT_INFO_EDIT);
		map.put("person", PERSONAL_INFO_EDIT);
		map.put("phone", PHONE_NUMBERS_EDIT);
		map.put("documents", DOCUMENTS_EDIT);
		map.put("address", ADDRESS_EDIT);
		map.put("saving", SAVE_NEW_ACCOUNT);
	}
	
	public static AccountPage.View parseOption(String parameter) {
		return map.get(parameter);
	}
	
}
