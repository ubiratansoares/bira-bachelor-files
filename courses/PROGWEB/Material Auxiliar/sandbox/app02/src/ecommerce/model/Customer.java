package ecommerce.model;

import javax.persistence.*;

/**
 * Modela um cliente. Um cliente pode fazer compras e gerenciar
 * apenas sua própria conta.
 */
@Entity
public class Customer extends Account {
	
	@Basic(optional=false)
	private Boolean tos; //terms of service
	
	public Customer() {
		this.tos = false;
	}
	
	/**
	 * Define se o cliente aceita os termos de serviço. O cliente deve
	 * aceitar os termos de serviço para criar uma conta, e para manter
	 * sua conta quando os termos forem atualizados.
	 */
	public void setAcceptsTOS(boolean value) {
		this.tos = value;
	}
	
	public Boolean acceptsTOS() {
		return tos;
	}
	
}
