<%

	if (request.getParameter("nome").length() > 0 ) {
		out.println(request.getParameter("nome")+"<br><br>");
		out.println(request.getParameter("endereco")+"<br><br>");
	} else {
		response.sendRedirect("http://www.google.com.br");
	}

%>
