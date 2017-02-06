<%@ page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %><%
%><%@ include file="auth.jsp" %><%
if(request.getParameter("action") == null || request.getParameter("action").trim().isEmpty()) {
%><jsp:include page="main.jsp"/><%
} else if(request.getParameter("action").trim().equals("manage_tasks")) {
%><jsp:include page="tasks.jsp"/><%
} else if(request.getParameter("action").trim().equals("admin_menu")) {
	response.sendRedirect("../admin/index.jsp");
}
%>
