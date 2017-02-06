package rtk.view;

import rtk.template.*;

import java.net.*;
import java.util.*;

public class Dummy extends PageFragment {
	
	private static final PageTemplate template = 
			new PageTemplate(Dummy.class.getResource("Dummy.html"));

	public Dummy(URI base) {
		super(template);
		this.setBase(base);
		
		this.status = "null";
		this.email = "anonymous";
	}
	
	private String status;
	private String email;
	
	public void setStatus(String status) {
		this.status = String.valueOf(status);
	}
	
	public void setEmail(String email) {
		this.email = String.valueOf(email);
	}
	
	public boolean isAllowed() {
		return false;
	}
	
	public boolean isAllowed(String group) {
		return "user".equals(group);
	}
	
	public boolean isAllowed(Set<String> groups) {
		return groups.contains("user");
	}
	
	protected void set(String group) {
		if("ok".equals(this.status)) {
			this.setOption("events", "login_ok");
			this.setValue("email", this.email);
		} else {
			this.setOption("events", "login_nok");
		}
		
		this.setTarget("back", this.getBase().resolve("login"));
	}
	
	protected void set(Set<String> groups) {
		this.set("user");
	}
	
}
