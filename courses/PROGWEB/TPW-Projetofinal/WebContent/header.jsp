<h1>TPW Invest</h1>

<%	if(session.getAttribute("uid") == null) { %>
<p><a href="index.jsp?page=user&action=create">Não é cadastrado?</a></p>
<%	} %>

<form id="session" action="auth" method="POST">
<%	if(session.getAttribute("uid") == null) { %>
	<label>Usuário: <input type="text" name="username" size="15"></label>
	<label>Senha: <input type="password" name="password" size="15"></label>
	<button name="action" value="login">Entrar</button>
<%	} else { %>
	<button name="action" value="logout">Sair</button>
<%	} %>
</form>
