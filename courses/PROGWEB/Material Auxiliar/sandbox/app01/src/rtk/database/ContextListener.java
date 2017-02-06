package rtk.database;

import java.io.*;
import javax.servlet.*;
import javax.xml.stream.*;

public class ContextListener implements ServletContextListener {
	
	public ContextListener() {
	}
	
	@Override
	public void contextInitialized(ServletContextEvent ev) {
		try {
			String configFile = ev.getServletContext().getRealPath("/WEB-INF/db-config.xml");
			ConnectionManager.getInstance().open(configFile);
			for(String msg : ConnectionManager.getInstance().getMessages())
				System.out.println(msg);
		} catch(XMLStreamException e) {
			System.err.println("ERROR: Failed to parse the database configuration file.");
		} catch(FileNotFoundException e) {
			System.err.println("ERROR: Database configuration file not found.");
		} catch(IOException e) {
			System.err.println("ERROR: Failed to read the database configuration file.");
		}
	}
	
	@Override
	public void contextDestroyed(ServletContextEvent ev) {
		ConnectionManager.getInstance().close();
	}
	
}
