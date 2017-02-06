<%@ page import="java.sql.*, model.Bank, util.*, org.hibernate.*"%> 

<html>

<%
try{
HibernateUtil.getSession().beginTransaction(); 
Bank bank = (Bank) HibernateUtil.getSession().get(Bank.class, Integer.valueOf(request.getParameter("id"))); 

%>

<body>
	<form action="\loan" method="post">
	Pedir emprestimo
	<table class="list">
		<tr>
			<th>Valor</th> <input type="text" name="ammount"/> <br/>
			Id do banco que esta indo hidden: <%= bank.getId()%> <br/>
			Session do investor que esta indo hidden: <%= session.getAttribute("uid")%> <br/>
			<input type="hidden" name="bankID" value="<%= bank.getId()%>"/> <br/>
			<input type="hidden" name="investorID" value="<%= session.getAttribute("uid")%>"/> <br/>
		</tr>
	
		<input type="submit" value="Solicitar emprestimo"/>
	
	
	</table>
	</form>
	</div>

<% HibernateUtil.getSession().getTransaction().commit();
} catch(Exception e) {
	HibernateUtil.getSession().getTransaction().rollback();
	throw e;
}
%>

</body>
</html>