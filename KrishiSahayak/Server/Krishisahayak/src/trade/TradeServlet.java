package trade;

import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.annotation.WebServlet;
import java.io.PrintWriter;

/**
 * Servlet implementation class TradeSevlet
 */
@WebServlet(name = "TradeSevlet", urlPatterns = { "/TradeSevlet" })
public class TradeServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
	private String REGISTER_ACTION="Register.do";
	private String PRODUCT_ACTION="Product.do";
	private String TRANSCATION_ACTION="Transcation.do";
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public TradeServlet() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		
		// TODO Auto-generated method stub
		PrintWriter writer=response.getWriter();
		String url=request.getRequestURI();
		if(url.contains(REGISTER_ACTION))
		{
			String id=request.getParameter("id");
			String region=request.getParameter("region");
			String mobile=request.getParameter("mobile");
			FarmerRegister register=new FarmerRegister();
			register.setId(id);
			register.setRegion(region);
			register.setMobile(mobile);
			
			
		}
		else if(url.contains(PRODUCT_ACTION))
		{
			
		}
		else if(url.contains(TRANSCATION_ACTION))
		{
			
		}
		
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		processRequest(request, response);
		// TODO Auto-generated method stub
		
	}

	/**
	 * Processes requests for both HTTP <code>GET</code> and <code>POST</code> methods.
	 * @param request servlet request
	 * @param response servlet response
	 * @throws ServletException if a servlet-specific error occurs
	 * @throws IOException if an I/O error occurs
	 */
	protected void processRequest(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		response.setContentType("text/html;charset=UTF-8");
		PrintWriter out = response.getWriter();
		try {
			out.println("<html>");
			out.println("<head>");
			out.println("<title>Servlet TradeSevlet</title>");
			out.println("</head>");
			out.println("<body>");
			out.println("<h1>Servlet TradeSevlet at "
					+ request.getContextPath() + "</h1>");
			out.println("</body>");
			out.println("</html>");
		} finally {
			out.close();
		}
	}

	/**
	 * Returns a short description of the servlet.
	 * @return a String containing servlet description
	 */
	@Override
	public String getServletInfo() {
		return "Short description";
	}

}
