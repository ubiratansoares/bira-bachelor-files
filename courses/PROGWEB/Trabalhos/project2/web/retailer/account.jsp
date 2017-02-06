<%@ page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %>
<%@ page import="ecommerce.util.HibernateUtil" %>
<%	HibernateUtil.getSession().beginTransaction();
	try { %>
<%@ include file="auth.jsp" %>
<jsp:include page="/base.jsp">
	<jsp:param name="title" value="E-Commerce - Gerenciar sua conta"/>
	<jsp:param name="css_file" value="/retailer/account.css"/>
	<jsp:param name="left_area" value="/retailer/menu.jsp"/>
	<jsp:param name="main_area" value="/retailer/account-form.jsp"/>
	<jsp:param name="right_area" value="/empty.jsp"/>
	<jsp:param name="edit" value="true"/>
</jsp:include>
<%	} finally {
		HibernateUtil.getSession().getTransaction().commit();
	} %>
