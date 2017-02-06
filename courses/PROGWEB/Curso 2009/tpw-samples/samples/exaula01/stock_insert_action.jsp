<jsp:useBean id="stock" class="org.stockmarket.system.Stock" scope="page" />
<jsp:setProperty name="stock" property="*" />
<%
	stock.insert();
%>
