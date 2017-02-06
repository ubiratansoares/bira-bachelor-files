<%

	String hostname = request.getRemoteHost();
	if (hostname != null) {
		%>
		<b>Hostname:</b> <%= hostname %> <br><br>
		<%

		out.println(request.getParameter("nome")+"<br><br>");
		out.println(request.getParameter("endereco")+"<br><br>");
	}

%>
