<%@ include file="auth.jsp" %>
<%@ page import="java.util.*" %>
<ul id="menu">
<%	if(request.getParameter("signup") != null) { %>
	<%	String form = request.getParameter("form"); 
		if(form == null) form = "account"; %>
	<li><a <% if("account".equals(form)) { %>class="selected"<% } %> href="signup.jsp?form=account">Seu Acesso</a></li>
	<li><a <% if("person".equals(form)) { %>class="selected"<% } %> href="signup.jsp?form=person">Seu Cadastro</a></li>
	<li><a <% if("address".equals(form)) { %>class="selected"<% } %> href="signup.jsp?form=address">Entrega</a></li>
	<li><a <% if("commit".equals(form)) { %>class="selected"<% } %> href="signup.jsp?form=commit">Concluindo</a></li>
<%	} else if(request.getParameter("manage") != null) { %>
	<%	String form = request.getParameter("form"); 
		if(form == null) form = "view"; %>
	<li><a <% if("view".equals(form)) { %>class="selected"<% } %> href="manage.jsp?form=view">Visualizar</a></li>
	<li><a <% if("account".equals(form)) { %>class="selected"<% } %> href="manage.jsp?form=account">Seu Acesso</a></li>
	<li><a <% if("person".equals(form)) { %>class="selected"<% } %> href="manage.jsp?form=person">Seu Cadastro</a></li>
	<li><a <% if("address".equals(form)) { %>class="selected"<% } %> href="manage.jsp?form=address">Entrega</a></li>
	<li><a href="main.jsp">Voltar</a></li>
<%	} else { %>
	<%	if(session.getAttribute("uid") != null) { %>
	<li><a href="main.jsp?view=account">Minha Conta</a></li>
	<li><a href="main.jsp?view=purchase">Minhas Compras</a>
		<ul>
			<li><a href="main.jsp?view=purchase&filter=current">Em progresso</a></li>
			<li><a href="main.jsp?view=purchase&filter=history">Histórico</a></li>
		</ul>
	</li>
	<%	} %>
	<li><a href="main.jsp?view=offers&category=all">Ofertas</a>
		<%	List<ProductCategory> list = Collections.checkedList(HibernateUtil.getSession().createQuery(
					"from ProductCategory order by name").list(), ProductCategory.class); 
			if(!list.isEmpty()) { 
				Integer cid = null;
				try {
					String category = request.getParameter("category");
					cid = (category == null)? (Integer) session.getAttribute("list.category"): Integer.valueOf(category);
				} catch(Exception e) {
				} %>
		<ul>
			<%	for(ProductCategory c : list) { %>
			<li><a <% if(c.getId().equals(cid)) out.print("class='selected'"); 
					%> href="main.jsp?view=offers&category=<%= c.getId() %>"><%= HtmlUtil.cdata(c.getName()) %></a></li>
			<%	} %>
		</ul>
		<%	} %>
	</li>
<%	} %>
</ul>
