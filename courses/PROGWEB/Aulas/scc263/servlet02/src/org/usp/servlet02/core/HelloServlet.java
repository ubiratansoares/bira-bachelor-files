package org.usp.servlet01.core;

import java.io.*;

import java.util.*;
import javax.servlet.http.*;
import javax.servlet.*;

public class HelloServlet extends HttpServlet {
  public void doGet (HttpServletRequest request, HttpServletResponse response)
	  throws ServletException, IOException
  {
	response.setContentType("text/html");
	Locale locale = response.getLocale();
    	PrintWriter out = response.getWriter();

	String cpath = request.getContextPath();
	String servletpath = request.getServletPath();
	HttpSession session = request.getSession();

    	out.println("Hello, world!<br>");
    	out.println("Locale: "+locale+"<br>");
    	out.println("Context Path: "+cpath+"<br>");
    	out.println("Servlet Path: "+servletpath+"<br>");
    	out.println("Session: "+session+"<br>");
    	out.close();
  }
}
