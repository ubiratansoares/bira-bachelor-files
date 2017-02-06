<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ page import="util.*" %>
<%	try {
		request.setCharacterEncoding("UTF-8");
		HibernateUtil.getSession().beginTransaction(); 
		String p = request.getParameter("page"); 
		if(p == null) p = "default";
		if(p.equals("user")) { %>
<jsp:include page="base.jsp">
	<jsp:param name="page.title" value="TPW Invest - Criar nova conta"/>
	<jsp:param name="page.css" value="user.css"/>
	<jsp:param name="page.content" value="user.jsp"/>
</jsp:include>
	<%	} else if(p.equals("loan")) { %>
<jsp:include page="emprestimosEPagamentos.jsp"/>
	<%	} else { %>
<jsp:include page="base.jsp">
	<jsp:param name="page.title" value="TPW Invest"/>
	<jsp:param name="page.css" value="global.css"/>
	<jsp:param name="page.content" value="home.jsp"/>
</jsp:include>
	<%	}
		if(HibernateUtil.getSession().isOpen())
			HibernateUtil.getSession().getTransaction().commit();
	} catch(Exception e) {
		if(HibernateUtil.getSession().isOpen())
			HibernateUtil.getSession().getTransaction().rollback();
		throw e;
	} %>
