<%@ include file="auth.jsp" %>
<%@ page import="ecommerce.util.*, ecommerce.model.*" %>
<%@ page import="org.hibernate.*, org.hibernate.criterion.*, java.util.*" %>

<!-- atualizar contadores regularmente por Ajax -->

<%	String path = this.getServletContext().getContextPath(); 
	String area = request.getParameter("page_area");
	Session hs = HibernateUtil.getSession();
	if("left".equals(area)) { %>
<h2>Últimas Ofertas</h2>

	<%	Criteria criteria = hs.createCriteria(Product.class)
					.addOrder(Order.desc("updated_on").ignoreCase())
					.add(Restrictions.eq("status", Product.Status.AVAILABLE));
		List<Product> list = Collections.checkedList(
				criteria.setMaxResults(4).list(), Product.class); 
		for(Product p : list) { %>
<div class="product">
	<img src="../images/dummy.jpg">
	<h3><a href="<%= path %>"><%= HtmlUtil.cdata(p.getHeading()) %></a></h3>
	<p class="counter"><%= HtmlUtil.cdata(p.getLastModificationTime(),
			"'atualizado em' dd-MM-yyyy HH:MM:ss", request.getLocale()) %></p>
	<p class="price"><%= HtmlUtil.currency(p.getBasePrice(), "R$", request.getLocale()) %></p>
</div>
<%		}



	} else if("main".equals(area)) { %>
<h2>Mais Vendidos</h2>

<div class="product">
	<img src="../images/dummy.jpg">
	<h3><a href="#">Produto 1</a></h3>
	<p class="counter">1.000.000 unidades vendidas</p>
	<p class="price">por R$1200,00</p>
</div>

<div class="product">
	<img src="../images/dummy.jpg">
	<h3><a href="#">Produto 2</a></h3>
	<p class="counter">500.000 unidades vendidas</p>
	<p class="price-cut">de R$2,00</p>
	<p class="price">para R$1,90</p>
	<p class="discount">10% de desconto!</p>
	<p class="validity">oferta válida até 01/01/1970 00:00</p>
</div>

<div class="product">
	<img src="../images/dummy.jpg">
	<h3><a href="#">Produto 3</a></h3>
	<p class="counter">1 unidade vendida</p>
	<p class="price-cut">de R$2,00</p>
	<p class="price">para R$1,90</p>
	<p class="discount">10% de desconto!</p>
	<p class="validity">oferta válida até 01/01/1970 00:00</p>
</div>

<div class="product">
	<img src="../images/dummy.jpg">
	<h3><a href="#">Produto 4</a></h3>
	<p class="counter">0 unidade vendida</p>
	<p class="price">por R$1200,00</p>
</div>



<% } else if("right".equals(area)) { %>
<h2>Últimas Unidades</h2>

<div class="product">
	<img src="../images/dummy.jpg">
	<h3><a href="#">Produto 1</a></h3>
	<p class="counter">1 unidade restante</p>
	<p class="price">por R$1200,00</p>
</div>

<div class="product">
	<img src="../images/dummy.jpg">
	<h3><a href="#">Produto 2</a></h3>
	<p class="counter">3 unidades restantes</p>
	<p class="price">por R$1200,00</p>
</div>

<div class="product">
	<img src="../images/dummy.jpg">
	<h3><a href="#">Produto 3</a></h3>
	<p class="counter">5 unidades restantes</p>
	<p class="price">por R$1200,00</p>
</div>

<div class="product">
	<img src="../images/dummy.jpg">
	<h3><a href="#">Produto 4</a></h3>
	<p class="counter">5 unidades restantes</p>
	<p class="price-cut">de R$2,00</p>
	<p class="price">para R$1,90</p>
	<p class="discount">10% de desconto!</p>
	<p class="validity">oferta válida até 01/01/1970 00:00</p>
</div>
<% } %>
