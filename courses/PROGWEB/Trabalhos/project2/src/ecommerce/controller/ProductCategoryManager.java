package ecommerce.controller;

import ecommerce.model.*;
import ecommerce.util.HibernateUtil;

import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

import org.hibernate.*;

public class ProductCategoryManager extends HttpServlet {
	
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
				dest = "/retailer/index.jsp?view=categories&action=list";
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
			dest = "/retailer/home.jsp?view=categories&action=list";
		} finally {
			hs.getTransaction().commit();
			response.sendRedirect(request.getContextPath() + dest);
		}
	}
	
	private String save(HttpServletRequest request) {
		try {
			HibernateUtil.getSession().save(
					new ProductCategory(request.getParameter("name")));
		} catch(HibernateException e) {
			return "/retailer/home.jsp?view=categories&action=create";
		}
		return "/retailer/home.jsp?view=categories&action=list";
	}
	
	private String update(HttpServletRequest request) {
		try {
			Integer id = Integer.valueOf(request.getParameter("id"));
			ProductCategory c = (ProductCategory) HibernateUtil
					.getSession().get(ProductCategory.class, id);
			c.setName(request.getParameter("name"));
		} catch(HibernateException e) {
			return "/retailer/home.jsp?view=categories&action=view" + 
					request.getParameter("id");
		}
		return "/retailer/home.jsp?view=categories&action=list";
	}

	private String delete(HttpServletRequest request) {
		try {
			Session hs = HibernateUtil.getSession();
			Integer id = Integer.valueOf(request.getParameter("id"));
			ProductCategory c = (ProductCategory) 
					hs.get(ProductCategory.class, id);
			hs.delete(c);
		} catch(HibernateException e) {
			return "/retailer/home.jsp?view=categories&action=view" + 
					request.getParameter("id");
		}
		return "/retailer/home.jsp?view=categories&action=list";
	}
	
	private static final long serialVersionUID = 1L;
}
