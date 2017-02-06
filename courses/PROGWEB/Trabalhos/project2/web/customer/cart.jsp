<%@ include file="auth.jsp" %>
<h2>Carrinho de Compras</h2>

<table id="items">
	<tr>
		<th>Produto</th>
		<th>Qtd</th>
		<th>Preço</th>
	</tr>
	
	<!-- 
	editar quantidade: 
		aumentar/diminuir quantidade, anular remove da lista
	-->
	
	<tr>
		<td><a href="#" title="Produto 1">Produto 1</a></td>
		<td><a href="#" title="Editar">1</a></td>
		<td>1,99</td>
	</tr>
	
	<tr>
		<td><a href="#" title="Produto 2">Produto 2</a></td>
		<td><a href="#" title="Editar">1</a></td>
		<td>1,99</td>
	</tr>
	
	<tr>
		<td><a href="#" title="Produto 3">Produto 3</a></td>
		<td><a href="#" title="Editar">1</a></td>
		<td>1,99</td>
	</tr>
	
	<tr>
		<th id="total" colspan="2">Total</th>
		<td>5,97</td>
	</tr>
</table>
