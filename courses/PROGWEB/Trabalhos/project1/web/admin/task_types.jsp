<%@ page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %><%
%><%@ page import="java.sql.*, tpw.agenda.database.*, tpw.agenda.model.*" %><%
%><%@ include file="auth.jsp" %>
<jsp:useBean id="type" class="tpw.agenda.model.TaskType">
	<jsp:setProperty name="type" property="*"/>
</jsp:useBean><%
	Connection conn = ConnectionManager.getConnection("main");
	String message = null;
	
	String action = request.getParameter("action");
	if(action != null && !action.trim().isEmpty()) {
		action = action.trim();
		type.setConnection(conn);
		if(action.equals("create")) {
			try {
				type.add();
				message = String.format("Tipo criado (ID: %d).", type.getId());
			} catch(DatabaseException e) {
				message = "Não foi possível criar o tipo.";
			}
		} else if(action.equals("delete")) {
			if(type.remove())
				message = "Tipo excluído.";
			else
				message = "Não foi possível excluir o tipo.";
		} else if(action.equals("update")) {
			if(type.set())
				message = "Tipo atualizado.";
			else
				message = "Não foi possível atualizar o tipo.";
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

<h2>Gerenciar Tipos de Tarefas</h2>

<div>
<p><a href="index.jsp">Voltar para o Menu Principal</a></p>

<form method="POST" action="task_types.jsp" class="bordered">
	<h3>Editar Tipo de Tarefa</h3>

	<table>
<%
	if(action != null && action.trim().equals("select")) {
		if(type.get())
%>
		<tr>
			<td class="label"><label for="id">Selected</label></td>
			<td class="field">
				<%= (type.getId() == null)? "": type.getId() %>
				<input type="hidden" id="id" name="id" value="<%= (type.getId() == null)? "": type.getId() %>">
			</td>
		</tr>
<%
	}
%>
		<tr>
			<td class="label"><label for="name">Name</label></td>
			<td class="field"><input type="text" id="name" name="name" 
				value="<%= (type.getName() == null)? "": type.getName() %>">
			</td>
		</tr>
		<tr>
			<td class="label"><label for="description">Descrição</label></td>
			<td class="field">
				<textarea id="description" name="description"><%= 
					(type.getDescription() == null)? "": type.getDescription() 
				%></textarea>
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
	TaskType.Cursor c = type.list();
	if(c.next()) {
%>
<div class="bordered" style="overflow: auto">
<h3>Lista de Tipos de Tarefas</h3>

<table class="list">
	<tr>
		<th>ID</th>
		<th>Nome</th>
		<th>Descrição</th>
		<th colspan="2">AÇÃO</th>
	</tr>
<%
		do {
%>
	<tr>
		<td><%= c.getId() %></td>
		<td><%= c.getName() %></td>
		<td><%= (c.getDescription() == null)? "": c.getDescription() %></td>
		<td><a href="task_types.jsp?action=delete&id=<%=c.getId()%>">EXCLUIR</a></td>
		<td><a href="task_types.jsp?action=select&id=<%=c.getId()%>">SELECIONAR</a></td>
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
