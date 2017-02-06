<%@ page session="false" %><%
%><%@ include file="/templates/html-config.jsp" %><%
	//html dtd: strict, transitional, frameset
	//access: public, restricted
	//db usage: yes, no
	template_title = "Hello World!!!";
	
	if(request.getParameter("username") == null) {
		template_content = "login.jsp";
	} else {
		template_content = "list.jsp";
	}
%><%@ include file="/templates/html-page.jsp" %>
