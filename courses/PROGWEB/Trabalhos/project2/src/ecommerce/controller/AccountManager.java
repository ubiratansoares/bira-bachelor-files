package ecommerce.controller;

import ecommerce.model.*;
import ecommerce.util.HibernateUtil;

import java.io.*;
import java.text.*;
import java.util.Date;
import javax.servlet.*;
import javax.servlet.http.*;

import org.hibernate.*;

public class AccountManager extends HttpServlet {
	
	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		String dest = "/customer/index.jsp";
		Session hs = HibernateUtil.getSession();
		hs.beginTransaction();
		try {
			request.setCharacterEncoding("UTF-8");
			HttpSession session = request.getSession();
			String action = request.getParameter("action");
			if(action == null) return;
			
			if(action.equals("gather")) {
				dest = this.gather(request);
			} else if(action.equals("save")) {
				dest = this.save(request);
			} else if(action.equals("update")) {
				dest = this.update(request);
			} else if(action.equals("delete")) {
				dest = this.delete(request);
			} else if(action.equals("clear")) {
				session.setAttribute("account", null);
				dest = "/customer/signup.jsp";
			}
		} finally {
			if(hs.isOpen()) hs.getTransaction().commit();
			response.sendRedirect(request.getContextPath() + dest);
		}
		return;
	}
	
	//reunir os dados (formulário em várias páginas)
	private String gather(HttpServletRequest request) {
		HttpSession session = request.getSession();
		
		boolean customer = true;
		Integer uid = (Integer) session.getAttribute("uid");
		if(uid != null) {
			Session hs = HibernateUtil.getSession();
			Account acc = (Account) hs.get(Account.class, uid);
			Class cl = Hibernate.getClass(acc); //bypass proxy
			customer = (acc == null) || Customer.class.isAssignableFrom(cl);
		}
		
		String form = request.getParameter("form");
		if(form == null) form = "account";
		if(customer) {
			
			//criar cliente
			Customer c = (Customer) session.getAttribute("account");
			if(c == null) c = new Customer();
			
			if(form.equals("account")) {
				try {
					c.setEmail(request.getParameter("email"));
					c.setPassword(request.getParameter("password"));
					c.setAcceptsTOS(request.getParameter("tos") != null);
					
					String retype = request.getParameter("retype");
					if(!c.getPassword().equals(retype) || !c.acceptsTOS())
						throw new IllegalArgumentException();
					return "/customer/signup.jsp?form=person";
				} catch(Exception e) {
					e.printStackTrace();
					return "/customer/signup.jsp?form=account";
				} finally {
					session.setAttribute("account", c);
				}
			} else if(form.equals("person")) {
				try {
					c.getName().setFirstName(request.getParameter("fname"));
					c.getName().setLastName(request.getParameter("lname"));
					c.getName().setMiddleNames(request.getParameter("mname"));
					c.getName().setNickname(request.getParameter("nname"));
					
					String day = request.getParameter("bday");
					String month = request.getParameter("bmon");
					String year = request.getParameter("byear");
					
					SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
					Date date = format.parse(String.format("%s-%s-%s", year, month, day));
					c.setBirthDate(date);
					return "/customer/signup.jsp?form=address";
				} catch(Exception e) {
					e.printStackTrace();
					return "/customer/signup.jsp?form=person";
				} finally {
					session.setAttribute("account", c);
				}
			} else if(form.equals("address")) {
				try {
					c.getAddress().setType(request.getParameter("type"));
					c.getAddress().setName(request.getParameter("street"));
					try {
						String n = request.getParameter("number");
						c.getAddress().setNumber(Integer.valueOf(n));
					} catch(NumberFormatException e) {
					}
					c.getAddress().setComplement(request.getParameter("complement"));
					
					c.getAddress().setDistrict(request.getParameter("district"));
					c.getAddress().setCity(request.getParameter("city"));
					c.getAddress().setState(request.getParameter("state"));
					c.getAddress().setCountry(request.getParameter("country"));
					c.getAddress().setPostalCode(request.getParameter("pcode"));
					
					return "/customer/signup.jsp?form=commit";
				} catch(Exception e) {
					e.printStackTrace();
					return "/customer/signup.jsp?form=address";
				} finally {
					session.setAttribute("account", c);
				}
			}
			
		}
		
		return "/index.jsp";
	}

	private String save(HttpServletRequest request) {
		HttpSession session = request.getSession();
		
		boolean customer = true;
		Integer uid = (Integer) session.getAttribute("uid");
		if(uid != null) {
			Session hs = HibernateUtil.getSession();
			Account acc = (Account) hs.get(Account.class, uid);
			Class cl = Hibernate.getClass(acc); //bypass proxy
			customer = (acc == null) || Customer.class.isAssignableFrom(cl);
		}
		
		if(customer) {
			try {
				Customer c = (Customer) session.getAttribute("account");
				if(c == null) return "/customer/signup.jsp?form=commit";
				session.setAttribute("account", null);
				c.setStatus(Account.Status.ACTIVE);
				c.setLocale(request.getLocale());
				HibernateUtil.getSession().save(c);
				return "/customer/signup.jsp?form=committed";
			} catch(HibernateException e) {
				return "/customer/signup.jsp?form=commit";
			}
		} else {
			Retailer r = (Retailer) session.getAttribute("account");
			try {
				if(r == null) r = new Retailer();
				
				//informações da conta
				r.setEmail(request.getParameter("email"));
				r.setPassword(request.getParameter("password"));
				String retype = request.getParameter("retype");
				if(!r.getPassword().equals(retype)) 
					throw new IllegalArgumentException();
					
				//informações pessoais
				r.getName().setFirstName(request.getParameter("fname"));
				r.getName().setLastName(request.getParameter("lname"));
				r.getName().setMiddleNames(request.getParameter("mname"));
				r.getName().setNickname(request.getParameter("nname"));
				r.setTitle(request.getParameter("title"));
				r.setDepartment(request.getParameter("dept"));
				
				String day = request.getParameter("bday");
				String month = request.getParameter("bmon");
				String year = request.getParameter("byear");
					
				SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
				Date date = format.parse(String.format("%s-%s-%s", year, month, day));
				r.setBirthDate(date);
				
				//endereço de residência
				r.getAddress().setType(request.getParameter("type"));
				r.getAddress().setName(request.getParameter("street"));
				try {
					String n = request.getParameter("number");
					r.getAddress().setNumber(Integer.valueOf(n));
				} catch(NumberFormatException e) {
				}
				r.getAddress().setComplement(request.getParameter("complement"));
				r.getAddress().setDistrict(request.getParameter("district"));
				r.getAddress().setCity(request.getParameter("city"));
				r.getAddress().setState(request.getParameter("state"));
				r.getAddress().setCountry(request.getParameter("country"));
				r.getAddress().setPostalCode(request.getParameter("pcode"));
				
				//outras informações
				r.setStatus(Account.Status.ACTIVE);
				r.setLocale(request.getLocale());
				
				HibernateUtil.getSession().save(r);
				session.setAttribute("account", null);
				return "/retailer/home.jsp?view=accounts&action=list&filter=retailers";
			} catch(Exception e) {
				e.printStackTrace();
				session.setAttribute("account", r);
				return "/retailer/home.jsp?view=accounts&action=create";
			}
		}
	}
	
	//atualiza uma conta (não usa a sessão)
	private String update(HttpServletRequest request) {
		HttpSession session = request.getSession();
		Session hs = HibernateUtil.getSession();
		
		boolean customer = true;
		Integer uid = (Integer) session.getAttribute("uid");
		if(uid != null) {
			Account acc = (Account) hs.get(Account.class, uid);
			if(acc == null) return "/index.jsp";
			Class cl = Hibernate.getClass(acc); //bypass proxy
			customer = Customer.class.isAssignableFrom(cl);
		} else 
			return "/index.jsp";
		
		String form = request.getParameter("form");
		if(customer) {
			if(form == null) return "/customer/manage.jsp?form=view";
			
			//carregar cliente
			Customer c = (Customer) hs.get(Customer.class, uid);
			if(c == null) return "/index.jsp";
			
			if(form.equals("account")) {
				try {
					String password = request.getParameter("password");
					if(!c.getPassword().equals(password))
						throw new IllegalArgumentException();
					
					c.setEmail(request.getParameter("email"));
					String newpwd = request.getParameter("newpwd");
					String retype = request.getParameter("retype");
					if(!newpwd.isEmpty() && newpwd.equals(retype))
						c.setPassword(newpwd);
				} catch(Exception e) {
					e.printStackTrace();
					hs.getTransaction().rollback();
					return "/customer/manage.jsp?form=account";
				}
			} else if(form.equals("person")) {
				try {
					c.getName().setFirstName(request.getParameter("fname"));
					c.getName().setLastName(request.getParameter("lname"));
					c.getName().setMiddleNames(request.getParameter("mname"));
					c.getName().setNickname(request.getParameter("nname"));
					
					String day = request.getParameter("bday");
					String month = request.getParameter("bmon");
					String year = request.getParameter("byear");
					
					SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
					Date date = format.parse(String.format("%s-%s-%s", year, month, day));
					c.setBirthDate(date);
				} catch(Exception e) {
					e.printStackTrace();
					hs.getTransaction().rollback();
					return "/customer/manage.jsp?form=person";
				}
			} else if(form.equals("address")) {
				try {
					c.getAddress().setType(request.getParameter("type"));
					c.getAddress().setName(request.getParameter("street"));
					try {
						String n = request.getParameter("number");
						c.getAddress().setNumber(Integer.valueOf(n));
					} catch(NumberFormatException e) {
					}
					c.getAddress().setComplement(request.getParameter("complement"));
					c.getAddress().setDistrict(request.getParameter("district"));
					c.getAddress().setCity(request.getParameter("city"));
					c.getAddress().setState(request.getParameter("state"));
					c.getAddress().setCountry(request.getParameter("country"));
					c.getAddress().setPostalCode(request.getParameter("pcode"));
				} catch(Exception e) {
					e.printStackTrace();
					hs.getTransaction().rollback();
					return "/customer/manage.jsp?form=address";
				}
			}
			
			c.setLastModificationTime();
			return "/customer/manage.jsp?form=view";
		} else if(request.getParameter("uid") != null) {
			try {
				Integer id = Integer.valueOf(request.getParameter("uid"));
				Account acc = (Account) hs.get(Account.class, id);
				Integer sid = Integer.valueOf(request.getParameter("status"));
				Account.Status st = Account.Status.class.getEnumConstants()[sid];
				acc.setStatus(st);
			} catch(Exception e) {
				e.printStackTrace();
				hs.getTransaction().rollback();
			}
			return "/retailer/home.jsp?view=accounts&action=view&uid=" + 
					request.getParameter("uid");
		} else {
			if(form == null) return "/retailer/account.jsp?form=view";
			
			//carregar vendedor
			Retailer r = (Retailer) hs.get(Retailer.class, uid);
			if(r == null) return "/index.jsp";
			
			if(form.equals("account")) {
				try { 
					String password = request.getParameter("password");
					if(!r.getPassword().equals(password))
						return "/retailer/account.jsp?form=account";
					
					r.setEmail(request.getParameter("email"));
					String newpwd = request.getParameter("newpwd");
					String retype = request.getParameter("retype");
					if(!newpwd.isEmpty() && newpwd.equals(retype))
						r.setPassword(newpwd);
				} catch(Exception e) {
					e.printStackTrace();
					hs.getTransaction().rollback();
					return "/retailer/account.jsp?form=account";
				}
			} else if(form.equals("person")) {
				try {
					r.getName().setFirstName(request.getParameter("fname"));
					r.getName().setLastName(request.getParameter("lname"));
					r.getName().setMiddleNames(request.getParameter("mname"));
					r.getName().setNickname(request.getParameter("nname"));
					r.setTitle(request.getParameter("title"));
					r.setDepartment(request.getParameter("dept"));
					
					String day = request.getParameter("bday");
					String month = request.getParameter("bmon");
					String year = request.getParameter("byear");
					
					SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
					Date date = format.parse(String.format("%s-%s-%s", year, month, day));
					r.setBirthDate(date);
				} catch(ParseException e) {
					e.printStackTrace();
					hs.getTransaction().rollback();
					return "/retailer/account.jsp?form=person";
				}
			} else if(form.equals("address")) {
				try {
					r.getAddress().setType(request.getParameter("type"));
					r.getAddress().setName(request.getParameter("street"));
					try {
						String n = request.getParameter("number");
						r.getAddress().setNumber(Integer.valueOf(n));
					} catch(NumberFormatException e) {
					}
					r.getAddress().setComplement(request.getParameter("complement"));
					
					r.getAddress().setDistrict(request.getParameter("district"));
					r.getAddress().setCity(request.getParameter("city"));
					r.getAddress().setState(request.getParameter("state"));
					r.getAddress().setCountry(request.getParameter("country"));
					r.getAddress().setPostalCode(request.getParameter("pcode"));
				} catch(Exception e) {
					e.printStackTrace();
					hs.getTransaction().rollback();
					return "/retailer/account.jsp?form=address";
				}
			}
			
			r.setLastModificationTime();
			return "/retailer/account.jsp?form=view";
		}
	}

	private String delete(HttpServletRequest request) {
		HttpSession session = request.getSession();
		Session hs = HibernateUtil.getSession();
		
		boolean customer = true;
		Integer uid = (Integer) session.getAttribute("uid");
		if(uid != null) {
			Account acc = (Account) hs.get(Account.class, uid);
			if(acc == null) return "/index.jsp";
			Class cl = Hibernate.getClass(acc); //bypass proxy
			customer = Customer.class.isAssignableFrom(cl);
		} else 
			return "/index.jsp";
		
		if(customer) {
			try {
				Customer c = (Customer) hs.get(Customer.class, uid);
				if(c == null) return "/index.jsp";
				
				String password = request.getParameter("password");
				if(!password.equals(c.getPassword()))
					return "/customer/manage.jsp?form=account";
				
				HibernateUtil.getSession().delete(c);
				session.invalidate();
				return "/index.jsp";
			} catch(HibernateException e) {
				return "/customer/manage.jsp?form=account";
			}
		} else {
			String view = null;
			try {
				Integer id = Integer.valueOf(request.getParameter("uid"));
				Account acc = (Account) hs.get(Account.class, id);
				if(acc == null) return "/retailer/home.jsp";
				
				//um vendedor não pode deletar a si mesmo
				//para não permitir excluir todas as contas
				if(acc.getId().equals(uid))
					return "/retailer/home.jsp";
				
				Class cl = Hibernate.getClass(acc); //bypass proxy
				customer = Customer.class.isAssignableFrom(cl);
				view = customer? "customers": "retailers";
				
				HibernateUtil.getSession().delete(acc);
				return "/retailer/home.jsp?view=accounts&action=list&filter=" + view;
			} catch(HibernateException e) {
				return "/retailer/home.jsp?view=accounts" + 
						((view != null)? "&action=list&filter=" + view: "");
			}
		}
	}
	
	private static final long serialVersionUID = 1L;
}
