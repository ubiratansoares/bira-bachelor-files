<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">

<html>
<head>
	<link rel="stylesheet" type="text/css" href="format.css" />		
	<style TYPE = "text/css"> </style>
	<title> Grade 1 </title>
</head>
<body>
	<p> <b> Nome: </b> <xsl:value-of select="inicio/nome"/> </p>
	<p> <b> Nº USP: </b> <xsl:value-of select="inicio/nro_usp"/> </p>
	<p> <b> e-mail: </b> <xsl:value-of select="inicio/e-mail"/> </p>
	<table class="lista" border="3" align="center">
		<thead>
			<tr class="topo">	
				<th>Código</th>
				<th>Disciplina</th>	
			</tr>		
		</thead>
	<xsl:for-each select="inicio/gradehoraria/linha/aula[@código!='vazio' and @nome!='vazio']">
		<xsl:sort select="@nome"/>
		<tr>
			<td class="codigo"> <xsl:value-of select="@código"/></td>
			<td class="disc"> <xsl:value-of select="@nome"/></td>
		</tr>
	</xsl:for-each>
	</table>
</body>
</html>

</xsl:template>
</xsl:stylesheet>