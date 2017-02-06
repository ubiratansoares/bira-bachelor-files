<%@ page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %>
<%@ page import="ecommerce.util.HibernateUtil" %>
<%	HibernateUtil.getSession().beginTransaction();
	try { %>
<%@ include file="auth-restricted.jsp" %>
<jsp:include page="/base.jsp">
	<jsp:param name="title" value="E-Commerce - Gerenciar conta"/>
	<jsp:param name="css_file" value="/customer/signup.css"/>
	<jsp:param name="left_area" value="/customer/menu.jsp"/>
	<jsp:param name="main_area" value="/customer/manage-form.jsp"/>
	<jsp:param name="right_area" value="/empty.jsp"/>
	<jsp:param name="manage" value="true"/>
</jsp:include>
<%	} finally {
		HibernateUtil.getSession().getTransaction().commit();
	} %>
