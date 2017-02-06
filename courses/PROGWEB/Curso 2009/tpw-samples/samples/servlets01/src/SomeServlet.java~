import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class SomeServlet extends HttpServlet {

  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
      throws ServletException, IOException {
      
        response.setContentType("text/html");
    String str = request.getParameter("p1");

    PrintWriter out = response.getWriter();
    out.print("GET = "+str);
  }
  
  public void doPost(HttpServletRequest request,
                    HttpServletResponse response)
      throws ServletException, IOException {
      
        response.setContentType("text/html");
    String str = request.getParameter("p1");

    PrintWriter out = response.getWriter();
    out.print("POST = "+str);
  }

}
