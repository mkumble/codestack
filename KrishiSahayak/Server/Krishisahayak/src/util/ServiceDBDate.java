package util;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class ServiceDBDate {

	static Connection conn = null;
	
	static ResultSet rs = null;
	static Statement stmt = null;
	static String dateDB = null;

	public static String lastDate(String modelname) throws SQLException {
		try {

			
			conn =ConnectDB.getConnection();
			stmt = conn.createStatement();

			String query = "Select purchaseDate from   maintainence where model like '"
					+ modelname + "'";

			rs = stmt.executeQuery(query);

			while (rs.next()) {
				dateDB = rs.getString("purchaseDate");
				//System.out.println(dateDB);

			}

		} catch (Exception e) {
			e.printStackTrace();
		}

		finally {
			if (stmt != null)
				stmt.close();
			if (rs != null)
				rs.close();
			if (conn != null)
				conn.close();

		}

		return dateDB;
	}


}
