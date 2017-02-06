<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ page import="util.HtmlUtil" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<%	String[] css = request.getParameterValues("page.css");
	if(css != null) {
		for(String file : css) { %>
<link rel="stylesheet" type="text/css" media="screen" href="<%= file %>">
	<%	}
	} %>
<title><%= HtmlUtil.cdata(request.getParameter("page.title")) %></title>
</head>
<body>
	<div id="header">
		<jsp:include page="header.jsp"/>
	</div>
	<hr>
	<div id="main">
	<%	String[] messages;
		
		messages = request.getParameterValues("error");
		if(messages != null) {
			for(String msg : messages) { %>
		<div class="error"><%= msg %></div>
		<%	}
		}
		
		messages = request.getParameterValues("warn");
		if(messages != null) {
			for(String msg : messages) { %>
		<div class="warn"><%= msg %></div>
		<%	}
		}
		
		messages = request.getParameterValues("info");
		if(messages != null) {
			for(String msg : messages) { %>
		<div class="info"><%= msg %></div>
		<%	}
		}
		
		String content = request.getParameter("page.content");
		if(content != null) { %>
		<jsp:include page="<%= content %>"/>
	<%	} %>
	</div>
	<hr>
	<div id="footer">
		<jsp:include page="footer.jsp"/>
	</div>
</body>
</html>
