<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ page import="model.*, util.*" %>
<%	String action = request.getParameter("action");
	Integer uid = (Integer) session.getAttribute("uid");
	Account acc = null;
	if(uid != null) {
		acc = (Account) HibernateUtil.getSession().get(Account.class, uid);
	}
	
	if(action == null) {
		//ação padrão
		if(acc != null) {
			action = (acc.getPrivilege().equals(Account.Privilege.NORMAL))?
				"home": "list";
		} else
			action = "create"; //se cadastrar
	}
	
	if(action.equals("create")) { 
		if(acc != null && acc.getPrivilege().equals(Account.Privilege.NORMAL))
			response.sendRedirect("index.jsp?error=Acesso+negado.");
		String which = (acc == null)? "Usuário": "Administrador"; %>

<form method="post" action="users">
	<h2>Cadastro de Novo <%= which %></h2>
	
	<table class="form">
	<tr> <th>Nome de Usuário:</th> <td><input type="text" name="username"></td> </tr>
	<tr> <th>Senha:</th> <td><input type="password" name="password"></td> </tr>
	<tr> <th>Confirmar Senha:</th> <td><input type="password" name="retype"></td> </tr>
	<tr> <th>Nome:</th> <td><input type="text" name="name"></td> </tr>
	<tr> <td colspan="2" style="text-align: center">
			<button name="action" value="create">Salvar</button>
			</td> </tr>
	</table>
</form>

<%	} else if(action.equals("home")) { %>
<jsp:include page="home.jsp"/>
<%	} %>
