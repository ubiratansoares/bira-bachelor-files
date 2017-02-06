<%
	user.User user = new user.User();
	user.setName(request.getParameter("name"));
	user.setAddress(request.getParameter("address"));
%>
Informações<BR>
Nome: <%= user.getName() %><br>
Endereço: <%= user.getAddress() %><br>
