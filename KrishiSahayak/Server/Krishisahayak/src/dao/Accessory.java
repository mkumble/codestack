package dao;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;

import org.json.JSONObject;

import util.ConnectDB;

/*This Module  will fetch the accessory details from database based on the powerid that is scanned and sent from the servlet and packages in to JSON Object  and sends result back to servlet*/
public class Accessory {
	static Connection conn = null;

	static ResultSet rs = null;
	static Statement stmt = null;
	static String [] accessoryArray =new String[10];
	static String[] access_names={"a01trol","a02trol","a03trol","b02harr","b03harr","c01grad","c02grad"};
	static JSONObject accessories=null;
	
	public static String getAccessoryid(String powerID) throws Exception {

		accessories=new JSONObject();
		try {

			
		

			conn = ConnectDB.getConnection();

			stmt = conn.createStatement();

			String query = "select *from accessoryid where powerid like'"
					+ powerID + "'";

			rs = stmt.executeQuery(query);

			while (rs.next()) {
				String accessory1 = rs.getString("acc1");
				
				if(accessory1.equals(access_names[0])||accessory1.equals(access_names[1])||accessory1.equals(access_names[2])){
					accessories.put(accessory1,accessory1+".jpg");
					
				}
						
				                   
			    String accessory2 = rs.getString("acc2");
				
			    if(accessory2.equals(access_names[3]) || accessory2.equals(access_names[4]) ){
					accessories.put(accessory2,accessory2+".jpg");
					
				}
				
				String accessory3 = rs.getString("acc3");
				
				if(accessory3.equals(access_names[5]) || accessory3.equals(access_names[6]) ){
					accessories.put(accessory3,accessory3+".png");
					
				}	
			
				

			}

			return accessories.toString();
		}

		catch (Exception e) {
			throw e;
		}
	}
	


}
