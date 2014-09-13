package price;

import org.json.JSONObject;
import java.sql.ResultSet;
import java.sql.Statement;
import java.sql.Connection;
import java.sql.DriverManager;
import com.mysql.jdbc.Driver;

public class GetCropPrice {
	
	Connection conn = null;
	String url = "jdbc:mysql://localhost:3306/";
	String dbName = "krishisahayak";
	String driver = "com.mysql.jdbc.Driver";
	String userName = "root";
	String password = "";
	ResultSet rs = null;
	Statement stmt = null;
	String crop_name=null;
	String crop_price=null;
	int crop_trend;
	JSONObject price=null;
	
	
	public String getPrice(){
		
		try {
			Class.forName(driver).newInstance();
			conn = DriverManager.getConnection(url + dbName, userName, password);
			
			stmt = conn.createStatement();
            price=new JSONObject(); 
			String query = "select * from  cropprice";

			rs = stmt.executeQuery(query);
            
			while (rs.next()) {
				crop_name=rs.getString(1);
				JSONObject crop=new JSONObject();
				crop_price=rs.getString(2);
				crop_trend=Integer.parseInt(rs.getString(3));
				crop.put("price",crop_price);
				crop.put("trend",crop_trend);
										
				price.put(crop_name,crop);
			}
            
			

		} catch (Exception e) {
			e.printStackTrace();
			
		}
		return price.toString();
			
	}

}
