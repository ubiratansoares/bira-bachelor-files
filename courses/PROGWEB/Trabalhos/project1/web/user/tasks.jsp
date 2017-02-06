<%@ page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %><%
%><%@ page import="java.sql.*, java.text.*, tpw.agenda.database.*, tpw.agenda.model.*" %><%
%><%@ include file="auth.jsp" %>
<jsp:useBean id="task" class="tpw.agenda.model.Task">
	<jsp:setProperty name="task" property="*"/>
</jsp:useBean><%
	String message = null;
	Connection conn = ConnectionManager.getConnection("main");
	task.setConnection(conn);
	task.setStartTime(request.getParameter("start"));
	task.setEndTime(request.getParameter("end"));
	
	String[] types = request.getParameterValues("types");
	if(types != null) {
		for(String type : types) {
			task.addTaskType(Integer.valueOf(type));
		}
	}
	
	Integer uid = (Integer) session.getAttribute("uid");
	Account user = new Account(conn);
	user.setId(uid);
	task.setOwner(user);

	String action = request.getParameter("action");
	if(action != null && !action.trim().isEmpty()) {
		action = action.trim();
		if(task.getShortDescription() != null && task.isBusy() == null)
			task.setBusy(Boolean.valueOf(false));
		
		if(action.equals("create")) {
			if(task.getShortDescription() == null) {
				message = "Uma breve descrição é necessária para criar a nova tarefa.";
			} else {
				try {
					task.add();
					message = String.format("Tarefa criada (ID: %d).", task.getId());
				} catch(DatabaseException e) {
					message = "Não foi possível criar a tarefa.";
				}
			}
		} else if(action.equals("delete")) {
			if(task.remove())
				message = "Tarefa excluída.";
			else
				message = "Não foi possível excluir a tarefa.";
		} else if(action.equals("update")) {
			if(task.getShortDescription() == null) {
				message = "Uma breve descrição é necessária para a tarefa.";
			} else {
				if(task.set())
					message = "Tarefa atualizada.";
				else
					message = "Não foi possível atualizar a tarefa.";
			}
		}
	}
	
	String email = "???";
	if(user.get()) email = user.getEmail();
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<head>
<link rel="stylesheet" href="../global.css">
<title>Agenda</title>

<body>

<div class="box">
<h1>Agenda Web</h1>
<small>Logado como <%= email %> <a href="../index.jsp?action=logout">[Logout]</a></small>

<h2>Gerenciar Tarefas</h2>

<div>
<p><a href="index.jsp">Voltar para o Menu Principal</a></p>

<form method="POST" action="tasks.jsp" class="bordered">
	<h3>Editar Tarefa</h3>
	
	<table style="vertical-align: top">
<%
	if(action != null && action.trim().equals("select")) {
		if(task.get())
%>
		<tr>
			<td class="label"><label for="id">Selected</label></td>
			<td class="field" colspan="3">
				<%= (task.getId() == null)? "": task.getId() %>
				<input type="hidden" id="id" name="id" value="<%= (task.getId() == null)? "": task.getId() %>">
			</td>
		</tr>
<%
	}
	
	DateFormat df = Task.DATE_FORMAT;
%>
		<tr>
			<td class="label"><label for="short_desc">Descrição curta</label></td>
			<td class="field">
				<input type="text" id="short_desc" name="shortDescription" 
					   value="<%= (task.getShortDescription() == null)? "": task.getShortDescription() %>">
			</td>
			
			<td class="label"><label for="location">Lugar</label>
			<td class="field">
				<input type="text" id="location" name="location" 
					   value="<%= (task.getLocation() == null)? "": task.getLocation() %>">
			</td>
		</tr>
		<tr>
			<td class="label" rowspan="3"><label for="long_desc">Descrição longa</label>
			<td class="field" rowspan="3">
				<textarea id="long_desc" name="longDescription"><%= 
					(task.getLongDescription() == null)? "": task.getLongDescription() 
				%></textarea>
			</td>
			
			<td class="label"><label for="start">Início</label>
			<td class="field">
				<input type="text" id="start" name="start" 
					   value="<%= (task.getStartTime() == null)? "": df.format(task.getStartTime()) %>">
			</td>
		</tr>
		<tr>
			<td class="label"><label for="end">Fim</label>
			<td class="field">
				<input type="text" id="end" name="end" 
					   value="<%= (task.getEndTime() == null)? "": df.format(task.getEndTime()) %>">
			</td>
		</tr>
		<tr>
			<td class="label"><label for="busy">Ocupado</label>
			<td class="field">
				<input type="checkbox" id="busy" name="busy" 
					<%= (task.isBusy() == null || !task.isBusy())? "": "checked" %>>
			</td>
		</tr>
	</table>
	
	<ul style="display: inline-block; vertical-align: top">Tipo:
<%
		for(TaskType type : new TaskType(conn).list()) {
%>
		<li style="list-style-type: none">
		<label class="right">
			<input type="checkbox" name="types" value="<%= type.getId() %>"
				<%= task.hasTaskType(type.getName())? "checked": "" %>> 
			<%= type.getName() %>
		</label>
		</li>
<%
		}
%>
	</ul>
	
	<p style="margin-bottom: 0em">
		<button type="submit" name="action" value="create">Criar</button>
		<button type="submit" name="action" value="update">Atualizar</button>
	</p>
</form>

<% if(message != null) { %>
<p id="message"><%= message %></p>
<% } %>
</div>

<div class="bordered">
<h3>Lista de Tarefas</h3>

<p>
<a href="tasks.jsp">[todas]</a>
<%	for(TaskType type : new TaskType(conn).list()) { %>
<a href="tasks.jsp?action=filter&type=<%= type.getId() %>">[<%= type.getName() %>]</a>
<%	} %>
</p>

<table class="list">
	<tr>
		<th>ID</th>
		<th>Descrição</th>
		<th>Início</th>
		<th>Fim</th>
		<th>Onde</th>
		<th>Ocupado</th>
		<th colspan="2">AÇÃO</th>
	</tr>
	
<%
	TaskType type = new TaskType();
	if("filter".equals(action))
		type.setId(request.getParameter("type"));
	else
		type.setId((Integer) null);
	
	for(Task t : task.list(user, type)) { %>
	<tr>
		<td><%= t.getId() %></td>
		<td><%= t.getShortDescription() %></td>
		<td><%= df.format(t.getStartTime()) %></td>
		<td><%= df.format(t.getEndTime()) %></td>
		<td><%= (t.getLocation() == null)? "": t.getLocation() %></td>
		<td><%= (t.isBusy() == null)? "": (t.isBusy()? "sim": "não") %></td>
		<td><a href="tasks.jsp?action=delete&id=<%=t.getId()%>">EXCLUIR</a></td>
		<td><a href="tasks.jsp?action=select&id=<%=t.getId()%>">SELECIONAR</a></td>
	</tr>
<%	} %>
</table>
</div>

</div>
<%
	conn.close();
%>
