<%@ page import="java.sql.*"%><%
%><%@ page import="rtk.database.*"%><%
	Connection conn = ConnectionManager.getConnection("main");
	Statement st = conn.createStatement();
	
	String query = "SELECT * FROM user_auth " +
			"WHERE username = \'" + request.getParameter("username") +
			"\' AND password = \'" + request.getParameter("password") + "\'";
	ResultSet rs = st.executeQuery(query);
	if(rs.next()) {
%><p>Hello <%=request.getParameter("username")%>!!!</p>

<table border="1">
<%
		rs.close();
		query = "SELECT * FROM user_auth";
		rs = st.executeQuery(query);
		while(rs.next()) {
%>	<tr>
		<td><%=rs.getString("username")%></td>
		<td><%=rs.getString("password")%></td>
		<td><%=rs.getString("email")%></td>
	</tr>
<%
		}
%></table><%
	} else {
%><p>Access denied.</p><%
	}
	
	rs.close();
	st.close();
	if(conn != null) conn.close();
%>
<a href="index.jsp">Back</a>