<%@ page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %>
<%@ page import="ecommerce.util.HibernateUtil" %>
<%	HibernateUtil.getSession().beginTransaction();
	try { %>
<%@ include file="auth.jsp" %>
<%		String view = request.getParameter("view");
		if(view == null) view = "offers";
		if(view.equals("offers")) { %>
<jsp:include page="/base.jsp">
	<jsp:param name="title" value="E-Commerce - Ofertas"/>
	<jsp:param name="css_file" value="/customer/main.css"/>
	<jsp:param name="left_area" value="/customer/menu.jsp"/>
	<jsp:param name="main_area" value="/customer/offers.jsp"/>
	<jsp:param name="right_area" value="/customer/cart.jsp"/>
</jsp:include>
	<%	} else if(view.equals("account")) { %>
<jsp:include page="/base.jsp">
	<jsp:param name="title" value="E-Commerce - Minha Conta"/>
	<jsp:param name="css_file" value="/customer/main.css"/>
	<jsp:param name="css_file" value="/customer/signup.css"/>
	<jsp:param name="left_area" value="/customer/menu.jsp"/>
	<jsp:param name="main_area" value="/customer/manage-form.jsp"/>
	<jsp:param name="right_area" value="/customer/cart.jsp"/>
	<jsp:param name="main_page" value="true"/>
</jsp:include>
	<%	}
	} finally {
		HibernateUtil.getSession().getTransaction().commit();
	} %>
