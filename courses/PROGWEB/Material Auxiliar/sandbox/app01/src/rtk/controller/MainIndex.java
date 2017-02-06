package rtk.controller;

import rtk.database.*;
import rtk.template.*;
import rtk.model.*;
import rtk.view.*;

import java.io.*;
import java.sql.*;
import java.util.*;
import javax.servlet.*;
import javax.servlet.http.*;

/**
 * Página principal do site.
 */
public class MainIndex extends HttpServlet {
	
	@Override
	public void doGet(HttpServletRequest request, HttpServletResponse response) 
			throws ServletException, IOException {
		//construção da página
		HtmlPage page = new HtmlPage(request.getLocale());
		HomePage home = new HomePage(request.getLocale());
		HeadFragment head = new HeadFragment();
		
		page.setDoctype(HtmlPage.Type.STRICT);
		page.setTitle("E-Commerce");
		page.setHeadContent(head);
		page.setBodyContent(home);
		
		//definição dos cabeçalhos
		Map<String, String> headers = page.getHeaders();
		for(String name : headers.keySet())
			response.setHeader(name, headers.get(name));
		
		//servindo o conteúdo
		PrintWriter out = response.getWriter();
		out.println(page.get());
		out.flush();
	}
	
	public static void main(String[] args) {
		Locale locale = new Locale("pt", "BR");
		
		//construção da página
		HtmlPage page = new HtmlPage(locale);
		HomePage home = new HomePage(locale);
		HeadFragment head = new HeadFragment();
		
		page.setDoctype(HtmlPage.Type.STRICT);
		page.setTitle("E-Commerce");
		page.setHeadContent(head);
		page.setBodyContent(home);
		
		//definição dos cabeçalhos
		System.out.println(page.getHeaders());
		
		//servindo o conteúdo
		System.out.println(page.get());
	}
	
	private static final long serialVersionUID = 1L;
	
}
