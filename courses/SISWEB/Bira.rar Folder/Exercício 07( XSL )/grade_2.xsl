<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">

<html>
<head>
	<link rel="stylesheet" type="text/css" href="format.css" />		
	<style TYPE = "text/css"> </style>
	<title> Grade 2 </title>
</head>
<body>
	<p> <b> Nome: </b> <xsl:value-of select="inicio/name"/> </p>
	<p> <b> Nº USP: </b> <xsl:value-of select="inicio/nro_usp"/> </p>
	<p> <b> e-mail: </b> <xsl:value-of select="inicio/e-mail"/> </p>
	<table class="lista" border="3" align="center">
		<thead>
			<tr class="topo">	
				<th>Dia da semana</th>
				<th>7:20 as 10:00</th>
				<th>10:10 as 11:50</th>
				<th>19:00 as 20:40</th>
				<th>21:00 as 22:40</th>
			</tr>		
		</thead>
	
		<tr class="topo">
			<td>Segunda</td>
			<xsl:for-each select="inicio/gradehoraria/linha/aula[@taghelp='1']">
				<td><xsl:value-of select="@nome" /></td>
			</xsl:for-each>
		</tr> 	
	
	    <tr class="topo">
			<td>Terca</td>
			<xsl:for-each select="inicio/gradehoraria/linha/aula[@taghelp='2']">
				<td><xsl:value-of select="@nome" /></td>
			</xsl:for-each>
		</tr>
		
		<tr class="topo">
			<td>Quarta</td>
			<xsl:for-each select="inicio/gradehoraria/linha/aula[@taghelp='3']">
				<td><xsl:value-of select="@nome" /></td>
			</xsl:for-each>
		</tr>
				
		<tr class="topo">
			<td>Quinta</td>
			<xsl:for-each select="inicio/gradehoraria/linha/aula[@taghelp='4']">
				<td><xsl:value-of select="@nome" /></td>
			</xsl:for-each>
		</tr>
		
		<tr class="topo">
			<td>Sexta</td>
			<xsl:for-each select="inicio/gradehoraria/linha/aula[@taghelp='5']">
				<td><xsl:value-of select="@nome" /></td>
			</xsl:for-each>
		</tr>
	
	</table>
</body>
</html>

</xsl:template>
</xsl:stylesheet>