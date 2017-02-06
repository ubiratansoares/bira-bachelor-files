<%@ include file="auth.jsp" %>
<%@ page import="java.util.*, java.text.*" %>
<%	String action = request.getParameter("action");
	if(action == null) action = "list";
	
	Session hs = HibernateUtil.getSession();
	
	if(action.equals("list")) {
		List<Product> list = Collections.checkedList(
				hs.createQuery("from Product order by name, brand, model, version")
				.list(), Product.class); %>
<h2>Produtos</h2>

<p><a href="home.jsp?view=products&action=create">[Cadastrar produto]</a></p>

<table class="list">
	<tr>
		<th>Nome</th> <th>Marca</th> <th>Modelo</th> <th>Versão</th> 
		<th>Preço (R$)</th> <th>Atualizado</th> <th>Status</th>
	</tr>
	<%	for(Product p : list) { %>
<tr>
	<td><a title="Ver Detalhes" 
			href="home.jsp?view=products&action=view&id=<%= p.getId() %>">
			<%= HtmlUtil.cdata(p.getName()) %></a></td>
	<td><%= HtmlUtil.cdata(p.getBrand()) %></td>
	<td><%= HtmlUtil.cdata(p.getModel()) %></td>
	<td><%= HtmlUtil.cdata(p.getVersion()) %></td>
	<td class="numeric"><%= HtmlUtil.currency(
			p.getBasePrice(), "R$", request.getLocale()) %></td>
	<td><%= HtmlUtil.cdata(p.getLastModificationTime(), 
			"dd-MM-yyyy", request.getLocale()) %></td>
	<td><%= HtmlUtil.cdata(p.getStatus()) %></td>
</tr>
	<%	} %>
</table>
	<%
	
	
	
	} else if(action.equals("view")) { 
		Integer id = null; Product p = null;
		try {
			//usar load neste caso para testar usando exceção
			id = Integer.valueOf(request.getParameter("id"));
			p = (Product) hs.load(Product.class, id); 
		} catch(Exception e) {
			out.println("Nenhuma categoria selecionada.");
			return;
	} %>
<h2>Produto</h2>

<table class="view">
	<caption class="bottom">
		<form method="POST" action="product">
		<input type="hidden" name="action" value="delete">
		<input type="hidden" name="id" value="<%= p.getId() %>">
		<button type="button" onclick="location = 'home.jsp?' +
				'view=products&action=list'">Voltar</button>
		<button type="button" onclick="location = 'home.jsp?' +
				'view=products&action=edit&id=<%= p.getId() %>'">Editar</button>
		<button type="submit" onclick="
				return confirm('Realmente deseja excluir esta categoria?')">
				Excluir</button>
		</form>
	</caption>
	<tr><th>Nome:</th> <td><%= HtmlUtil.cdata(p.getName()) %></td></tr>
	<tr><th>Marca:</th> <td><%= HtmlUtil.cdata(p.getBrand()) %></td></tr>
	<tr><th>Modelo:</th> <td><%= HtmlUtil.cdata(p.getModel()) %></td></tr>
	<tr><th>Versão:</th> <td><%= HtmlUtil.cdata(p.getVersion()) %></td></tr>
	<tr><th>Fabricante:</th> <td><%= HtmlUtil.cdata(p.getManufacturer()) %></td></tr>
	<tr><th>Descrição:</th> <td><%= HtmlUtil.cdata(p.getShortDescription()) %></td></tr>
	<tr><th>Detalhes:</th> <td><%= HtmlUtil.cdata(p.getLongDescription()) %></td></tr>
	<tr><th>Lançamento:</th> <td><%= HtmlUtil.cdata(
			p.getReleaseTime(), "dd-MM-yyyy HH:mm", request.getLocale()) %></td></tr>
	<tr><th>Última Atualização:</th> <td><%= HtmlUtil.cdata(
			p.getLastModificationTime(), "dd-MM-yyyy HH:mm", request.getLocale()) %></td></tr>
	<tr><th>Status:</th> <td><%= HtmlUtil.cdata(p.getStatus()) %></td></tr>
	<tr><th>Preço:</th> <td><%= HtmlUtil.currency(
			p.getBasePrice(), "R$", request.getLocale()) %></td></tr>
	<%	Set<ProductCategory> set = p.getCategories();
		if(!set.isEmpty()) { 
			Iterator<ProductCategory> i = set.iterator();
			ProductCategory c = i.next(); %>
	<tr>
		<th rowspan="<%= set.size() %>">Categorias:</th> 
		<td><%= HtmlUtil.cdata(c.getName()) %></td> 
	</tr>
		<%	while(i.hasNext()) { 
				c = i.next(); %>
	<tr><td><%= HtmlUtil.cdata(c.getName()) %></td></tr>
	<%		}
		} %>
</table>
<%	} else if(action.equals("edit")) { 
		Integer id = null; Product p = null;
		try {
			//usar load neste caso para testar usando exceção
			id = Integer.valueOf(request.getParameter("id"));
			p = (Product) hs.load(Product.class, id); 
		} catch(Exception e) {
			out.println("Nenhum produto selecionado.");
			return;
		} %>
<h2>Produto</h2>

<form method="POST" action="product">
<table class="edit">
	<caption class="bottom">
		<input type="hidden" name="id" value="<%= p.getId() %>">
		<button type="button" onclick="location = 'home.jsp?' +
				'view=products&action=view&id=<%= p.getId() %>'">Voltar</button>
		<button type="submit" name="action" value="update">Salvar</button>
	</caption>
	<tr>
		<th>Nome:</th> 
		<td colspan="2"><input type="text" size="40" name="name" 
				value="<%= HtmlUtil.cdata(p.getName()) %>"></td>
	</tr>
	<tr>
		<th>Marca:</th> 
		<td colspan="2"><input type="text" name="brand" 
				value="<%= HtmlUtil.cdata(p.getBrand()) %>"></td>
	</tr>
	<tr>
		<th>Modelo:</th> 
		<td colspan="2"><input type="text" name="model" 
				value="<%= HtmlUtil.cdata(p.getModel()) %>"></td>
	</tr>
	<tr>
		<th>Versão:</th> 
		<td colspan="2"><input type="text" name="version" 
				value="<%= HtmlUtil.cdata(p.getVersion()) %>"></td>
	</tr>
	<tr>
		<th>Fabricante:</th> 
		<td colspan="2"><input type="text" name="manufacturer" 
				value="<%= HtmlUtil.cdata(p.getManufacturer()) %>"></td>
	</tr>
	<tr>
		<th>Descrição:</th> 
		<td colspan="2"><input type="text" size="60" name="sdesc"
			value="<%= HtmlUtil.cdata(p.getShortDescription()) %>"></td>
	</tr>
	<tr>
		<th style="vertical-align: top">Detalhes:</th> 
		<td colspan="2">
			<textarea cols="80" rows="6" name="ldesc"><%= HtmlUtil.cdata(p.getLongDescription()) %></textarea>
		</td>
	</tr>
	<tr>
		<th>Lançamento:</th> 
		<td colspan="2">
	<%	if(p.getReleaseTime() == null) { %>
		<input type="text" size="3" maxlength="2" name="day">
		de <select name="month">
		<%	String[] months = DateFormatSymbols.getInstance(request.getLocale()).getMonths();
			for(int i = 0; i < months.length; i++) { %>
			<option value="<%= i %>"><%= months[i] %></option>
		<%	} %>
		</select>
		de <input type="text" size="5" maxlength="4" name="year">
		
		<input type="text" size="3" maxlength="2" name="hour">:
		<input type="text" size="3" maxlength="2" name="minute">
	<% } else { 
			Calendar cal = Calendar.getInstance(request.getLocale());
			cal.setTime(p.getReleaseTime()); %>
		<input type="text" size="3" maxlength="2" name="day"
				value="<%= Integer.toString(cal.get(Calendar.DAY_OF_MONTH)) %>">
		de <select name="month">
		<%	String[] months = DateFormatSymbols.getInstance(request.getLocale()).getMonths();
			for(int i = 0; i < months.length; i++) { %>
			<option value="<%= (i + 1) %>" <% if(i == cal.get(Calendar.MONTH)) {
				%>selected<% } %>><%= months[i] %></option>
		<%	} %>
		</select>
		de <input type="text" size="5" maxlength="4" name="year"
				value="<%= Integer.toString(cal.get(Calendar.YEAR)) %>">
		
		<input type="text" size="3" maxlength="2" name="hour"
				value="<%= String.format("%02d", cal.get(Calendar.HOUR_OF_DAY)) %>">:
		<input type="text" size="3" maxlength="2" name="minute"
				value="<%= String.format("%02d", cal.get(Calendar.MINUTE)) %>">
	<%	} %>
		</td>
	</tr>
	<tr>
		<th>Preço:</th> 
		<td colspan="2">R$<input type="text" size="8" maxlength="16" name="price" 
				value="<%= HtmlUtil.cdata(p.getBasePrice(), 1, 2, 2, request.getLocale()) %>"></td>
	</tr>
	<tr>
		<th>Situação</th>
		<td>
			<select name="status">
			<%	for(Product.Status st : Product.Status.class.getEnumConstants()) { %>
				<option value="<%= st.ordinal() %>" <% 
					if(st.equals(p.getStatus())) out.print("selected"); 
				%>><%= st.toString().replace("_", " ") %></option>
			<%	} %>
			</select>
		</td>
	</tr>
	<%	List<ProductCategory> list = Collections.checkedList(
				hs.createQuery("from ProductCategory order by name").list(), 
				ProductCategory.class);
		if(!list.isEmpty()) { 
			Iterator<ProductCategory> i = list.iterator();
			ProductCategory c = i.next(); %>
	<tr>
		<th rowspan="<%= list.size() %>">Categorias:</th> 
		<td style="text-align: right"><%= HtmlUtil.cdata(c.getName()) %></td> 
		<td><input type="checkbox" name="category" value="<%= c.getId() %>"<%
			if(p.getCategories().contains(c)) out.print("checked"); %>></td>
	</tr>
		<%	while(i.hasNext()) { 
				c = i.next(); %>
	<tr>
		<td style="text-align: right"><%= HtmlUtil.cdata(c.getName()) %></td> 
		<td><input type="checkbox" name="category" value="<%= c.getId() %>"<%
			if(p.getCategories().contains(c)) out.print("checked"); %>></td>
	</tr>
	<%		}
		} %>
</table>
</form>
<%	} else if(action.equals("create")) { 
		Product p = (Product) session.getAttribute("product"); 
		if(p == null) p = new Product(); %>
<h2>Produto</h2>

<form method="POST" action="product">
<table class="edit">
	<caption class="bottom">
		<button type="button" onclick="location = 'home.jsp?' +
				'view=products&action=list'">Voltar</button>
		<button type="submit" name="action" value="save">Salvar</button>
	</caption>
	<tr>
		<th>Nome:</th> 
		<td colspan="2"><input type="text" size="40" name="name" 
				value="<%= HtmlUtil.cdata(p.getName()) %>"></td>
	</tr>
	<tr>
		<th>Marca:</th> 
		<td colspan="2"><input type="text" name="brand" 
				value="<%= HtmlUtil.cdata(p.getBrand()) %>"></td>
	</tr>
	<tr>
		<th>Modelo:</th> 
		<td colspan="2"><input type="text" name="model" 
				value="<%= HtmlUtil.cdata(p.getModel()) %>"></td>
	</tr>
	<tr>
		<th>Versão:</th> 
		<td colspan="2"><input type="text" name="version" 
				value="<%= HtmlUtil.cdata(p.getVersion()) %>"></td>
	</tr>
	<tr>
		<th>Fabricante:</th> 
		<td colspan="2"><input type="text" name="manufacturer" 
				value="<%= HtmlUtil.cdata(p.getManufacturer()) %>"></td>
	</tr>
	<tr>
		<th>Descrição:</th> 
		<td colspan="2"><input type="text" size="60" name="sdesc"
			value="<%= HtmlUtil.cdata(p.getShortDescription()) %>"></td>
	</tr>
	<tr>
		<th style="vertical-align: top">Detalhes:</th> 
		<td colspan="2">
			<textarea cols="80" rows="6" name="ldesc"><%= HtmlUtil.cdata(p.getLongDescription()) %></textarea>
		</td>
	</tr>
	<tr>
		<th>Lançamento:</th> 
		<td colspan="2">
	<%	if(p.getReleaseTime() == null) { %>
		<input type="text" size="3" maxlength="2" name="day">
		de <select name="month">
		<%	String[] months = DateFormatSymbols.getInstance(request.getLocale()).getMonths();
			for(int i = 0; i < months.length; i++) { %>
			<option value="<%= i %>"><%= months[i] %></option>
		<%	} %>
		</select>
		de <input type="text" size="5" maxlength="4" name="year">
		
		<input type="text" size="3" maxlength="2" name="hour">:
		<input type="text" size="3" maxlength="2" name="minute">
	<% } else { 
			Calendar cal = Calendar.getInstance(request.getLocale());
			cal.setTime(p.getReleaseTime()); %>
		<input type="text" size="3" maxlength="2" name="day"
				value="<%= Integer.toString(cal.get(Calendar.DAY_OF_MONTH)) %>">
		de <select name="month">
		<%	String[] months = DateFormatSymbols.getInstance(request.getLocale()).getMonths();
			for(int i = 0; i < months.length; i++) { %>
			<option value="<%= (i + 1) %>" <% if(i == cal.get(Calendar.MONTH)) {
				%>selected<% } %>><%= months[i] %></option>
		<%	} %>
		</select>
		de <input type="text" size="5" maxlength="4" name="year"
				value="<%= Integer.toString(cal.get(Calendar.YEAR)) %>">
		
		<input type="text" size="3" maxlength="2" name="hour"
				value="<%= String.format("%02d", cal.get(Calendar.HOUR_OF_DAY)) %>">:
		<input type="text" size="3" maxlength="2" name="minute"
				value="<%= String.format("%02d", cal.get(Calendar.MINUTE)) %>">
	<%	} %>
		</td>
	</tr>
	<tr>
		<th>Preço:</th> 
		<td colspan="2">R$<input type="text" size="8" maxlength="16" name="price" 
				value="<%= HtmlUtil.cdata(p.getBasePrice(), 1, 2, 2, request.getLocale()) %>"></td>
	</tr>
	<%	List<ProductCategory> list = Collections.checkedList(
				hs.createQuery("from ProductCategory order by name").list(), 
				ProductCategory.class);
		if(!list.isEmpty()) { 
			Iterator<ProductCategory> i = list.iterator();
			ProductCategory c = i.next(); %>
	<tr>
		<th rowspan="<%= list.size() %>">Categorias:</th> 
		<td style="text-align: right"><%= HtmlUtil.cdata(c.getName()) %></td> 
		<td><input type="checkbox" name="category" value="<%= c.getId() %>"<%
			if(p.getCategories().contains(c)) out.print("checked"); %>></td>
	</tr>
		<%	while(i.hasNext()) { 
				c = i.next(); %>
	<tr>
		<td style="text-align: right"><%= HtmlUtil.cdata(c.getName()) %></td> 
		<td><input type="checkbox" name="category" value="<%= c.getId() %>"<%
			if(p.getCategories().contains(c)) out.print("checked"); %>></td>
	</tr>
	<%		}
		} %>
</table>
</form>
<%	} %>
