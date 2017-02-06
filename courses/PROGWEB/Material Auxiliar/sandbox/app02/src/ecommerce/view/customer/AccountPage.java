package ecommerce.view.customer;

import ecommerce.template.*;
import ecommerce.model.Customer;

import java.net.URI;
import java.util.Locale;
import java.util.Set;

public class AccountPage extends PageFragment {
	
	/* == atributos e construtores == */
	
	private static final PageTemplate template = 
			new PageTemplate(AccountPage.class.getResource("AccountPage.html"));
	
	public AccountPage(Locale locale, Customer c, View view) {
		super(template, locale, "ecommerce.view.customer.AccountPage");
		this.view = view;
		this.customer = (c == null)? new Customer(): c;
	}
	
	private View view;
	private Customer customer;
	
	/* == construção da página == */
	
	protected void set() {
		String next = null;
		switch(view) {
			case ACCOUNT_INFO_EDIT: next = "person"; break;
			case PERSONAL_INFO_EDIT: next = "phone"; break;
			case PHONE_NUMBERS_EDIT: next = "documents"; break;
			case DOCUMENTS_EDIT: next = "address"; break;
			case ADDRESS_EDIT: next = "saving"; break;
			case SAVE_NEW_ACCOUNT: next = "saved"; break;
		}
		
		this.setOption("page_view", view.toString().toLowerCase());
		this.setOption("document_requirement", "id_optional");
		try {
			this.setTarget("submit", new URI("signup?page=" + next));
		} catch(java.net.URISyntaxException e) {
			throw new RuntimeException(e);
		}
		
		//account info
		this.setValue("email", customer.getEmail());
		this.setValue("password", customer.getPassword());
		this.displayOptional("tos", customer.acceptsTOS());
		
		//personal info
		this.setValue("fname", customer.getName().getFirstName());
		this.setValue("lname", customer.getName().getLastName());
		this.setValue("mnames", customer.getName().getMiddleNames());
		this.setValue("nname", customer.getName().getNickname());
//		this.setValue("bday", customer.getBirthDate());

		//phone numbers
		
		//delivery address
		this.setValue("type", customer.getAddress().getType());
		this.setValue("street", customer.getAddress().getName());
		String value = (customer.getAddress().getNumber() == null)? "":
				customer.getAddress().getNumber().toString();
		this.setValue("number", value);
		this.setValue("complement", customer.getAddress().getComplement());
		this.setValue("district", customer.getAddress().getDistrict());
		this.setValue("city", customer.getAddress().getCity());
		this.setValue("state", customer.getAddress().getState());
		this.setValue("country", customer.getAddress().getCountry());
		this.setValue("pcode", customer.getAddress().getPostalCode());
	}
	
	/**
	 * Retorna {@code false}. Apenas usuários não cadastrados podem se
	 * cadastrar. Administradores e vendedores não podem cadastrar 
	 * usuários (a não ser que saiam temporariamente do sistema. o
	 * que é intratável).
	 */
	protected boolean isAllowed(String group) {
		return false;
	}
	
	protected boolean isAllowed(Set<String> group) {
		return false;
	}
	
	/* == classes aninhadas e enumerações == */
	
	public static enum View {
		ACCOUNT_INFO_EDIT, PERSONAL_INFO_EDIT, PHONE_NUMBERS_EDIT,
		DOCUMENTS_EDIT, ADDRESS_EDIT, SAVE_NEW_ACCOUNT, NEW_ACCOUNT_SAVED
	}
	
}
