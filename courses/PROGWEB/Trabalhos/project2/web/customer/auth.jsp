<%@ page import="org.hibernate.*, ecommerce.util.*, ecommerce.model.*" %><% 
	if(!HibernateUtil.getSession().isOpen()) {
		//página acessada de forma irregular
		%><jsp:forward page="/index.jsp"/><%
	}
	
	if(session.getAttribute("uid") != null) {
		Account acc = (Account) HibernateUtil.getSession().get(
				Account.class, (Integer) session.getAttribute("uid"));
		Class cl = Hibernate.getClass(acc); //bypass proxy
		if(acc == null || (acc != null && !Customer.class.isAssignableFrom(cl))) {
			//usuário autenticado
			//redirecionar para página correta
			response.sendRedirect(this.getServletContext()
					.getContextPath() + "/index.jsp");
			return;
		}
	}
%>
