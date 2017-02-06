<html>
	<head>
		<title>Titulo da página web</title>
		<link href="style.css" rel="stylesheet" type="text/css">
	</head>

	<body cellspacing="0" cellpadding="0" topmargin="0" leftmargin="0">

		<table border="0" cellspacing="0" cellpadding="0">
			<tr><td><img src="images/picture.jpg" border="0" /></td></tr>
			<tr bgcolor="#012559"><td class="titulo">Home&nbsp;&nbsp;&nbsp;Login&nbsp;&nbsp;&nbsp;Logout&nbsp;&nbsp;&nbsp;</td></tr>
		</table>

	<b>&nbsp;&nbsp;O horário atual é: <%= new java.util.Date() %></b><br><br><br>
	<b>&nbsp;&nbsp;O horário atual é: <% out.print(new java.util.Date()); %></b><br><br><br>
	<b>&nbsp;&nbsp;O horário atual é: <% System.out.print("server "+(new java.util.Date())); %></b><br><br><br>
<%

	for (int i = 0; i < 10; i++) {
		out.print("&nbsp;&nbsp;&nbsp;<b>"+i+"</b><br>");
	}

%>
	</body>
</html>
