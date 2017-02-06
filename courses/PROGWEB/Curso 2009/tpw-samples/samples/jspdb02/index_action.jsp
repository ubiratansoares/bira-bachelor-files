<%@ page import="java.util.*, db.*, java.sql.*" %>
<%
    try {
	DBConnection db = new DBConnection(
		"org.postgresql.Driver",
		"jdbc:postgresql://localhost:5432/db02", 
		"user01", "user01");

	String name = request.getParameter("nome");
	String addr = request.getParameter("endereco");

	db.update("insert into registry values ('"+name+"', '"+addr+"')");

	db.close();

     } catch (Exception e){ out.print(e.getMessage()); }
%>
<jsp:include page="message.jsp?msg=0" />
