<%@ page import="java.sql.*" %><%
%><%@ page import="rtk.model.*" %><%
%><%@ page import="rtk.persistence.*" %><%
%><%@ page import="rtk.persistence.database.*" %><%
	String action = request.getParameter("action");
	String id = request.getParameter("uid");
	String msg = "";
	
	Connection conn = ConnectionManager.getConnection("main");
	UserAuthentication auth = new UserAuthentication(conn);
	auth.setId((id == null || id.isEmpty())? null: Integer.parseInt(id));
	auth.setEmail(request.getParameter("email"));
	auth.setPassword(request.getParameter("password"));
	
	if(action.equals("create")) {
		try {
			auth.add();
			msg = "New user created.";
		} catch(PersistenceException e) {
			msg = "Could not create user.";
		}
	} else if(action.equals("delete")) {
		if(auth.remove())
			msg = "User deleted.";
		else
			msg = "Could not delete user.";
	} else if(action.equals("update")) {
		if(auth.set())
			msg = "User updated.";
		else
			msg = "Could not update user.";
	}
	
	conn.close();
%><jsp:include page="index.jsp">
	<jsp:param name="ret" value="<%=msg%>"/>
</jsp:include>
