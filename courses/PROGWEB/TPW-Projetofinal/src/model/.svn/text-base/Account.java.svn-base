package model;

import java.io.File;
import javax.persistence.*;

import util.HibernateUtil;

/**
 * Usuário do sistema. Um usuário válido precisa ser autenticável.
 */
@Entity
public class Account extends Investor {

	/* == atributos e construtores == */
	
	@Basic(optional=false)
	@Column(unique=true, length=31)
	private String username;
	
	@Basic(optional=false)
	@Column(length=64)
	private String password;
	
	@Column(unique=true)
	private String photoPath;
	
	@Basic(optional=false)
	@Enumerated(EnumType.STRING)
	private Privilege privilege;
	
	/**
	 * Cria um usuário com saldo inicial, sem carteira.
	 */
	public Account() {
		super();
		this.setBalance(10000.00);
		this.privilege = Privilege.NORMAL;
	}

	
	
	/* == setters e getters == */

	/**
	 * Obtém o nome de usuário. Este nome identifica o usuário
	 * no sistema, e não deve ser usado para exibição.
	 * 
	 * @return
	 * O nome de usuário.
	 */
	public String getUsername() {
		return username;
	}
	
	public void setUsername(String username) {
		if(username == null || username.isEmpty())
			throw new IllegalArgumentException();
		this.username = username;
	}

	/**
	 * Obtém a senha do usuário. A senha pode já estar criptografada.
	 * 
	 * @return
	 * A senha do usuário, possivelmente criptografada.
	 */
	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		if(password == null || password.isEmpty())
			throw new IllegalArgumentException();
		this.password = password;
	}

	/**
	 * Obtém o arquivo com a foto do usuário.
	 * 
	 * @return
	 * O caminho do arquivo de imagem.
	 */
	public File getPhotoPath() {
		return new File(photoPath);
	}

	public void setPhotoPath(File photoPath) {
		this.photoPath = photoPath.getPath();
	}

	/**
	 * Obtém o privilégio do usuário dentro do sistema.
	 * 
	 * @return
	 * O privilégio do usuário dentro do sistema.
	 */
	public Privilege getPrivilege() {
		return privilege;
	}

	public void setPrivilege(Privilege privilege) {
		if(privilege == null)
			throw new IllegalArgumentException();
		this.privilege = privilege;
	}


	
	/* == outros métodos == */
	
	public static Account authenticate(String username, String password) {
		return (Account) HibernateUtil.getSession().createQuery(
				"from Account where username = :username and password = :password")
				.setParameter("username", username)
				.setParameter("password", password)
				.uniqueResult();
	}
	
	/* == enumerações == */

	/**
	 * Privilégios de usuários.
	 */
	public static enum Privilege {
		/** Pode apenas gerenciar sua própria conta e participar da simulação. */ 
		NORMAL, 
		
		/** Pode configurar a simulação e gerenciar usuários comuns. */
		ADMIN, 
		
		/**
		 * Pode gerenciar administradores do sistema. Não será possível 
		 * criar ou excluir este tipo de usuário (deve haver exatamente
		 * um único usuário com este privilégio).
		 */
		OWNER
	}

	
	
	/* == teste == */
	
	@Override
	public String toString() {
		return String.format(
				"Account [username=%s, privilege=%s, toString()=%s]", username,
				privilege, super.toString());
	}

	private static void testSave() {
		Account acc = new Account();
		acc.setName("Test User");
		acc.setUsername("test");
		acc.setPassword("test");
		acc.setBalance(100.00);
		acc.setPrivilege(Privilege.NORMAL);
		HibernateUtil.getSession().save(acc);
		System.out.println(acc);
	}
	
	private static void testLoad() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Account.class).list();
		System.out.println(list);
	}
	
	private static void testRemove() {
		@SuppressWarnings("rawtypes")
		java.util.List list = HibernateUtil.getSession()
				.createCriteria(Account.class).list();
		HibernateUtil.getSession().delete(list.get(0));
		
	}
	
	public static void main(String[] args) {
		try {
			HibernateUtil.getSession().beginTransaction();
			testSave();
			testLoad();
			testRemove();
			testLoad();
			HibernateUtil.getSession().getTransaction().commit();
		} catch(Exception e) {
			HibernateUtil.getSession().getTransaction().rollback();
			e.printStackTrace();
		}
	}
	
}
