<%@ page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %><%
%><%@ page import="tpw.agenda.database.*, tpw.agenda.model.*" %><%
%><%@ page import="java.sql.*, java.text.*" %><%
%><%@ include file="auth.jsp" %><%
	Integer uid = (Integer) session.getAttribute("uid");
	Connection conn = ConnectionManager.getConnection("main");
	Account acc = new Account(conn);
	acc.setId(uid);
	
	String email = "???";
	String name = "???";
	String last_access = "???";
	if(acc.get()) {
		email = acc.getEmail();
		name = acc.getFirstName();
		DateFormat df = new SimpleDateFormat("dd 'de' MMMM 'de' yyyy 'às' H:MM:ss");
		last_access = (acc.getLastVisitTime() == null)? 
				"????": df.format(acc.getLastVisitTime());
	}
%><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<head>
<link rel="stylesheet" href="../global.css">
<title>Agenda</title>

<body>

<div class="box">
<h1>Agenda Web</h1>
<small>Logado como <%= email %> <a href="../index.jsp?action=logout">[Logout]</a></small>

<% if(acc.hasPrivilege("admin")) { %>
<h2>Módulo do Usuário</h2>
<% } %>

<p>Olá <%= name %>! Sua última visita foi em <%= last_access %>.</p>

<ul class="options">
	<li><a href="index.jsp?action=manage_tasks">Gerenciar Tarefas</a></li>

<% if(acc.hasPrivilege("admin")) { %>
	<li><a href="index.jsp?action=admin_menu">Módulo de Admin</a></li>
<% } %>
</ul>

</div>

<%
	conn.close();
%>
