<%@ include file="auth.jsp" %>
<%@ page import="org.hibernate.*" %>
<%	ResourceBundle rbGlobal = HtmlUtil.getBundle("global", request);
	Account acc = null; Class acc_type = null;
	String path = this.getServletContext().getContextPath(); 
	Integer uid = (Integer) session.getAttribute("uid");
	if(uid != null) {
		acc = (Account) HibernateUtil.getSession()
				.get(Account.class, uid);
		if(acc == null) {
			session.invalidate();
			response.sendRedirect(path + "/index.jsp");
			return;
		} else
			acc_type = Hibernate.getClass(acc);
	} %>
<h1><a href="<%= path %>/index.jsp"><%= rbGlobal.getString("company-name") %></a></h1>
<form method="POST" action="<%= path %>/auth">
<div class="layout" id="top-menu">
	<span class="layout" id="top-nav">
		<a href="<%= path %>/index.jsp"><%= rbGlobal.getString("nav.home") %></a>
		
		<%	if(uid == null || Customer.class.isAssignableFrom(acc_type)) { %>
		<a href="<%= path %>/customer/main.jsp"><%= rbGlobal.getString("nav.offers") %></a>
		<%	} %>
		
		<%	if(request.getParameter("signup") == null && uid == null) { %>
		<a href="<%= path %>/customer/signup.jsp"><%= rbGlobal.getString("nav.signup") %></a>
		<%	} %>
		
		<a href="<%= path %>"><%= rbGlobal.getString("nav.help") %></a>
	</span>
	<span class="layout" id="session">
	<%	if(uid == null) { %>
		<input type="hidden" name="action" value="login">
		<label><%= rbGlobal.getString("session.username") %>: 
				<input type="text" name="username"></label>
		<label><%= rbGlobal.getString("session.password") %>: 
				<input type="password" name="password"></label>
		<button type="submit"><%= rbGlobal.getString("session.login") %></button>
	<%	} else if(acc != null) { %>
		<input type="hidden" name="action" value="logout">
		<%=	acc.getEmail() %>
		<button type="submit"><%= rbGlobal.getString("session.logout") %></button>
	<%	} %>
	</span>
</form>
</div>
