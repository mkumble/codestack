package dao;

import java.io.IOException;
import org.json.*;

import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
/*
 * This takes hourMeter reading as inputfrom mobile and tells the health based on our assumption values from maintainence .java
 * 
 * */
public class MaintainenceServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;

	protected void doGet(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {

		JSONObject servletObject = new JSONObject();
		PrintWriter out = response.getWriter();

		String hourMeter = request.getParameter("hourMeterFromVehicle");
	//String hourMeter="120.0";

		double hourMeterDouble = Double.parseDouble(hourMeter);
		double gearOil = Maintainence.gearOilChange(hourMeterDouble);
		double oil = Maintainence.oilChange(hourMeterDouble);
		double maintain = Maintainence.maintainence(hourMeterDouble);

		try {
			servletObject.put("gearOilkey", gearOil);
			servletObject.put("oilKey", oil);
			servletObject.put("maintainKey", maintain);
		} catch (JSONException e) {

			e.printStackTrace();
		}

		String ressponsetoString = servletObject.toString();
		out.print(ressponsetoString);

	}

}
