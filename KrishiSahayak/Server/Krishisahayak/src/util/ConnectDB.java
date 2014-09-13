package util;

import java.sql.Connection;
import java.sql.SQLException;

import org.apache.commons.dbcp.BasicDataSource;

public class ConnectDB {

	static BasicDataSource bds = new BasicDataSource();
	static Connection con = null;

	public static Connection getConnection() throws SQLException

	{

	
		
		bds.setDriverClassName("com.mysql.jdbc.Driver");
		bds.setUrl("jdbc:mysql://localhost:3306/krishisahayak");
		bds.setUsername("root");
		bds.setPassword("");
		bds.setMaxActive(10);
		bds.setPoolPreparedStatements(true);

		return bds.getConnection();
	}

}
