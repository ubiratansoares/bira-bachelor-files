import tpw.agenda.database.*;
import java.sql.*;

public class Main {
	public static void main(String[] args) {
		try {
			Connection conn = ConnectionManager.getConnection("main");
			Statement st = conn.createStatement();
			ResultSet rs = st.executeQuery("select * from account");
			
			while(rs.next()) {
				System.out.println(rs.getString("email"));
			}
			
			conn.close();
		} catch(Exception e) {
			e.printStackTrace();
		}
		return;
	}
}