<%@ include file="auth.jsp" %>
<ul id="menu">
<%	String view = request.getParameter("view");
	if(view == null) view = "default"; %>
<%	if(request.getParameter("edit") != null) { %>
	<%	String form = request.getParameter("form"); 
		if(form == null) form = "view"; %>
	<li><a <% if("view".equals(form)) { %>class="selected"<% } %> href="account.jsp?form=view">Visualizar</a></li>
	<li><a <% if("account".equals(form)) { %>class="selected"<% } %> href="account.jsp?form=account">Informações de Acesso</a></li>
	<li><a <% if("person".equals(form)) { %>class="selected"<% } %> href="account.jsp?form=person">Informações Pessoais</a></li>
	<li><a <% if("address".equals(form)) { %>class="selected"<% } %> href="account.jsp?form=address">Residência</a></li>
	<li><a href="home.jsp?view=account">Voltar</a></li>
<%	} else { //default view %>
	<li><a href="home.jsp?view=account">Conta</a></li>
	<li><a href="home.jsp?view=accounts">Usuários</a>
		<ul>
			<li><a href="home.jsp?view=accounts&action=list&filter=retailers">Vendedores</a></li>
			<li><a href="home.jsp?view=accounts&action=list&filter=customers">Clientes</a></li>
		</ul>
	</li>
	<li><a href="#">Vendas</a></li>
	<li><a href="home.jsp?view=products&action=list">Produtos</a>
		<ul>
			<li><a href="#">Estoque</a></li>
			<li><a href="home.jsp?view=categories&action=list">Categorias</a></li>
		</ul>
	</li>
<%	} %>
</ul>
