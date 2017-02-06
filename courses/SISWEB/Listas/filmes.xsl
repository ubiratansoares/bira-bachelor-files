<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">

<html>
<head>
	<title>Filmes Antigos</title>
</head>
<body>
		<table border="2" bgcolor="white">
			<tr>
				<th>TITULO</th>
				<th>Genero</th>
				<th>Ano</th>
			</tr>
			<xsl:for-each select="filmes/filme">
				<xsl:sort select="ano" order="ascending"/>
				<xsl:sort select="titulo"/>
				<tr>
					<td><xsl:value-of select="titulo"/></td>
					<td><xsl:value-of select="genero/@categoria"/></td>
					<td><xsl:value-of select="ano"/></td>
				</tr>
			</xsl:for-each>
		</table>
</body>
</html>

</xsl:template>
</xsl:stylesheet>