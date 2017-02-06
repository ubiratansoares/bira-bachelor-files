<%@ page import="java.sql.*" %>
<%
	String codigo = request.getParameter("codigo");
	String nome = request.getParameter("nome");
	String login = request.getParameter("login");
	String senha = request.getParameter("senha");

	Class.forName("org.postgresql.Driver");
	String url = "jdbc:postgresql://localhost:5432/testdb01";
	String username = "testuser";
	String password = "testpass";
	Connection conn = 
		DriverManager.getConnection(url, username, password);

	out.println("<br><b>"+nome+"</b><br>");

	// conexao aberta com o banco de dados
	Statement stmt = conn.createStatement();
	stmt.executeUpdate("insert into _user (codigo, nome, login, senha) values ("+codigo+", '"+nome+"', '"+login+"', '"+senha+"')");
	stmt.close();
	conn.close();
%>
