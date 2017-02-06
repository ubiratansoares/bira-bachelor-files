<%@ include file="auth.jsp" %>
<% ResourceBundle rbGlobal = HtmlUtil.getBundle("global", request); %>
<p><%= rbGlobal.getString("footer.project") %> 
	<em><%= rbGlobal.getString("footer.course") %></em>.</p>
<p><%= rbGlobal.getString("footer.students") %>: 
	Petrus A. Pavoni Gomes (5634966), 
	Ricardo Takashi Kagawa (5634712), 
	Ubiratan F. Soares (5634292)</p>
<p>São Carlos, <%= rbGlobal.getString("footer.date") %> - 
	Universidade de São Paulo</p>
