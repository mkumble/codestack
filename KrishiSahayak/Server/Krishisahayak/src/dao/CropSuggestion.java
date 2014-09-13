package dao;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;

import org.json.JSONArray;

import util.ConnectDB;

/*
 * This module takes  climate,irrigation,soil as input from servlet and suggests suitable output
 * */
public class CropSuggestion {

	static ResultSet rs = null;
	static Statement stmt = null;
	
	Connection conn = null;

	public JSONArray getSuggestion(String climate, String irrigation,
			String soil) throws Exception {
		try {
		
			 JSONArray suggestionArray = new JSONArray();
			 
			conn = ConnectDB.getConnection();

			stmt = conn.createStatement();

			String query = "select T.name as nameCrop from crop as T, enviornment as U where T.no=U.no and U.climate like'"
					+ climate
					+ "' and irrigation like '"
					+ irrigation
					+ "' and soil like '" + soil + "';";

			rs = stmt.executeQuery(query);

			while (rs.next()) {
				String cropname = rs.getString("nameCrop");

			
				suggestionArray.put(cropname);

			}
			return suggestionArray;
		}
		
		catch (Exception e) {
			throw e;
		} finally {
			conn.close();
		}
	}

	
}
