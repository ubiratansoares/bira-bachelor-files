<%@ page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"><%
	String message = request.getParameter("message");
%><head>
<link rel="stylesheet" href="global.css">
<style type="text/css">
@media screen {
	form {
		display: block;
		margin: 1em;
		padding: 1em;
		border: thin solid black;
		text-align: center;
	}
}
</style>
<title>Agenda - Login</title>

<body>

<div class="box">
<h1>Agenda Web</h1>

<form method="POST" action="index.jsp">
	<p style="margin-top: 0em">Autenticação Necessária</p>
	
	<table>
		<tr>
			<td class="label"><label for="email">E-Mail</label></td>
			<td class="field"><input type="text" id="email" name="email"></td>
		</tr>
		<tr>
			<td class="label"><label for="password">Senha</label></td>
			<td class="field"><input type="password" id="password" name="password"></td>
		</tr>
	</table>
	
	<p style="margin-bottom: 0em"><button type="submit">Log In</button></p>
</form>

<% if(message != null) { %>
<p id="message"><%= message %></p>
<% } %>

</div>
