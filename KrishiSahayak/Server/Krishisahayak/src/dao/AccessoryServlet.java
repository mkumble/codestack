package dao;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/* This servlet receives data from mobile and sends it accessory . java*/
public class AccessoryServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;

	Accessory accessoryObject = new Accessory();
	String accessoryArray = null;

	protected void doGet(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {

		PrintWriter out = response.getWriter();
		String powerID = request.getParameter("powerID");
		try {
			accessoryArray = Accessory.getAccessoryid(powerID);

		} catch (Exception e) {

			e.printStackTrace();
		}

		out.print(accessoryArray);

	}

}
