<%@ include file="auth.jsp" %>
<%@ page import="java.util.*, java.text.*" %>
<%	String action = request.getParameter("action");
	if(action == null) action = "list";
	
	Session hs = HibernateUtil.getSession();
	if(action.equals("list")) {
		List<ProductCategory> list = Collections.checkedList(
				hs.createQuery("from ProductCategory order by name")
				.list(), ProductCategory.class); %>
<h2>Categorias de Produtos</h2>

<p><a href="home.jsp?view=categories&action=create">[Criar nova categoria]</a></p>

<table class="list">
	<tr><th>Nome</th> <th>Produtos</th></tr>
	<%	for(ProductCategory c : list) { %>
	<tr>
		<td><a title="Ver Detalhes" href="home.jsp?view=categories&action=view&id=<%= c.getId() %>">
				<%= HtmlUtil.cdata(c.getName()) %></a></td>
		<td class="numeric"><%= HtmlUtil.cdata(c.getProducts().size()) %></td>
	</tr>
	<%	} %>
</table>
	<%
	
	
	
	} else if(action.equals("view")) { 
		Integer id = null; ProductCategory c = null;
		try {
			//usar load neste caso para testar usando exceção
			id = Integer.valueOf(request.getParameter("id"));
			c = (ProductCategory) hs.load(ProductCategory.class, id); 
		} catch(Exception e) {
			out.println("Nenhuma categoria selecionada.");
			return;
		} %>
		
		<h2>Categoria de Produto</h2>

	<form method="POST" action="category">
		<input type="hidden" name="action" value="delete">
		<input type="hidden" name="id" value="<%= c.getId() %>">
		
		<p><strong>Nome</strong>: <%= HtmlUtil.cdata(c.getName()) %></p>
		
		<p>
		<button type="button" onclick="location='home.jsp?' +
				'view=categories&action=list'">Voltar</button>
		<button type="button" onclick="location='home.jsp?' +
				'view=categories&action=edit&id=<%= c.getId() %>'">Editar</button>
		<button type="submit" onclick="return confirm('Deseja ' +
				'realmente excluir esta categoria?');">Excluir</button>
		</p>
	</form>



<%	} else if(action.equals("edit")) { 
		Integer id = null; ProductCategory c = null;
		try {
			//usar load neste caso para testar usando exceção
			id = Integer.valueOf(request.getParameter("id"));
			c = (ProductCategory) hs.load(ProductCategory.class, id); 
		} catch(Exception e) {
			out.println("Nenhuma categoria selecionada.");
			return;
		} %>

<h2>Editar Categoria</h2>

<form method="POST" action="category">
<input type="hidden" name="action" value="update">
<input type="hidden" name="id" value="<%= c.getId() %>">
<table>
<caption class="bottom">
	<button type="button" 
			onclick="location = 'home.jsp?view=categories&action=view' + 
			'&id=<%= request.getParameter("id")%>'">Voltar</button>
	<button type="submit">Salvar</button>
</caption>
<tr>
	<th><label for="name">Nome:</label></th>
	<td><input id="name" type="text" name="name" value="<%= c.getName() %>"></td>
</tr>
</table>
</form>

<%	} else if(action.equals("create")) { %>

<h2>Cadastrar Nova Categoria</h2>

<form method="POST" action="category">
<input type="hidden" name="action" value="save">
<table>
<caption class="bottom">
	<button type="button" 
			onclick="location = 'home.jsp?view=categories&action=list'">
		Voltar</button>
	<button type="submit">Salvar</button>
</caption>
<tr>
	<th><label for="name">Nome:</label></th>
	<td><input id="name" type="text" name="name"></td>
</tr>
</table>
</form>

<%	} %>
