package controller;

import java.io.IOException;
import java.util.*;
import javax.servlet.*;
import javax.servlet.http.*;

import util.HibernateUtil;

import model.Account;

public class AuthenticationService extends HttpServlet {

	public void doPost(HttpServletRequest request, HttpServletResponse response) 
			throws ServletException, IOException {
		try {
			Result result = Result.NOP;
			String action = request.getParameter("action");
			if(action == null) action = "nop";
			
			try {
				HibernateUtil.getSession().beginTransaction();
				if(action.equals("login"))
					result = this.login(request);
				else if(action.equals("logout"))
					result = this.logout(request);
				HibernateUtil.getSession().getTransaction().commit();
			} catch(Exception e) {
				HibernateUtil.getSession().getTransaction().rollback();
				throw e;
			}
			
			response.sendRedirect(this.getServletContext()
					.getContextPath() + map.get(result));
		} catch(IOException e) {
			throw e;
		} catch(Exception e) {
			throw new ServletException(e);
		}
	}
	
	private Result login(HttpServletRequest request) {
		Account acc = Account.authenticate(
				request.getParameter("username"), 
				request.getParameter("password"));
		if(acc != null) {
			request.getSession().setAttribute("uid", acc.getId());
			switch(acc.getPrivilege()) {
			case NORMAL: return Result.LOGIN_USER;
			case ADMIN: return Result.LOGIN_ADMIN;
			case OWNER: return Result.LOGIN_OWNER;
			default: return Result.LOGIN_NOK;
			}
		} else {
			request.getSession().invalidate();
			return Result.LOGIN_NOK;
		}
	}
	
	private Result logout(HttpServletRequest request) {
		request.getSession().invalidate();
		return Result.LOGOUT;
	}
	
	/* == mapa de ações == */
	
	private static final Map<Result, String> map = new HashMap<Result, String>();

	static {
		map.put(Result.NOP, "/index.jsp");
		map.put(Result.LOGOUT, "/index.jsp");
		map.put(Result.LOGIN_NOK, "/index.jsp");
		map.put(Result.LOGIN_USER, "/index.jsp");
		map.put(Result.LOGIN_ADMIN, "/index.jsp");
		map.put(Result.LOGIN_OWNER, "/index.jsp");
	}
	
	private static final long serialVersionUID = 1L;

	private enum Result {
		NOP, LOGOUT, LOGIN_NOK, LOGIN_USER, LOGIN_ADMIN, LOGIN_OWNER
	}
	
}
