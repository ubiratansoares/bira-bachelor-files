<%@ page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %>
<%@ page import="ecommerce.util.HibernateUtil" %>
<%	HibernateUtil.getSession().beginTransaction();
	try { %>
	<%@ include file="auth.jsp" %>
	<%	String view = request.getParameter("view");
		if(view == null) view = "default";
		
		if(view.equals("account")) { %>
			<jsp:include page="/base.jsp">
				<jsp:param name="title" value="E-Commerce - Sua Conta"/>
				<jsp:param name="css_file" value="/retailer/home.css"/>
				<jsp:param name="css_file" value="/retailer/account.css"/>
				<jsp:param name="left_area" value="/retailer/menu.jsp"/>
				<jsp:param name="main_area" value="/retailer/account-form.jsp"/>
				<jsp:param name="right_area" value="/empty.jsp"/>
				<jsp:param name="form" value="view"/>
			</jsp:include>
	<%	} else if(view.equals("accounts")) { %>
			<jsp:include page="/base.jsp">
				<jsp:param name="title" value="E-Commerce - Contas de Usuários"/>
				<jsp:param name="css_file" value="/retailer/home.css"/>
				<jsp:param name="css_file" value="/retailer/account.css"/>
				<jsp:param name="left_area" value="/retailer/menu.jsp"/>
				<jsp:param name="main_area" value="/retailer/accounts.jsp"/>
				<jsp:param name="right_area" value="/empty.jsp"/>
			</jsp:include>
	<%	} else if(view.equals("products")) { %>
			<jsp:include page="/base.jsp">
				<jsp:param name="title" value="E-Commerce - Produtos"/>
				<jsp:param name="css_file" value="/retailer/home.css"/>
				<jsp:param name="css_file" value="/retailer/products.css"/>
				<jsp:param name="left_area" value="/retailer/menu.jsp"/>
				<jsp:param name="main_area" value="/retailer/products.jsp"/>
				<jsp:param name="right_area" value="/empty.jsp"/>
			</jsp:include>
	<%	} else if(view.equals("categories")) { %>
			<jsp:include page="/base.jsp">
				<jsp:param name="title" value="E-Commerce - Categorias de Produtos"/>
				<jsp:param name="css_file" value="/retailer/home.css"/>
				<jsp:param name="css_file" value="/retailer/retailer.css"/>
				<jsp:param name="left_area" value="/retailer/menu.jsp"/>
				<jsp:param name="main_area" value="/retailer/categories.jsp"/>
				<jsp:param name="right_area" value="/empty.jsp"/>
			</jsp:include>
	<%	} else { %>
			<jsp:include page="/base.jsp">
				<jsp:param name="title" value="E-Commerce - Administração"/>
				<jsp:param name="css_file" value="/retailer/home.css"/>
				<jsp:param name="left_area" value="/retailer/menu.jsp"/>
				<jsp:param name="main_area" value="/retailer/default.jsp"/>
				<jsp:param name="right_area" value="/empty.jsp"/>
			</jsp:include>
	<%	} 
	} finally {
		HibernateUtil.getSession().getTransaction().commit();
	} %>
