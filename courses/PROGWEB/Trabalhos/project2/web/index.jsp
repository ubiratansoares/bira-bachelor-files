<%@ page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %><%
%><%@ page import="ecommerce.util.HibernateUtil, ecommerce.model.*, org.hibernate.Hibernate" %><%
	String dest = "/customer/index.jsp";
	if(session.getAttribute("uid") != null) {
		HibernateUtil.getSession().beginTransaction();
		Account acc = (Account) HibernateUtil.getSession().get(
				Account.class, (Integer) session.getAttribute("uid"));
		
		if(acc != null) {
			//usuário autenticado
			//redirecionar para página correta
			Class cl = Hibernate.getClass(acc);
			if(Customer.class.isAssignableFrom(cl)) {
				dest = "/customer/index.jsp";
			} else if(Retailer.class.isAssignableFrom(cl)) {
				dest = "/retailer/index.jsp";
			} else 
				session.invalidate();
		} else
			session.invalidate();
		
		HibernateUtil.getSession().getTransaction().commit();
	}
	
	response.sendRedirect(this.getServletContext().getContextPath() + dest);
%>
