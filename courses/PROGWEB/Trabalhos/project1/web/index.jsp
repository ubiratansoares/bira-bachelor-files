<%@ page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %><%
%><%@ page import="java.sql.*, tpw.agenda.database.*, tpw.agenda.model.*" %><%
	if("logout".equals(request.getParameter("action"))) {
	
		//logout
		session.invalidate();
%><jsp:include page="login.jsp">
	<jsp:param name="message" value="Você saiu do sistema."/>
</jsp:include><%

	} else if(session.getAttribute("uid") == null) {
	
		//não tem sessão
%><jsp:useBean id="acc" class="tpw.agenda.model.Account">
	<jsp:setProperty name="acc" property="*"/>
</jsp:useBean><%

		if(acc.getEmail() == null) {
		
			//primeira visita
%><jsp:include page="login.jsp"/><%

		} else {
		
			//tentativa de login
			Connection conn = ConnectionManager.getConnection("main");
			acc.setConnection(conn);
			if(acc.authenticate()) {
			
				//login ok 
				session.setAttribute("uid", acc.getId());
				if(acc.hasPrivilege("admin"))
					response.sendRedirect("admin/index.jsp");
				else if(acc.hasPrivilege("user"))
					response.sendRedirect("user/index.jsp");
				else {
					session.invalidate();
%><jsp:include page="login.jsp">
	<jsp:param name="message" value="Privilégios insuficientes."/>
</jsp:include><%
				}

			} else {
			
				//login falhou
%><jsp:include page="login.jsp">
	<jsp:param name="message" value="Acesso negado."/>
</jsp:include><%

			}
			
			conn.close();
		}
		
	} else {
	
		//já possui sessão, redirecionar
		Connection conn = ConnectionManager.getConnection("main");
		Account acc = new Account(conn);
		acc.setId((Integer) session.getAttribute("uid"));
		if(acc.get()) {
		
			//conta ainda existe
			if(acc.hasPrivilege("admin"))
				response.sendRedirect("admin/index.jsp");
			else if(acc.hasPrivilege("user"))
				response.sendRedirect("user/index.jsp");
			else {
				session.invalidate();
%><jsp:include page="login.jsp">
	<jsp:param name="message" value="Privilégios insuficientes."/>
</jsp:include><%
			}

		} else {
		
			//conta não existe mais
			session.invalidate();
%><jsp:include page="login.jsp">
	<jsp:param name="message" value="Esta conta foi excluída."/>
</jsp:include><%

		}
		
		conn.close();
	}
	
%>
