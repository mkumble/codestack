package dao;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONArray;

/*
 * This module takes  climate,irrigation,soil as input from Mobile and suggests suitable output
 * to Mobile
 * */
public class SuggestionServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;

	CropSuggestion cropSuggest = new CropSuggestion();
	JSONArray cropArray = new JSONArray();
	String crops = null;

	protected void doGet(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		PrintWriter out = response.getWriter();
		String climate = request.getParameter("climate");
		String irrigation = request.getParameter("irrigation");
		String soil = request.getParameter("soil");
		try {
			
			cropArray = cropSuggest.getSuggestion(climate, irrigation, soil);

		} catch (Exception e) {

			e.printStackTrace();
		}

		crops = cropArray.toString();
		
		out.println(crops);

	}

}
