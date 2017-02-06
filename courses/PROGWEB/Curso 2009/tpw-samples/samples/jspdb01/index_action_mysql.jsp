<%@ page import="java.util.*, db.*, java.sql.*" %>
<%

    try {
	DBConnection db = new DBConnection(
		"org.gjt.mm.mysql.Driver",
		"jdbc:mysql://localhost/db02", 
		"root", "");

	String name = request.getParameter("nome");
	String addr = request.getParameter("endereco");

	db.update("insert into registry values ('"+name+"', '"+addr+"')");
	ResultSet rs = db.query("select name, address from registry");

	while (rs.next()) {
		out.print(rs.getString("name")+" "+rs.getString("address")+"<br>");
	}
	// fechar statement do query
	db.close();
     } catch (Exception e){ e.printStackTrace(); }

%>
