<%

String name = request.getParameter( "nome" );

session.setMaxInactiveInterval(3); // em segs
session.setAttribute( "theName", name );

%>

<HTML>
	<BODY>
		<A HREF="nextpage01.jsp">Continuar</A>
	</BODY>
</HTML>

