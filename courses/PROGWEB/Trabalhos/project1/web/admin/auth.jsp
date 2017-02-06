<%@ page import="java.sql.*, tpw.agenda.database.*, tpw.agenda.model.*" %><%
if(session.getAttribute("uid") == null) {
	//não tem sessão
	response.sendRedirect("../index.jsp");
	return;
} else {
	//verificar credenciais
	Connection conn = ConnectionManager.getConnection("main");
	Account acc = new Account(conn);
	acc.setId((Integer) session.getAttribute("uid"));
	if(acc.get()) {
	
		//conta ainda existe
		if(!acc.hasPrivilege("admin"))
			response.sendRedirect("../index.jsp"); //privilégios insuficientes

	} else {
		
		//conta não existe mais
		session.invalidate();
		response.sendRedirect("../index.jsp");
		return;
		
	}
	conn.close();
}
%>