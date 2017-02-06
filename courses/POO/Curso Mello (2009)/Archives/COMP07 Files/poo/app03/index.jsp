<%@page import="java.sql.*, com.news.project.*" %>
<html>
	<head>
		<title>myWorld.com</title>
	</head>

	<body cellpadding="0" cellspacing="0" topmargin="0" leftmargin="0">
		<table width="780">
			<tr><td><img src="images/top.jpg" border="0"></td></tr>
<%
	ResultSet rs = News.findNewer();
	News news = null;
	while ((news = News.next(rs)) != null) {
		out.println("<tr><td><b>"+news.getDate()+"</b></td></tr>");
		out.println("<tr><td><b>"+news.getTitle()+"</b></td></tr>");
		out.println("<tr><td>"+news.getBody()+"</td></tr>");
		out.println("<tr><td>&nbsp;</td></tr>");
	}

/* TRAZ UMA NOTICIA
	News news = News.next(rs);
	out.println("<tr><td>"+news.getDate()+"</td></tr>");
	out.println("<tr><td>"+news.getTitle()+"</td></tr>");
	out.println("<tr><td>"+news.getBody()+"</td></tr>");
	out.println("<tr><td>&nbsp;</td></tr>");
*/
/*
	while (rs.next()) {
		out.println("<tr><td>"+rs.getString("date")+"</td></tr>");
		out.println("<tr><td>"+rs.getString("title")+"</td></tr>");
		out.println("<tr><td>"+rs.getString("body")+"</td></tr>");
		out.println("<tr><td>&nbsp;</td></tr>");
	}*/
%>
			<tr bgcolor="#cccccc"><td>Copyright(c) - by POO</td></tr>
		</table>
	</body>
</html>
