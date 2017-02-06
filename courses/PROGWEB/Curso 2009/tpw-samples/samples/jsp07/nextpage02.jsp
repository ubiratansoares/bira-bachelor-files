<HTML>
	<BODY>
		Nome novamente: <%= session.getAttribute( "theName" ) %>
	</BODY>
</HTML>
<%
  // para destruir a sessão use a linha abaixo
  session.invalidate();
%>
