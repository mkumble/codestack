package dao;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class ServletServiceDate extends HttpServlet {
	ServiceDate serviceDays=new ServiceDate();
	
	private static final long serialVersionUID = 1L;

	protected void doGet(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {

		String model=request.getParameter("model");
		PrintWriter out = response.getWriter();
	
		

			String days =serviceDays.dueDate(model);

			out.print(days);
		

	}
}