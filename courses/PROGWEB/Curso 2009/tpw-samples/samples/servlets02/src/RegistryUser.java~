import java.io.*;
import java.util.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class RegistryUser extends HttpServlet {

    ResourceBundle rb = ResourceBundle.getBundle("LocalStrings");
    
    public void doGet(HttpServletRequest request,
                      HttpServletResponse response)
        throws IOException, ServletException
    {
        response.setContentType("text/html");

        PrintWriter out = response.getWriter();
        out.println("<html>");
        out.println("<head>");

        String title   = rb.getString("USER_REGISTRY");
        String name    = rb.getString("USER_NAME");
        String address = rb.getString("USER_ADDRESS");

        out.println("<title>" + title + "</title>");
        out.println("</head>");
        out.println("<body>");

        String dataName    = request.getParameter("name");
        String dataAddress = request.getParameter("address");

        if (dataName != null && dataAddress != null) {
            out.print(dataName+" "+dataAddress+"<br>");
        }

        out.print("<form action=\"");
	out.print(response.encodeURL("RegistryUser"));
        out.print("\" ");
        out.println("method=POST>");
        out.println(rb.getString("USER_NAME"));
        out.println("<input type=text size=20 name=name>");
        out.println("<br>");
        out.println(rb.getString("USER_ADDRESS"));
        out.println("<input type=text size=20 name=address>");
        out.println("<br>");
        out.println("<input type=submit>");
        out.println("</form>");

        out.println("<br><br>");

        out.print("<form action=\"");
	out.print(response.encodeURL("RegistryUser"));
        out.print("\" ");
        out.println("method=GET>");
        out.println(rb.getString("USER_NAME"));
        out.println("<input type=text size=20 name=name>");
        out.println("<br>");
        out.println(rb.getString("USER_ADDRESS"));
        out.println("<input type=text size=20 name=address>");
        out.println("<br>");
        out.println("<input type=submit>");
        out.println("</form>");

        out.println("</body>");
        out.println("</html>");
    }

    public void doPost(HttpServletRequest request,
                      HttpServletResponse response)
        throws IOException, ServletException
    {
        doGet(request, response);
    }

}
