<%@ include file="auth.jsp" %>
<%@ page import="java.text.*, java.util.*" %>
<%	String form = request.getParameter("form");
	if(form == null) form = "account";
	Customer c = (Customer) session.getAttribute("account");
	if(c == null) c = new Customer();
	if(form.equals("account")) { %>

<h2>Informações de Acesso</h2>

<p>Por favor, forneça um endereço de e-mail válido que você possa 
acessar e uma senha de sua escolha. Estas informações serão usadas para 
que você possa acessar nosso sistema, portanto são campos 
obrigatórios.</p>

<form method="POST" action="account">
<input type="hidden" name="action" value="gather">
<input type="hidden" name="form" value="account">
<table>
<caption class="bottom">
	<button type="reset">Limpar</button>
	<button type="submit">Próximo</button>
</caption>
<tr>
	<th><label for="email">E-mail:</label></th>
	<td><input id="email" type="text" maxlength="254" style="width: 25em"
			name="email" value="<%= HtmlUtil.cdata(c.getEmail()) %>"></td>
</tr>
<tr>
	<th><label for="password">Senha:</label></th>
	<td><input id="password" type="password" maxlength="12" name="password"></td>
</tr>
<tr>
	<th><label for="password2">Confirme a senha:</label></th>
	<td><input id="password2" type="password" maxlength="12" name="retype"></td>
</tr>
<tr>
	<td colspan="2" class="center"><label>Li e aceito os 
		<a href="#">Termos de Serviço</a> 
		<input type="checkbox" name="tos"></label></td>
</tr>
</table>
</form>


<%	} else if(form.equals("person")) { %>

<h2>Informações Pessoais</h2>

<p>Agora, informe alguns de seus dados pessoais para sabermos quem você 
é. Algumas destas informações serão usadas para confirmar sua 
identidade durante as transações, e outras serão usadas para 
entregarmos seus produtos. Campos opcionais estão indicados.</p>

<form method="POST" action="account">
<input type="hidden" name="action" value="gather">
<input type="hidden" name="form" value="person">
<table>
<caption class="bottom">
	<button type="reset">Limpar</button>
	<button type="submit">Próximo</button>
</caption>
<tr>
	<th><label for="fname">Primeiro Nome:</label></th>
	<td><input id="fname" type="text" name="fname" 
			value="<%= HtmlUtil.cdata(c.getName().getFirstName()) %>"></td>
</tr>
<tr>
	<th><label for="lname">Sobrenome:</label></th>
	<td><input id="lname" type="text" name="lname" 
			value="<%= HtmlUtil.cdata(c.getName().getLastName()) %>"></td>
</tr>
<tr>
	<th><label for="mnames">Nomes do Meio:</label></th>
	<td><input id="mnames" type="text" name="mname" 
			value="<%= HtmlUtil.cdata(c.getName().getMiddleNames()) %>">
			<small>(opcional)</small></td>
</tr>
<tr>
	<th><label for="nname">Apelido:</label></th>
	<td><input id="nname" type="text" name="nname" 
			value="<%= HtmlUtil.cdata(c.getName().getNickname()) %>">
			<small>(opcional)</small></td>
</tr>
<tr>
	<th><label>Data de Nascimento:</label></th>
	<td>
	<%	if(c.getBirthDate() == null) { %>
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
			cal.setTime(c.getBirthDate()); %>
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
</table>
</form>



<%	} else if(form.equals("address")) { %>

<h2>Endereço de Entrega</h2>

<p>Para podermos entregar suas compras, precisaremos de seu endereço 
(ou o endereço de algém que possa receber suas encomendas). Por favor 
preencha o formulário abaixo com seu endereço, tão precisamente quanto 
puder.</p>

<form method="POST" action="account">
<input type="hidden" name="action" value="gather">
<input type="hidden" name="form" value="address">
<table>
<caption class="bottom">
	<button type="reset">Limpar</button>
	<button type="submit">Próximo</button>
</caption>
<tr>
	<th><label for="street">Endereço:</label></th>
	<td colspan="3">
		<select name="type">
			<%	String type = c.getAddress().getType(); 
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
			value="<%= HtmlUtil.cdata(c.getAddress().getName()) %>">
	</td>
</tr>
<tr>
	<th><label for="number">Número:</label></th>
	<td><input id="number" type="text" size="6" maxlength="5" 
			name="number" value="<%= HtmlUtil.cdata(c.getAddress().getNumber()) %>"></td>
	<th><label for="complement">Complemento:</label></th>
	<td><input id="complement" type="text" style="width: 15em" 
			name="complement" value="<%= HtmlUtil.cdata(c.getAddress().getComplement()) %>">
		<small>(opcionais)</small></td>
</tr>
<tr>
	<th><label for="district">Bairro:</label></th>
	<td colspan="3"><input id="district" type="text" style="width: 83%" 
			name="district" value="<%= HtmlUtil.cdata(c.getAddress().getDistrict()) %>">
		<small>(opcional)</small></td>
</tr>
<tr>
	<th><label for="city">Cidade:</label></th>
	<td colspan="3"><input id="city" type="text" style="width: 98%" 
			name="city" value="<%= HtmlUtil.cdata(c.getAddress().getCity()) %>"></td>
</tr>
<tr>
	<th><label for="state">Estado:</label></th>
	<td colspan="3">
		<input type="text" name="state" 
				value="<%= HtmlUtil.cdata(c.getAddress().getState()) %>">
		<small>(opcional)</small>
	</td>
</tr>
<tr>
	<th><label for="country">País:</label></th>
	<td colspan="3">
		<input type="text" name="country" 
				value="<%= HtmlUtil.cdata(c.getAddress().getCountry()) %>">
	</td>
</tr>
<tr>
	<th><label for="pcode">CEP:</label></th>
	<td colspan="3">
		<input id="pcode" type="text" size="15" maxlength="15"
			name="pcode" value="<%= HtmlUtil.cdata(c.getAddress().getPostalCode()) %>">
	</td>
</tr>
</table>
</form>



<%	} else if(form.equals("commit")) { %>

<h2>Revisando suas Informações</h2>

<p>Confira seus dados antes de salvar.</p>

<table>
	<tr>
		<th>Email</th>
		<td><%= HtmlUtil.cdata(c.getEmail()) %></td>
	</tr>
	<tr>
		<th>Nome Completo</th>
		<td><%= HtmlUtil.cdata(c.getName().getFullName(true)) %></td>
	</tr>
	<tr>
		<th>Apelido</th>
		<td><%= HtmlUtil.cdata(c.getName().getNickname()) %></td>
	</tr>
	<tr>
		<th>Data de Nascimento</th>
		<td><%= HtmlUtil.cdata(c.getBirthDate(), "dd 'de' MMMM 'de' yyyy", request.getLocale()) %></td>
	</tr>
	<tr>
		<th>Endereço</th>
		<td><%= HtmlUtil.cdata(c.getAddress().getFirstLine(request.getLocale())) %></td>
	</tr>
	<tr>
		<th>Bairro</th>
		<td><%= HtmlUtil.cdata(c.getAddress().getDistrict()) %></td>
	</tr>
	<tr>
		<th>Cidade</th>
		<td><%= HtmlUtil.cdata(c.getAddress().getCity()) %></td>
	</tr>
	<tr>
		<th>Estado</th>
		<td><%= HtmlUtil.cdata(c.getAddress().getState()) %></td>
	</tr>
	<tr>
		<th>País</th>
		<td><%= HtmlUtil.cdata(c.getAddress().getCountry()) %></td>
	</tr>
	<tr>
		<th>CEP</th>
		<td><%= HtmlUtil.cdata(c.getAddress().getPostalCode()) %></td>
	</tr>
</table>

<form method="POST" action="account">
	<input type="hidden" name="form" value="commit">
	
	Todas as informações estão corretas? 
	<button type="submit" name="action" value="save">Salvar</button>
	<button type="submit" name="action" value="clear">Limpar</button>
</form>



<%	} else if(form.equals("committed")) { %>

<h2>Concluído</h2>

<p>Seu cadastro foi efetuado. Você pode usar o e-mail e a senha 
cadastradas para entrar no sistema, através do formulário no canto 
superior direito da página.</p>

<p><em>Boas compras!</em></p>



<%	} %>
