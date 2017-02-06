package ecommerce.controller;

import ecommerce.template.*;
import ecommerce.model.Customer;
import ecommerce.view.*;
import ecommerce.view.customer.*;

import java.io.*;
import java.net.URI;
import java.util.*;

import javax.servlet.*;
import javax.servlet.http.*;

import static ecommerce.view.customer.AccountPage.View.*;

public class AccountManager extends HttpServlet {
	
	public void doGet(HttpServletRequest request, HttpServletResponse response) 
			throws ServletException, IOException {
		try {
			Locale locale = request.getLocale();
			HtmlPage page = new HtmlPage(locale, HtmlPage.Type.STRICT);
			page.setTitle("E-Commerce - Criar nova conta");
			page.setBase(new URI(request.getContextPath()));
			
			HeadFragment head = new HeadFragment();
			head.addStyleSheet(new URI("global.css"));
			head.addStyleSheet(new URI("customer/signup.css"));
			page.setHeadContent(head);
			
			Base base = new Base(locale);
			base.getHeader().displaySignUpLink(false);
			page.setBodyContent(base);
			
			base.setLeftContent(new CreateCustomerMenu(locale));
			
			HttpSession session = request.getSession();
			Customer customer = (Customer) session.getAttribute("customer.new");
			
			AccountPage.View index = CreateCustomerMenu.parseOption(
					request.getParameter("page"));
			if(index == null) {
				index = ACCOUNT_INFO_EDIT;
				customer = new Customer();
			}
			base.setMainContent(new AccountPage(locale, customer, index));
			
			Map<String, String> headers = page.getHeaders();
			for(String key : headers.keySet())
				response.setHeader(key, headers.get(key));
			
			response.getWriter().println(page.get());
		} catch(Exception e) {
			throw new ServletException(e);
		}
		return;
	}
	
	public void doPost(HttpServletRequest request, HttpServletResponse response) 
			throws ServletException, IOException {
		try {
			Locale locale = request.getLocale();
			HtmlPage page = new HtmlPage(locale, HtmlPage.Type.STRICT);
			page.setTitle("E-Commerce - Criar nova conta");
			page.setBase(new URI(request.getContextPath()));
			
			HeadFragment head = new HeadFragment();
			head.addStyleSheet(new URI("global.css"));
			head.addStyleSheet(new URI("customer/signup.css"));
			page.setHeadContent(head);
			
			Base base = new Base(locale);
			base.getHeader().displaySignUpLink(false);
			page.setBodyContent(base);
			
			base.setLeftContent(new CreateCustomerMenu(locale));
			
			HttpSession session = request.getSession();
			Customer customer = (Customer) session.getAttribute("customer.new");
			if(customer == null) customer = new Customer();
			
			//conteúdo principal
			AccountPage.View index = CreateCustomerMenu
					.parseOption(request.getParameter("page"));
			if(index == null) {
				index = ("saved".equals(request.getParameter("page")))?
						NEW_ACCOUNT_SAVED: ACCOUNT_INFO_EDIT;
				customer = new Customer();
			}
			
			//salvando campos
			if(request.getParameter("form_id") == null) {
			} else {
				if(request.getParameter("form_id").equals("account")) {
					customer.setEmail(request.getParameter("email"));
					customer.setPassword(request.getParameter("password"));
					customer.setAcceptsTOS(request.getParameter("tos") != null);
				} else if(request.getParameter("form_id").equals("person")) {
					customer.getName().setFirstName(request.getParameter("fname"));
					customer.getName().setLastName(request.getParameter("lname"));
					customer.getName().setMiddleNames(request.getParameter("mnames"));
					customer.getName().setNickname(request.getParameter("nname"));
				} else if(request.getParameter("form_id").equals("address")) {
					customer.getAddress().setType(request.getParameter("type"));
					customer.getAddress().setName(request.getParameter("street"));
					customer.getAddress().setNumber(Integer.valueOf(request.getParameter("number")));
					customer.getAddress().setComplement(request.getParameter("complement"));
					customer.getAddress().setCity(request.getParameter("city"));
					customer.getAddress().setDistrict(request.getParameter("district"));
					customer.getAddress().setState(request.getParameter("state"));
					customer.getAddress().setCountry(request.getParameter("country"));
					customer.getAddress().setPostalCode(request.getParameter("pcode"));
				}
			}
			base.setMainContent(new AccountPage(locale, customer, index));
			session.setAttribute("customer.new", customer);
			
			Map<String, String> headers = page.getHeaders();
			for(String key : headers.keySet())
				response.setHeader(key, headers.get(key));
			
			response.getWriter().println(page.get());
		} catch(Exception e) {
			throw new ServletException(e);
		}
		return;
	}
	
	private static final long serialVersionUID = 1L;
	
}
