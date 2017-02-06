package ecommerce.controller;

import ecommerce.model.*;
import ecommerce.util.HibernateUtil;

import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

import org.hibernate.*;

public class AuthenticationService extends HttpServlet {
	
	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		try {
			request.setCharacterEncoding("UTF-8");
		} catch(Exception e) {
			e.printStackTrace();
		}
		
		HttpSession session = request.getSession();
		
		String dest = "/index.jsp";
		if("login".equals(request.getParameter("action"))) {
			Session hs = HibernateUtil.getSession();
			hs.beginTransaction();
			
			Account acc = (Account) hs.createQuery("from Account where " +
					"email = :email and password = :password")
					.setParameter("email", request.getParameter("username"))
					.setParameter("password", request.getParameter("password"))
					.uniqueResult();
			
			if(acc != null) {
				Class cl = Hibernate.getClass(acc); //bypass proxy
				acc.setLastAccessTime();
				session.setAttribute("uid", acc.getId());
				if(Customer.class.isAssignableFrom(cl)) {
					dest = "/customer/index.jsp";
				} else if(Retailer.class.isAssignableFrom(cl)) {
					dest = "/retailer/index.jsp";
				} else
					session.invalidate();
			}
				
			hs.getTransaction().commit();
		} else if("logout".equals(request.getParameter("action"))) {
			session.invalidate();
		}
		
		String path = this.getServletContext().getContextPath();
		response.sendRedirect(path + dest);
		return;
	}
	
	private static final long serialVersionUID = 1L;
}
