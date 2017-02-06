<%@ page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8" %><%
%><%@ include file="auth.jsp" %><%
if(request.getParameter("action") == null || request.getParameter("action").trim().isEmpty()) {
%><jsp:include page="main.jsp"/><%
} else if(request.getParameter("action").trim().equals("manage_users")) {
%><jsp:include page="accounts.jsp"/><%
} else if(request.getParameter("action").trim().equals("manage_task_types")) {
%><jsp:include page="task_types.jsp"/><%
} else if(request.getParameter("action").trim().equals("user_menu")) {
	response.sendRedirect("../user/index.jsp");
}
%>
