<%@ page session="false" %><%
%><%@ include file="/templates/html-config.jsp" %><%
	//html dtd: strict, transitional, frameset
	//access: public, restricted
	//db usage: yes, no
	template_title = "Hello World!!!";
	
	if(request.getParameter("name") == null) {
		template_content = "hello.jsp";
	} else {
		template_content = "hello2.jsp";
	}
%><%@ include file="/templates/html-page.jsp" %>
