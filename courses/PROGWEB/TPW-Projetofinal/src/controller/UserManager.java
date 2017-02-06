package controller;

import java.io.IOException;
import java.net.URLEncoder;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import model.Account;

import org.hibernate.HibernateException;

import util.HibernateUtil;

public class UserManager extends HttpServlet {

	public void doPost(HttpServletRequest request, HttpServletResponse response) 
			throws IOException, ServletException {
		try {
			Result r = Result.NOP;
			String errorMessage = null;
			try {
				HibernateUtil.getSession().beginTransaction();
				
				String action = request.getParameter("action");
				if(action == null) action = "nop";
				
				if(action.equals("create"))
					r = this.save(request);
				
				HibernateUtil.getSession().getTransaction().commit();
			} catch(ControllerException e) {
				errorMessage = URLEncoder.encode(e.getMessage(), "UTF-8");
				r = (Result) e.getResult();
				HibernateUtil.getSession().getTransaction().rollback();
			} catch(HibernateException e) {
				errorMessage = URLEncoder.encode("Erro ao realizar operação.", "UTF-8");
				r = Result.NOP;
			} catch(Exception e) {
				HibernateUtil.getSession().getTransaction().rollback();
				throw e;
			}
			
			if(errorMessage != null) {
				errorMessage = String.format("%cerror=%s", 
						map.get(r).contains("?")? '&': '?',
						errorMessage);
			} else
				errorMessage = "";
			
			response.setCharacterEncoding("UTF-8");
			response.sendRedirect(this.getServletContext()
					.getContextPath() + map.get(r) + errorMessage);
		} catch(IOException e) {
			throw e;
		} catch(Exception e) {
			throw new ServletException(e);
		}
	}
	
	private Result save(HttpServletRequest request) throws ControllerException {
		try {
			Integer id = (Integer) request.getSession().getAttribute("uid");
			Account acc = new Account();
			acc.setUsername(request.getParameter("username"));
			acc.setPassword(request.getParameter("password"));
			acc.setName(request.getParameter("name"));
			if(id == null) {
				//novo usuário se cadastrando
				acc.setPrivilege(Account.Privilege.NORMAL);
			} else {
				Account a = (Account) HibernateUtil.getSession().get(Account.class, id);
				if(a.getPrivilege().equals(Account.Privilege.ADMIN) || 
						a.getPrivilege().equals(Account.Privilege.OWNER)) {
					acc.setPrivilege(Account.Privilege.ADMIN);
				} else
					throw new ControllerException(Result.CREATE_NOK, "Acesso negado.");
			}
			if(!acc.getPassword().equals(request.getParameter("retype")))
				throw new ControllerException(Result.CREATE_NOK, "Senha não confirmada.");
			HibernateUtil.getSession().save(acc);
			return Result.CREATE_OK_USER;
		} catch(IllegalArgumentException e) {
			throw new ControllerException(Result.CREATE_NOK, "Dados inválidos ou insuficientes.");
		}
	}
	
	private static final Map<Result, String> map = new HashMap<Result, String>();
	
	static {
		map.put(Result.NOP, "/index.jsp");
		map.put(Result.CREATE_NOK, "/index.jsp?page=user&action=create");
		map.put(Result.CREATE_OK_USER, "/index.jsp?info=Seu+cadastro+foi+efetuado.");
		map.put(Result.CREATE_OK_ADMIN, "/index.jsp?page=user&action=list&info=Administrador+cadastrado.");
	}
	
	private static final long serialVersionUID = 1L;

	private enum Result {
		NOP, CREATE_NOK, CREATE_OK_USER, CREATE_OK_ADMIN
	}
}
