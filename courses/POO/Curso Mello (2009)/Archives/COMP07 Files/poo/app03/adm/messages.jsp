<html>
	<head><title>Messages</title></head>

	<body bgcolor="#cccccc">
		<table border="0">
			<tr>
				<td>
	<%
		int code = 
			Integer.parseInt(request.getParameter("code"));
		switch (code) {
			case 0: out.println("Not&iacute;cia inserida com sucesso!");
				break;
		}
	%>
				</td>
			</tr>
	<tr>
		<td><input type="button" value="Voltar" 
			onClick="javascript: history.back();"></td>
	</tr>
		</table>
		</form>
	</body>
</html>
