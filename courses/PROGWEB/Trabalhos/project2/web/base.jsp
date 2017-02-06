<%@ include file="auth.jsp" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html lang="en-US">
<head>
	<% String base = this.getServletContext().getContextPath(); %>
	<link rel="stylesheet" href="<%= base %>/global.css">
	<% for(String file : request.getParameterValues("css_file")) { %>
	<link rel="stylesheet" href="<%= base + file %>">
	<% } %>
	<title><%= request.getParameter("title") %></title>
</head>

<body>
	<div class="layout" id="top">
		<jsp:include page="header.jsp"/>
	</div>
	
	<div class="layout" id="middle">
		<% String left = request.getParameter("left_area"); %>
		<div class="layout" id="left">
			<jsp:include page="<%= left %>">
				<jsp:param name="page_area" value="left"/>
			</jsp:include>
		</div>
		
		<% String main = request.getParameter("main_area"); %>
		<div class="layout" id="main">
			<jsp:include page="<%= main %>">
				<jsp:param name="page_area" value="main"/>
			</jsp:include>
		</div>
		
		<% String right = request.getParameter("right_area"); %>
		<div class="layout" id="right">
			<jsp:include page="<%= right %>">
				<jsp:param name="page_area" value="right"/>
			</jsp:include>
		</div>
	</div>
	
	<div class="layout" id="bottom">
		<jsp:include page="footer.jsp"/>
	</div>
</body>
</html>
