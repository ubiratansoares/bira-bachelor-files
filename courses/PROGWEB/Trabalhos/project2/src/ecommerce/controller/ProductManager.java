package ecommerce.controller;

import ecommerce.model.*;
import ecommerce.util.HibernateUtil;

import java.io.*;
import java.text.*;
import java.util.*;

import javax.servlet.*;
import javax.servlet.http.*;

import org.hibernate.*;

public class ProductManager extends HttpServlet {
	
	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		String dest = "/index.jsp";
		Session hs = HibernateUtil.getSession();
		hs.beginTransaction();
		try {
			request.setCharacterEncoding("UTF-8");
			HttpSession session = request.getSession();
			Integer uid = (Integer) session.getAttribute("uid");
			if(uid == null) return;
			
			Account acc = (Account) hs.load(Account.class, uid);
			if(acc == null) return;
			
			Class cl = Hibernate.getClass(acc);
			if(!Retailer.class.isAssignableFrom(cl)) return;
			
			String action = request.getParameter("action");
			if(action == null) {
				dest = "/retailer/index.jsp?view=products&action=list";
				return;
			}
			
			if(action.equals("save")) {
				dest = this.save(request);
			} else if(action.equals("update")) {
				dest = this.update(request);
			} else if(action.equals("delete")) {
				dest = this.delete(request);
			}
		} catch(Exception e) {
			dest = "/retailer/home.jsp?view=products&action=list";
		} finally {
			if(hs.isOpen()) hs.getTransaction().commit();
			response.sendRedirect(request.getContextPath() + dest);
		}
	}
	
	private String save(HttpServletRequest request) {
		Integer id = null;
		HttpSession session = request.getSession();
		Session hs = HibernateUtil.getSession();
		Product p = (Product) session.getAttribute("product");
		if(p == null) p = new Product();
		
		try {
			p.setName(request.getParameter("name"));
			p.setBrand(request.getParameter("brand"));
			p.setModel(request.getParameter("model"));
			p.setVersion(request.getParameter("version"));
			p.setManufacturer(request.getParameter("manufacturer"));
			p.setShortDescription(request.getParameter("sdesc"));
			p.setLongDescription(request.getParameter("ldesc"));
			
			String day = request.getParameter("day");
			String month = request.getParameter("month");
			String year = request.getParameter("year");
			String hour = request.getParameter("hour");
			String minute = request.getParameter("minute");
			
			SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm");
			Date date = df.parse(String.format("%s-%s-%s %s:%s", 
					year, month, day, hour, minute));
			p.setReleaseTime(date);
			
			NumberFormat nf = NumberFormat.getNumberInstance(request.getLocale());
			p.setBasePrice(nf.parse(request.getParameter("price")).doubleValue());
			
			p.getCategories().clear();
			String[] cids = request.getParameterValues("category");
			if(cids != null) {
				for(String cid : cids) {
					try {
						ProductCategory c = (ProductCategory) hs.get(
								ProductCategory.class, Integer.valueOf(cid));
						if(c != null) p.getCategories().add(c);
					} catch(Exception e) {
					}
				}
			}
			
			p.setStatus((new Date().after(p.getReleaseTime()))? 
					Product.Status.AVAILABLE: Product.Status.NOT_RELEASED);
			p.setLastModificationTime();
			
			id = (Integer) hs.save(p);
			session.setAttribute("product", null);
			return "/retailer/home.jsp?view=products&action=view&id=" + id;
		} catch(Exception e) {
			e.printStackTrace();
			session.setAttribute("product", p);
			return "/retailer/home.jsp?view=products&action=create";
		}
	}
	
	private String update(HttpServletRequest request) {
		Integer id = null;
		try {
			System.out.println(request.getParameter("name"));
			System.out.println(request.getCharacterEncoding());
			
			id = Integer.valueOf(request.getParameter("id"));
			Product p = (Product) HibernateUtil.getSession()
					.get(Product.class, id);
			
			p.setName(request.getParameter("name"));
			p.setBrand(request.getParameter("brand"));
			p.setModel(request.getParameter("model"));
			p.setVersion(request.getParameter("version"));
			p.setManufacturer(request.getParameter("manufacturer"));
			
			p.setShortDescription(request.getParameter("sdesc"));
			p.setLongDescription(request.getParameter("ldesc"));
			
			String day = request.getParameter("day");
			String month = request.getParameter("month");
			String year = request.getParameter("year");
			String hour = request.getParameter("hour");
			String minute = request.getParameter("minute");
			
			SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm");
			Date date = df.parse(String.format("%s-%s-%s %s:%s", 
					year, month, day, hour, minute));
			p.setReleaseTime(date);
			
			NumberFormat nf = NumberFormat.getNumberInstance(request.getLocale());
			p.setBasePrice(nf.parse(request.getParameter("price")).doubleValue());
			
			Integer sid = Integer.valueOf(request.getParameter("status"));
			Product.Status st = Product.Status.class.getEnumConstants()[sid];
			p.setStatus((new Date().after(p.getReleaseTime()))? 
					st: Product.Status.NOT_RELEASED);
			
			p.getCategories().clear();
			String[] cids = request.getParameterValues("category");
			if(cids != null) {
				for(String cid : cids) {
					ProductCategory c = (ProductCategory) HibernateUtil.getSession()
							.get(ProductCategory.class, Integer.valueOf(cid));
					if(c != null) p.getCategories().add(c);
				}
			}
			
			p.setLastModificationTime();
			return "/retailer/home.jsp?view=products&action=view&id=" + id;
		} catch(Exception e) {
			e.printStackTrace();
			HibernateUtil.getSession().getTransaction().rollback();
			return "/retailer/home.jsp?view=products&action=edit&id=" + 
					request.getParameter("id");
		}
	}

	private String delete(HttpServletRequest request) {
		try {
			Session hs = HibernateUtil.getSession();
			Integer id = Integer.valueOf(request.getParameter("id"));
			Product p = (Product) hs.get(Product.class, id);
			hs.delete(p);
		} catch(HibernateException e) {
			return "/retailer/home.jsp?view=products&action=view" + 
					request.getParameter("id");
		}
		return "/retailer/home.jsp?view=products&action=list";
	}
	
	private static final long serialVersionUID = 1L;
}
