<%@ page session="false" %><%
%><%@ page import="java.sql.*" %><%
%><%@ page import="rtk.database.*" %><%
%><%@ include file="/templates/html-config.jsp" %><%
	template_title = "Hello World!!!";
	
	String username = request.getParameter("username");
	String password = request.getParameter("password");
	String email = request.getParameter("email");
	
	if(username.isEmpty() || password.isEmpty() || email.isEmpty()) {
		template_content = "login.jsp";
	} else {
		Connection conn = ConnectionManager.getConnection("main");
		Statement st = conn.createStatement();
		
		int r = st.executeUpdate("INSERT INTO user_auth (username, password, email) VALUES('" +
				username + "', '" + password + "', '" + email + "')");
		
		st.close();
		if(conn != null) conn.close();
		template_content = (r == 1)? "list.jsp": "login.jsp";
	}
%><%@ include file="/templates/html-page.jsp" %>
