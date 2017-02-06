<%@ include file="auth.jsp" %>
<%@ page import="java.util.*, java.text.*" %>
<%	String action = request.getParameter("action");
	if(action == null) action = "list";
	
	Session hs = HibernateUtil.getSession();
	if(action.equals("list")) {
		String view = request.getParameter("filter");
		if(view == null) view = "customers";
		
		if(view.equals("customers")) { 
			List<Customer> list = Collections.checkedList(
					hs.createQuery("from Customer order by first_name, last_name, id")
					.list(), Customer.class); %>
		<h2>Clientes</h2>

		<table class="list">
			<tr>
				<th>Nome</th> <th>E-Mail</th> <th>País</th> <th>Estado</th> 
				<th>Último Acesso</th> <th>Situação</th>
			</tr>
		<%	for(Customer c : list) { %>
				<tr>
					<td><a title="Ver Detalhes" href="home.jsp?view=accounts&action=view&uid=<%= c.getId() %>">
							<%= HtmlUtil.cdata(c.getName().getFullName(false)) %></a></td>
					<td><%= HtmlUtil.cdata(c.getEmail()) %></td>
					<td><%= HtmlUtil.cdata(c.getAddress().getCountry()) %></td>
					<td><%= HtmlUtil.cdata(c.getAddress().getState()) %></td>
					<td><%= HtmlUtil.cdata(c.getLastAccessTime(), "yyyy-MM-dd HH:mm:ss", request.getLocale()) %></td>
					<td><%= HtmlUtil.cdata(c.getStatus()) %></td>
				</tr>
		<%	} %>
		</table>
	<%	} else if(view.equals("retailers")) { 
			Integer uid = (Integer) session.getAttribute("uid");
			List<Retailer> list = Collections.checkedList(
					hs.createQuery("from Retailer order by first_name, last_name, id")
					.list(), Retailer.class); %>
		<h2>Vendedores</h2>
		
		<p><a href="home.jsp?view=accounts&action=create">[Criar nova conta]</a></p>
		
		<table class="list">
			<tr>
				<th>Nome</th> <th>E-Mail</th> <th>País</th> <th>Cargo</th> 
				<th>Departamento</th> <th>Situação</th>
			</tr>
		<%	for(Retailer r : list) { %>
				<tr>
					<td>
					<%	if(!uid.equals(r.getId())) { %>
						<a title="Ver Detalhes" href="home.jsp?view=accounts&action=view&uid=<%= r.getId() %>">
						<%= HtmlUtil.cdata(r.getName().getFullName(false)) %></a>
					<%	} else { %>
						<%= HtmlUtil.cdata(r.getName().getFullName(false)) %>
					<%	} %>
					</td>
					<td><%= HtmlUtil.cdata(r.getEmail()) %></td>
					<td><%= HtmlUtil.cdata(r.getAddress().getCountry()) %></td>
					<td><%= HtmlUtil.cdata(r.getTitle()) %></td>
					<td><%= HtmlUtil.cdata(r.getDepartment()) %></td>
					<td><%= HtmlUtil.cdata(r.getStatus()) %></td>
				</tr>
		<%	} %>
		</table>
	<%	} 
	
	
	
	} else if(action.equals("view")) { 
		Integer uid = null; Account acc = null; Retailer r = null;
		try {
			uid = Integer.valueOf(request.getParameter("uid"));
			acc = (Account) hs.get(Account.class, uid); 
		} catch(Exception e) {
			out.println("Nenhuma conta selecionada.");
			return;
		}
		
		Class cl = Hibernate.getClass(acc);
		if(Customer.class.isAssignableFrom(cl)) { %>
			<h2>Cadastro de Cliente</h2>
	<%	} else if(Retailer.class.isAssignableFrom(cl)) { 
			r = (Retailer) hs.get(Retailer.class, uid); %>
			<h2>Cadastro de Vendedor</h2>
	<%	} else {
			out.println("Conta com erros.");
			return;
		} %>

	<form method="POST" action="account">
	<input type="hidden" name="uid" value="<%= acc.getId() %>">
	
	<table>
		<caption class="bottom">
			<% String filter = (r == null)? "customers": "retailers"; %>
			<button type="button" onclick="location='home.jsp?view=accounts' +
					'&action=list&filter=<%= filter %>'">Voltar</button>
			<button type="submit" name="action" value="update">Salvar</button>
			<button type="submit" name="action" value="delete" onclick="
				return confirm('Deseja realmente excluir esta conta?');
			">Excluir</button>
		</caption>
		<tr>
			<th>Email</th>
			<td><%= HtmlUtil.cdata(acc.getEmail()) %></td>
		</tr>
		<tr>
			<th>Nome Completo</th>
			<td><%= HtmlUtil.cdata(acc.getName().getFullName(true)) %></td>
		</tr>
		<tr>
			<th>Apelido</th>
			<td><%= HtmlUtil.cdata(acc.getName().getNickname()) %></td>
		</tr>
		<tr>
			<th>Data de Nascimento</th>
			<td><%= HtmlUtil.cdata(acc.getBirthDate(), "dd 'de' MMMM 'de' yyyy", request.getLocale()) %></td>
		</tr>
		<% if(r != null) { %>
		<tr>
			<th>Cargo</th>
			<td><%= HtmlUtil.cdata(r.getTitle()) %></td>
		</tr>
		<tr>
			<th>Departamento</th>
			<td><%= HtmlUtil.cdata(r.getDepartment()) %></td>
		</tr>
		<% } %>
		<tr>
			<th>Endereço</th>
			<td><%= HtmlUtil.cdata(acc.getAddress().getFirstLine(request.getLocale())) %></td>
		</tr>
		<tr>
			<th>Bairro</th>
			<td><%= HtmlUtil.cdata(acc.getAddress().getDistrict()) %></td>
		</tr>
		<tr>
			<th>Cidade</th>
			<td><%= HtmlUtil.cdata(acc.getAddress().getCity()) %></td>
		</tr>
		<tr>
			<th>Estado</th>
			<td><%= HtmlUtil.cdata(acc.getAddress().getState()) %></td>
		</tr>
		<tr>
			<th>País</th>
			<td><%= HtmlUtil.cdata(acc.getAddress().getCountry()) %></td>
		</tr>
		<tr>
			<th>CEP</th>
			<td><%= HtmlUtil.cdata(acc.getAddress().getPostalCode()) %></td>
		</tr>
		<tr>
			<th>Criado em:</th>
			<td><%= HtmlUtil.cdata(acc.getCreationTime(), 
				"dd-MMM-yyyy HH:mm", request.getLocale()) %></td>
		</tr>
		<tr>
			<th>Última Atualização:</th>
			<td><%= HtmlUtil.cdata(acc.getLastModificationTime(), 
				"dd-MMM-yyyy HH:mm", request.getLocale()) %></td>
		</tr>
		<tr>
			<th>Último Acesso:</th>
			<td><%= HtmlUtil.cdata(acc.getLastAccessTime(), 
				"dd-MMM-yyyy HH:mm", request.getLocale()) %></td>
		</tr>
		<tr>
			<th>Situação</th>
			<td>
				<select name="status">
				<%	for(Account.Status st : Account.Status.class.getEnumConstants()) { %>
					<option value="<%= st.ordinal() %>" <% 
						if(st.equals(acc.getStatus())) out.print("selected"); 
					%>><%= st.toString().replace("_", " ") %></option>
				<%	} %>
				</select>
			</td>
		</tr>
	</table>
	</form>



<%	} else if(action.equals("create")) { 
		Retailer r = (Retailer) session.getAttribute("account"); 
		if(r == null) r = new Retailer(); %>

<h2>Cadastrar Novo Vendedor</h2>

<form method="POST" action="account">
<input type="hidden" name="action" value="save">
<table>
<caption class="bottom">
	<button type="button" onclick="location = 'home.jsp?view=accounts' + 
			'&action=list&filter=retailers'">Voltar</button>
	<button type="reset">Limpar</button>
	<button type="submit">Salvar</button>
</caption>

<!-- informações da conta -->
<tr>
	<th><label for="email">E-mail:</label></th>
	<td colspan="3"><input id="email" type="text" maxlength="254" style="width: 25em"
			name="email" value="<%= HtmlUtil.cdata(r.getEmail()) %>"></td>
</tr>
<tr>
	<th><label for="password">Senha:</label></th>
	<td colspan="3"><input id="password" type="password" maxlength="12" name="password"></td>
</tr>
<tr>
	<th><label for="password2">Confirme a senha:</label></th>
	<td colspan="3"><input id="password2" type="password" maxlength="12" name="retype"></td>
</tr>

<!-- informações pessoais -->
<tr>
	<th><label for="fname">Primeiro Nome:</label></th>
	<td colspan="3"><input id="fname" type="text" name="fname" 
			value="<%= HtmlUtil.cdata(r.getName().getFirstName()) %>"></td>
</tr>
<tr>
	<th><label for="lname">Sobrenome:</label></th>
	<td colspan="3"><input id="lname" type="text" name="lname" 
			value="<%= HtmlUtil.cdata(r.getName().getLastName()) %>"></td>
</tr>
<tr>
	<th><label for="mnames">Nomes do Meio:</label></th>
	<td colspan="3"><input id="mnames" type="text" name="mname" 
			value="<%= HtmlUtil.cdata(r.getName().getMiddleNames()) %>">
			<small>(opcional)</small></td>
</tr>
<tr>
	<th><label for="nname">Apelido:</label></th>
	<td colspan="3"><input id="nname" type="text" name="nname" 
			value="<%= HtmlUtil.cdata(r.getName().getNickname()) %>">
			<small>(opcional)</small></td>
</tr>
<tr>
	<th><label>Data de Nascimento:</label></th>
	<td colspan="3">
	<%	if(r.getBirthDate() == null) { %>
		<input type="text" size="3" maxlength="2" name="bday">
		de <select name="bmon">
		<%	String[] months = DateFormatSymbols.getInstance(request.getLocale()).getMonths();
			for(int i = 0; i < months.length; i++) { %>
			<option value="<%= i %>"><%= months[i] %></option>
		<%	} %>
		</select>
		de <input type="text" size="5" maxlength="4" name="byear">
	<% } else { 
			Calendar cal = Calendar.getInstance(request.getLocale());
			cal.setTime(r.getBirthDate()); %>
		<input type="text" size="3" maxlength="2" name="bday"
				value="<%= Integer.toString(cal.get(Calendar.DAY_OF_MONTH)) %>">
		de <select name="bmon">
		<%	String[] months = DateFormatSymbols.getInstance(request.getLocale()).getMonths();
			for(int i = 0; i < months.length; i++) { %>
			<option value="<%= (i + 1) %>" <% if(i == cal.get(Calendar.MONTH)) {
				%>selected<% } %>><%= months[i] %></option>
		<%	} %>
		</select>
		de <input type="text" size="5" maxlength="4" name="byear"
				value="<%= Integer.toString(cal.get(Calendar.YEAR)) %>">
	<%	} %>
	</td>
</tr>
<tr>
	<th><label for="title">Cargo:</label></th>
	<td colspan="3"><input id="title" type="text" name="title" 
			value="<%= HtmlUtil.cdata(r.getTitle()) %>"></td>
</tr>
<tr>
	<th><label for="dept">Departmento:</label></th>
	<td colspan="3"><input id="dept" type="text" name="dept" 
			value="<%= HtmlUtil.cdata(r.getDepartment()) %>">
			<small>(opcional)</small></td>
</tr>

<!-- endereço de residência -->
<tr>
	<th><label for="street">Endereço:</label></th>
	<td colspan="3">
		<select name="type">
			<%	String type = r.getAddress().getType(); 
				if(type == null) type = "street"; %>
			<option value="" <% if(type.isEmpty()) {%>selected<% } %>></option>
			<option value="alley" <% if(type.equals("alley")) {%>selected<% } %>>Alameda</option>
			<option value="avenue" <% if(type.equals("avenue")) {%>selected<% } %>>Avenida</option>
			<option value="square" <% if(type.equals("square")) {%>selected<% } %>>Praça</option>
			<option value="block" <% if(type.equals("block")) {%>selected<% } %>>Quadra</option>
			<option value="road" <% if(type.equals("road")) {%>selected<% } %>>Rodovia</option>
			<option value="street" <% if(type.equals("street")) {%>selected<% } %>>Rua</option>
			<option value="lane" <% if(type.equals("lane")) {%>selected<% } %>>Travessa</option>
		</select>
		<input id="street" type="text" size="50" name="street"
			value="<%= HtmlUtil.cdata(r.getAddress().getName()) %>">
	</td>
</tr>
<tr>
	<th><label for="number">Número:</label></th>
	<td><input id="number" type="text" size="6" maxlength="5" 
			name="number" value="<%= HtmlUtil.cdata(r.getAddress().getNumber()) %>"></td>
	<th><label for="complement">Complemento:</label></th>
	<td><input id="complement" type="text" style="width: 15em" 
			name="complement" value="<%= HtmlUtil.cdata(r.getAddress().getComplement()) %>">
		<small>(opcionais)</small></td>
</tr>
<tr>
	<th><label for="district">Bairro:</label></th>
	<td colspan="3"><input id="district" type="text" style="width: 83%" 
			name="district" value="<%= HtmlUtil.cdata(r.getAddress().getDistrict()) %>">
		<small>(opcional)</small></td>
</tr>
<tr>
	<th><label for="city">Cidade:</label></th>
	<td colspan="3"><input id="city" type="text" style="width: 98%" 
			name="city" value="<%= HtmlUtil.cdata(r.getAddress().getCity()) %>"></td>
</tr>
<tr>
	<th><label for="state">Estado:</label></th>
	<td colspan="3">
		<input type="text" name="state" 
				value="<%= HtmlUtil.cdata(r.getAddress().getState()) %>">
		<small>(opcional)</small>
	</td>
</tr>
<tr>
	<th><label for="country">País:</label></th>
	<td colspan="3">
		<input type="text" name="country" 
				value="<%= HtmlUtil.cdata(r.getAddress().getCountry()) %>">
	</td>
</tr>
<tr>
	<th><label for="pcode">CEP:</label></th>
	<td colspan="3">
		<input id="pcode" type="text" size="15" maxlength="15"
			name="pcode" value="<%= HtmlUtil.cdata(r.getAddress().getPostalCode()) %>">
	</td>
</tr>
</table>
</form>

<%	} %>
