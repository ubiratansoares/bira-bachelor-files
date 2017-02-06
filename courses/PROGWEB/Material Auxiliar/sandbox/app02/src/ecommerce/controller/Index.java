package ecommerce.controller;

import ecommerce.template.*;
import ecommerce.view.*;

import java.io.*;
import java.net.URI;
import java.util.*;

import javax.servlet.*;
import javax.servlet.http.*;

public class Index extends HttpServlet {
	
	public void doGet(HttpServletRequest request, HttpServletResponse response) 
			throws ServletException, IOException {
		try {
			Locale locale = request.getLocale();
			HtmlPage page = new HtmlPage(locale, HtmlPage.Type.STRICT);
			page.setTitle("E-Commerce - Início");
			page.setBase(new URI(request.getContextPath()));
			
			HeadFragment head = new HeadFragment();
			head.addStyleSheet(new URI("global.css"));
			head.addStyleSheet(new URI("index.css"));
			page.setHeadContent(head);
			
			Base base = new Base(locale);
			base.getHeader().displaySignUpLink(true);
			page.setBodyContent(base);
			
			Offers recent = new Offers(locale);
			recent.setTitle("Últimas Ofertas");
			for(int i = 0; i < 4; i++) {
				Offers.Entry e = recent.addOffer("Produto " + (i + 1), new URI(""), 1.99);
				e.setImage(new URI("dummy.jpg"));
				e.setCounter("atualizado agora");
			}
			base.setLeftContent(recent);
			
			Offers popular = new Offers(locale);
			popular.setTitle("Mais Vendidos");
			for(int i = 0; i < 4; i++) {
				Offers.Entry e = popular.addOffer("Produto " + (i + 1), new URI(""), 1.99);
				e.setImage(new URI("dummy.jpg"));
				e.setCounter("0 unidade vendida");
			}
			base.setMainContent(popular);
			
			Offers scarce = new Offers(locale);
			scarce.setTitle("Últimas Unidades");
			for(int i = 0; i < 4; i++) {
				Offers.Entry e = scarce.addOffer("Produto " + (i + 1), new URI(""), 1.99);
				e.setImage(new URI("dummy.jpg"));
				e.setCounter("1 unidade restante");
			}
			base.setRightContent(scarce);
			
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
