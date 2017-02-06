<%@page import="java.sql.*, com.news.project.*" %>
<html>
	<head>
		<title>myWorld.com</title>
	</head>

	<body cellpadding="0" cellspacing="0" topmargin="0" leftmargin="0">
		<table width="780">
			<form action="news_insert_action.jsp">
			<tr><td><img src="../images/top.jpg" border="0"></td></tr>
			<tr><td>&nbsp;</td></tr>
			<tr><td align="center">Dados da not&iacute;cia</td></tr>
			<tr><td>&nbsp;</td></tr>
			<tr><td align="center">Data: <input type="text" name="date" size="30"></td></tr>
			<tr><td align="center">T&iacute;tulo: <input type="text" name="title" size="50"></td></tr>
			<tr><td align="center">Corpo: <textarea name="body" rows="5" cols="40"></textarea></td></tr>
			<tr><td align="center"><input type="submit" value="Inserir"></td></tr>
			<tr><td>&nbsp;</td></tr>
			</form>
			<tr bgcolor="#cccccc"><td>Copyright(c) - by POO</td></tr>
		</table>
	</body>
</html>
