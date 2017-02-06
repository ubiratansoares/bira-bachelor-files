<jsp:useBean id="user" class="org.usp.news.User" scope="page"/>
<jsp:setProperty name="user" property="*"/> 
<!-- jsp:setProperty name="user" property="login"/ --> 
<%
	user.insert();
%>
<jsp:include page="message.jsp?msg=0" />
