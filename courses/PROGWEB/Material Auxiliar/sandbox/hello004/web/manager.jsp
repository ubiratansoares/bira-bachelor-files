<%@ page import="java.sql.*"%><%
%><%@ page import="rtk.model.*"%><%
%><%@ page import="rtk.persistence.database.*"%><%
	Connection conn = ConnectionManager.getConnection("main");
	UserAuthentication auth = new UserAuthentication(conn);
	
	String ret = request.getParameter("ret");
	if(ret != null && !ret.trim().isEmpty()) {
%><p><%=ret%></p>
<%
	}
	
	UserAuthentication.Cursor c = auth.list();
	if(c.next()) {
%><table border="1">
	<tr>
		<th>UID</th>
		<th>e-mail</th>
		<th>password</th>
		<th>DELETE</th>
		<th>SELECT</th>
	</tr>
<%
		do {
			if(c.getId() == null) c.setId(0);
			if(c.getEmail() == null) c.setEmail("");
			if(c.getPassword() == null) c.setPassword("");
%>	<tr>
		<td><%=c.getId()%></td>
		<td><%=c.getEmail()%></td>
		<td><%=c.getPassword()%></td>
		<td><a href="account.jsp?action=delete&uid=<%=c.getId()%>">DELETE</a></td>
		<td><a href="account.jsp?action=select&uid=<%=c.getId()%>">SELECT</a></td>
	</tr>
<%
		} while(c.next());
%></table>

<%
	}
%><br>

<form method="POST" action="account.jsp">
<%
	auth.setId(null);
	auth.setEmail(null);
	auth.setPassword(null);
	
	String action = request.getParameter("action");
	if(action != null && action.equals("select")) {
		String id = request.getParameter("uid");
		auth.setId((id == null || id.isEmpty())? null: Integer.parseInt(id));
		if(auth.get())
%>	<p>Selected: <%=id%></p>
	<input type="hidden" name="uid" value="<%=id%>">
<%	}
	
	String id = (auth.getId() == null)? "": auth.getId().toString();
	if(auth.getEmail() == null) auth.setEmail("");
	if(auth.getPassword() == null) auth.setPassword("");
%>	<p>E-Mail: <input type="text" name="email" value="<%=auth.getEmail()%>"></p>
	<p>Password: <input type="password" name="password" value="<%=auth.getPassword()%>"></p>
	<input type="submit" name="action" value="create">
	<input type="submit" name="action" value="update">
</form><%
	
	conn.close();
%>