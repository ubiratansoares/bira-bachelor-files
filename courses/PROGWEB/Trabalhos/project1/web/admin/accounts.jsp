<%@ page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %><%
%><%@ page import="java.sql.*, tpw.agenda.database.*, tpw.agenda.model.*" %><%
%><%@ include file="auth.jsp" %>
<jsp:useBean id="acc" class="tpw.agenda.model.Account">
	<jsp:setProperty name="acc" property="*"/>
</jsp:useBean><%
	Connection conn = ConnectionManager.getConnection("main");
	String message = null;
	
	String action = request.getParameter("action");
	String retype = request.getParameter("retype");
	String admin = request.getParameter("admin");
	retype = (retype == null)? null: retype.trim();
	if(action != null && !action.trim().isEmpty()) {
		action = action.trim();
		acc.setConnection(conn);
		if(action.equals("create")) {
			if(acc.getPassword() == null) {
				message = "Uma senha é necessária para criar o novo usuário.";
			} else if(!acc.getPassword().equals(retype)) {
				message = "Você pode ter errado ao digitar a senha. Tente novamente.";
			} else {
				try {
					acc.addPrivilege("user");
					if("on".equals(admin)) 
						acc.addPrivilege("admin");
					acc.add();
					message = String.format("Usuário criado (ID: %d).", acc.getId());
				} catch(DatabaseException e) {
					message = "Não foi possível criar o usuário.";
				}
			}
		} else if(action.equals("delete")) {
			if(acc.remove())
				message = "Usuário excluído.";
			else
				message = "Não foi possível excluir o usuário.";
		} else if(action.equals("update")) {
			acc.addPrivilege("user");
			if("on".equals(admin)) 
				acc.addPrivilege("admin");
			else
				acc.removePrivilege("admin");
			if(acc.getPassword() != null && !acc.getPassword().equals(retype)) {
				message = "You might have mistyped the new password. Try again.";
			} else {
				if(acc.set())
					message = "Usuário atualizado.";
				else
					message = "Não foi possível atualizar o usuário.";
			}
		}
	}
	
	Integer uid = (Integer) session.getAttribute("uid");
	Account user = new Account(conn);
	user.setId(uid);
	
	String email = "???";
	if(user.get()) email = user.getEmail();
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<head>
<link rel="stylesheet" href="../global.css">
<title>Agenda - Admin</title>

<body>

<div class="box">
<h1>Agenda Web</h1>
<small>Logado como <%= email %> <a href="../index.jsp?action=logout">[Logout]</a></small>

<h2>Gerenciar Usuários</h2>

<div>
<p><a href="index.jsp">Voltar para o Menu Principal</a></p>

<form method="POST" action="accounts.jsp" class="bordered">
	<h3>Editar Usuário</h3>
	
<%
	boolean selected = false;
	if(action != null && action.trim().equals("select"))
		selected = acc.get();
%>
	<table>
<%	if(selected) { %>
		<tr>
			<td class="label"><label for="uid">Selected</label></td>
			<td class="field" colspan="3">
				<%= (acc.getId() == null)? "": acc.getId() %>
				<input type="hidden" id="uid" name="id" 
					   value="<%= (acc.getId() == null)? "": acc.getId() %>">
			</td>
		</tr>
<%	} %>

		<tr>
			<td class="label"><label for="email">E-Mail</label></td>
			<td class="field">
				<input type="text" id="email" name="email" tabindex="1"
					   value="<%= (acc.getEmail() == null)? "": acc.getEmail() %>">
			</td>
			
			<td class="label"><label for="admin">Administrador</label></td>
			<td class="field">
				<input type="checkbox" id="admin" name="admin" tabindex="4"
					<%= acc.hasPrivilege("admin")? "checked": "" %>>
			</td>
		</tr>
		<tr>
			<td class="label"><label for="password">Senha</label></td>
			<td class="field"><input type="password" id="password" name="password" tabindex="2"></td>

			<td class="label"><label for="fname">Nome</label></td>
			<td class="field">
				<input type="text" id="fname" name="firstName" tabindex="5"
					   value="<%= (acc.getFirstName() == null)? "": acc.getFirstName() %>">
			</td>
		</tr>
		<tr>
			<td class="label"><label for="retype">Confirmar Senha</label></td>
			<td class="field"><input type="password" id="retype" name="retype" tabindex="3"></td>
			
			<td class="label"><label for="lname">Sobrenome</label></td>
			<td class="field">
				<input type="text" id="lname" name="lastName" tabindex="6"
					   value="<%= (acc.getLastName() == null)? "": acc.getLastName() %>">
			</td>
		</tr>
	</table>
	
	<p style="margin-bottom: 0em">
		<button type="submit" name="action" value="create">Criar</button>
		<button type="submit" name="action" value="update">Atualizar</button>
	</p>
</form>

<% if(message != null) { %>
<p id="message"><%= message %></p>
<% } %>
</div>

<%
	Account.Cursor c = user.list();
	if(c.next()) {
%>
<div class="bordered" style="overflow: auto">
<h3>Lista de Usuários</h3>

<table class="list">
	<tr>
		<th>ID</th>
		<th>E-mail</th>
		<th>Nome</th>
		<th>Sobrenome</th>
		<th>Admin?</th>
		<th colspan="2">AÇÃO</th>
	</tr>
<%
		do {
			c.setConnection(conn);
%>
	<tr>
		<td><%= c.getId() %></td>
		<td><%= c.getEmail() %></td>
		<td><%= c.getFirstName() %></td>
		<td><%= (c.getLastName() == null)? "": c.getLastName() %></td>
		<td><%= c.hasPrivilege("admin")? "sim": "não" %></td>
		<td><a href="accounts.jsp?action=delete&id=<%=c.getId()%>">EXCLUIR</a></td>
		<td><a href="accounts.jsp?action=select&id=<%=c.getId()%>">SELECIONAR</a></td>
	</tr>
<%
		} while(c.next());
%>
</table>
</div>

<%
	}
%>
</div><%
	conn.close();
%>
