<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.sql.*, model.Bank, util.*, org.hibernate.*"%> 
   
<%
    
%>
  
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Emprestimos e Pagamentos do Usuario</title>
</head>
<body>

<div class="bordered" style="overflow: auto">
<h3>Lista de Bancos</h3>

<%	
Bank bank = new Bank();
bank.setBalance(2.2);
bank.setInterest(1.0);
bank.setName("HSBC");
bank.setStrategy(3.0);
bank.setTotalLoans(5.5);
Session s = HibernateUtil.getSession();
s.save(bank);

%>
Lista de bancos: <br/>

<%

java.util.List list = s.createCriteria(Bank.class).list();

for (int i=0; i<list.size(); i++){
	Bank banco = (Bank) list.get(i);
	%>
	<a href="pedirEmprestimos.jsp?id=<%=banco.getId()%>"><%=banco.getName()%></a> <br/>
	<%
	
}

%> 



</table>
</body>
</html>