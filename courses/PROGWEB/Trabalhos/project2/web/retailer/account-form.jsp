<%@ include file="auth.jsp" %>
<%@ page import="java.text.*, java.util.*" %>
<%	String form = request.getParameter("form");
	if(form == null) form = "view";
	Session hs = HibernateUtil.getSession();
	Integer uid = (Integer) session.getAttribute("uid");
	Retailer r = (Retailer) hs.get(Retailer.class, uid);
	if(form.equals("account")) { %>

<h2>Informações de Acesso</h2>

<p>Dados usados na autenticação do sistema.</p>

<form method="POST" action="account">
<input type="hidden" name="form" value="account">
<table>
<caption class="bottom">
	<button type="reset">Limpar</button>
	<button type="submit" name="action" value="update">Salvar</button>
</caption>
<tr>
	<th><label for="email">E-mail:</label></th>
	<td><input id="email" type="text" maxlength="254" style="width: 25em"
			name="email" value="<%= HtmlUtil.cdata(r.getEmail()) %>"></td>
</tr>
<tr>
	<th><label for="password">Nova senha:</label></th>
	<td><input id="password" type="password" maxlength="12" name="newpwd"></td>
</tr>
<tr>
	<th><label for="password2">Confirme a nova senha:</label></th>
	<td><input id="password2" type="password" maxlength="12" name="retype"></td>
</tr>
<tr>
	<th><label for="old-password">Senha atual:</label></th>
	<td><input id="old-password" type="password" maxlength="12" name="password"></td>
</tr>
</table>



<%	} else if(form.equals("person")) { %>

<h2>Informações Pessoais</h2>

<p>Informações pessoais e de cargo para cadastro.</p>

<form method="POST" action="account">
<input type="hidden" name="action" value="update">
<input type="hidden" name="form" value="person">
<table>
<caption class="bottom">
	<button type="reset">Limpar</button>
	<button type="submit">Salvar</button>
</caption>
<tr>
	<th><label for="fname">Primeiro Nome:</label></th>
	<td><input id="fname" type="text" name="fname" 
			value="<%= HtmlUtil.cdata(r.getName().getFirstName()) %>"></td>
</tr>
<tr>
	<th><label for="lname">Sobrenome:</label></th>
	<td><input id="lname" type="text" name="lname" 
			value="<%= HtmlUtil.cdata(r.getName().getLastName()) %>"></td>
</tr>
<tr>
	<th><label for="mnames">Nomes do Meio:</label></th>
	<td><input id="mnames" type="text" name="mname" 
			value="<%= HtmlUtil.cdata(r.getName().getMiddleNames()) %>">
			<small>(opcional)</small></td>
</tr>
<tr>
	<th><label for="nname">Apelido:</label></th>
	<td><input id="nname" type="text" name="nname" 
			value="<%= HtmlUtil.cdata(r.getName().getNickname()) %>">
			<small>(opcional)</small></td>
</tr>
<tr>
	<th><label>Data de Nascimento:</label></th>
	<td>
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
	<td><input id="title" type="text" name="title" 
			value="<%= HtmlUtil.cdata(r.getTitle()) %>"></td>
</tr>
<tr>
	<th><label for="dept">Departmento:</label></th>
	<td><input id="dept" type="text" name="dept" 
			value="<%= HtmlUtil.cdata(r.getDepartment()) %>">
			<small>(opcional)</small></td>
</tr>
</table>
</form>



<%	} else if(form.equals("address")) { %>

<h2>Endereço de Residência</h2>

<p>Enviaremos documentos oficiais para este endereço.</p>

<form method="POST" action="account">
<input type="hidden" name="action" value="update">
<input type="hidden" name="form" value="address">
<table>
<caption class="bottom">
	<button type="reset">Limpar</button>
	<button type="submit">Salvar</button>
</caption>
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



<%	} else if(form.equals("view")) { %>

<h2>Seu Cadastro</h2>

<table>
	<%	if(request.getParameter("edit") == null) { %>
	<caption class="bottom">
		<form method="GET" action="account.jsp">
			<button type="submit" name="action" value="view">Editar</button>
		</form>
	</caption>
	<%	} %>
	<tr>
		<th>Email</th>
		<td><%= HtmlUtil.cdata(r.getEmail()) %></td>
	</tr>
	<tr>
		<th>Nome Completo</th>
		<td><%= HtmlUtil.cdata(r.getName().getFullName(true)) %></td>
	</tr>
	<tr>
		<th>Apelido</th>
		<td><%= HtmlUtil.cdata(r.getName().getNickname()) %></td>
	</tr>
	<tr>
		<th>Data de Nascimento</th>
		<td><%= HtmlUtil.cdata(r.getBirthDate(), "dd 'de' MMMM 'de' yyyy", request.getLocale()) %></td>
	</tr>
	<tr>
		<th>Cargo</th>
		<td><%= HtmlUtil.cdata(r.getTitle()) %></td>
	</tr>
	<tr>
		<th>Departamento</th>
		<td><%= HtmlUtil.cdata(r.getDepartment()) %></td>
	</tr>
	<tr>
		<th>Endereço</th>
		<td><%= HtmlUtil.cdata(r.getAddress().getFirstLine(request.getLocale())) %></td>
	</tr>
	<tr>
		<th>Bairro</th>
		<td><%= HtmlUtil.cdata(r.getAddress().getDistrict()) %></td>
	</tr>
	<tr>
		<th>Cidade</th>
		<td><%= HtmlUtil.cdata(r.getAddress().getCity()) %></td>
	</tr>
	<tr>
		<th>Estado</th>
		<td><%= HtmlUtil.cdata(r.getAddress().getState()) %></td>
	</tr>
	<tr>
		<th>País</th>
		<td><%= HtmlUtil.cdata(r.getAddress().getCountry()) %></td>
	</tr>
	<tr>
		<th>CEP</th>
		<td><%= HtmlUtil.cdata(r.getAddress().getPostalCode()) %></td>
	</tr>
</table>



<%	} %>
