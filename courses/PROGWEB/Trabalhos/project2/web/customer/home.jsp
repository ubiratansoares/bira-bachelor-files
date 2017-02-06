<%@ page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %>
<%@ page import="ecommerce.util.HibernateUtil" %>
<%	HibernateUtil.getSession().beginTransaction();
	try { %>
<%@ include file="auth.jsp" %>
<jsp:include page="/base.jsp">
	<jsp:param name="title" value="E-Commerce - Início"/>
	<jsp:param name="css_file" value="/customer/home.css"/>
	<jsp:param name="left_area" value="/customer/offers-short.jsp"/>
	<jsp:param name="main_area" value="/customer/offers-short.jsp"/>
	<jsp:param name="right_area" value="/customer/offers-short.jsp"/>
</jsp:include>
<%	} finally {
		HibernateUtil.getSession().getTransaction().commit();
	} %>
