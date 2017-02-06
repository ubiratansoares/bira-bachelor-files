<%@ include file="auth.jsp" %>
<%@ page import="ecommerce.util.*, ecommerce.model.*" %>
<%@ page import="org.hibernate.*, java.util.*" %>
<%@ page import="org.hibernate.criterion.*, org.hibernate.transform.*" %>

<%	String path = this.getServletContext().getContextPath(); 
	Session hs = HibernateUtil.getSession();
	String action = request.getParameter("action");
	if(action == null) action = "list";
	
	if(action.equals("list")) { 
		String sort = request.getParameter("sort"); 
		if(sort == null) sort = (String) session.getAttribute("list.sort");
		if(sort == null) sort = "recent";
		session.setAttribute("list.sort", sort);
		
		Integer cid = null;
		String category = request.getParameter("category");
		try {
			cid = (category == null)? 
					(Integer) session.getAttribute("list.category"): 
					Integer.valueOf(category);
		} catch(Exception e) {
		}
		session.setAttribute("list.category", cid); 
		
		Criteria criteria = hs.createCriteria(Product.class, "p")
				.add(Restrictions.eq("status", Product.Status.AVAILABLE)); 
		if(cid != null) {
			criteria.createCriteria("categories", "c")
					.add(Restrictions.eq("c.id", cid));
		} %>
		
		<div id="filters">
			<a <% if(sort.equals("popular")) out.print("class='selected'"); 
				%> href="main.jsp?view=offers&action=list&sort=popular">Mais Vendidos</a> |
			<a <% if(sort.equals("recent")) out.print("class='selected'"); 
				%> href="main.jsp?view=offers&action=list&sort=recent">Últimas Ofertas</a> |
			<a <% if(sort.equals("scarce")) out.print("class='selected'"); 
				%> href="main.jsp?view=offers&action=list&sort=scarce">Últimas Unidades</a>
		</div>

	<%	if(sort.equals("recent")) { %>
			<h2>Últimas Ofertas</h2>

		<%	criteria.addOrder(Order.desc("updated_on").ignoreCase());
			List<Product> list = Collections.checkedList(criteria.list(), Product.class); 
			for(Product p : list) { %>
			<div class="product">
				<img src="../images/dummy.jpg">
				<p class="price float-right"><%= HtmlUtil.currency(p.getBasePrice(), 
						"R$", request.getLocale()) %></p>
				<h3><a href="<%= path %>"><%= HtmlUtil.cdata(p.getHeading()) %></a></h3>
				<p class="counter"><%= HtmlUtil.cdata(p.getLastModificationTime(),
						"'atualizado em' dd-MM-yyyy HH:MM:ss", request.getLocale()) %></p>
				<p class="description"><%= HtmlUtil.cdata(p.getShortDescription()) %></p>
			</div>
		<%	}
		} else if(sort.equals("popular")) { %>
			<h2>Mais Vendidos</h2>

		<%	criteria.createCriteria("purchase_items", "pi", Criteria.LEFT_JOIN)
					.setProjection(Projections.projectionList()
					.add(Projections.sum("pi.amount"), "amountSold")
					.add(Projections.groupProperty("p.id"), "id")
					.add(Projections.groupProperty("p.name"), "name")
					.add(Projections.groupProperty("p.brand"), "brand")
					.add(Projections.groupProperty("p.short_description"), "shortDescription")
					.add(Projections.groupProperty("p.price"), "basePrice")
					.add(Projections.groupProperty("p.updated_on"), "lastModificationTime"))
					.addOrder(Order.desc("amountSold").ignoreCase())
					.setResultTransformer(new AliasToBeanResultTransformer(Product.class));
			List<Product> list = Collections.checkedList(criteria.list(), Product.class); 
			for(Product p : list) { %>
			<div class="product">
				<img src="../images/dummy.jpg">
				<p class="price float-right"><%= HtmlUtil.currency(p.getBasePrice(), 
						"R$", request.getLocale()) %></p>
				<h3><a href="<%= path %>"><%= HtmlUtil.cdata(p.getHeading()) %></a></h3>
				<p class="counter"><%= HtmlUtil.quantity(p.getAmountSold(), "", 
						"%d unidade vendida", "%d unidades vendidas", request.getLocale()) %></p>
				<p class="description"><%= HtmlUtil.cdata(p.getShortDescription()) %></p>
			</div>
		<%	}
		} else if(sort.equals("scarce")) { %>
			<h2>Últimas Unidades</h2>

		<%	criteria.addOrder(Order.desc("updated_on").ignoreCase());
			List<Product> list = Collections.checkedList(criteria.list(), Product.class); 
			for(Product p : list) { %>
			<div class="product">
				<img src="../images/dummy.jpg">
				<p class="price float-right"><%= HtmlUtil.currency(p.getBasePrice(), 
						"R$", request.getLocale()) %></p>
				<h3><a href="<%= path %>"><%= HtmlUtil.cdata(p.getHeading()) %></a></h3>
				<p class="counter"><%= HtmlUtil.cdata(p.getLastModificationTime(),
						"'atualizado em' dd-MM-yyyy HH:MM:ss", request.getLocale()) %></p>
				<p class="description"><%= HtmlUtil.cdata(p.getShortDescription()) %></p>
			</div>
		<%	}
		}
	} %>
