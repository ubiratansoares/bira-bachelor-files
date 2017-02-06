<%
	switch(template_dtd) {
		case DTD_TRANSITIONAL:
%><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/1999/REC-html401-19991224/loose.dtd"><%
			break;
		case DTD_FRAMESET:
%><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Frameset//EN" "http://www.w3.org/TR/1999/REC-html401-19991224/frameset.dtd"><%
			break;
		default:
%><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"><%
	}
%>
<html>
<head>
	<title><%=template_title%></title>
</head>

<body>

<jsp:include page="<%=template_content%>"/>

</body>
</html>
