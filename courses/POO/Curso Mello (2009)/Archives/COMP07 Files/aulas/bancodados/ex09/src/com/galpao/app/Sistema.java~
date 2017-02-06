package com.galpao.app;

import java.util.*;

public class Sistema {
	public static void main(String args[]) throws Exception {
		
		//Locale locale = Locale.getDefault();
		Locale locale = new Locale("pt", "BR"); // pt_BR
		ResourceBundle msg = 
			ResourceBundle.getBundle("com.galpao.i18n.messages", locale);

		System.out.println(locale);
		System.out.println(msg.getString("INITIAL"));

		Pessoa p = new Pessoa("1", "nome", "rua",
				"numero", "complemento",
				"bairro", "cep");
		p.insert();

		p.setNome("Joao");
		p.update();

		p.remove();
	}
}
