<%@ page import="org.hibernate.Session, ecommerce.util.*, ecommerce.model.*" %>
<%@ page import="java.util.*" %><% 
	if(!HibernateUtil.getSession().isOpen()) {
		//página acessada de forma irregular
		%><jsp:forward page="/index.jsp"/><%
	}
%>
