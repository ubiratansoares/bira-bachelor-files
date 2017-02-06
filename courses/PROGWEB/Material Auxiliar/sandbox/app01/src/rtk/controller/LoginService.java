package rtk.controller;

import rtk.database.*;
import rtk.model.*;
import rtk.view.*;

import java.io.*;
import java.sql.*;
import java.util.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class LoginService extends HttpServlet {
	
	@Override
	public void doGet(HttpServletRequest request, HttpServletResponse response) 
			throws ServletException, IOException {
		Login page = new Login(request.getLocale());
		Map<String, String> headers = page.getHeaders();
		for(String name : headers.keySet())
			response.setHeader(name, headers.get(name));
		
		PrintWriter out = response.getWriter();
		out.println(page.get());
		out.flush();
	}
	
	@Override
	public void doPost(HttpServletRequest request, HttpServletResponse response) 
			throws ServletException, IOException {
		try {
			Dummy page = new Dummy(this.getServletContext().getResource(".").toURI());
			Map<String, String> headers = page.getHeaders();
			for(String name : headers.keySet())
				response.setHeader(name, headers.get(name));
			
			Connection conn = ConnectionManager.getInstance().getConnection("main");
			UserAuthentication auth = new UserAuthentication();
			auth.setEmail(request.getParameter("email"));
			auth.setPassword(request.getParameter("password"));
			boolean ok = auth.authenticate(conn);
			conn.close();
			
			page.setEmail(request.getParameter("email"));
			page.setStatus(ok? "ok": "nok");
			
			PrintWriter out = response.getWriter();
			out.println(page.get("user"));
			out.flush();
		} catch(Exception e) {
			throw new ServletException(e);
		}
	}
	
	private static final long serialVersionUID = 1L;
	
}
