package dao;

import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.Calendar;

import util.ServiceDBDate;

public class ServiceDate {
	static Calendar cal1 = Calendar.getInstance();
	static Calendar cal2 = Calendar.getInstance();
	static long diffDays = 0;
	static String daysRemaning = null;

	public String dueDate(String model) {
		try {
			Calendar currentDate = Calendar.getInstance();
			SimpleDateFormat formatter = new SimpleDateFormat("yyyy/MM/dd");
			String dateNow = formatter.format(currentDate.getTime());

			String splitDate[] = dateNow.split("/");

			int yearNow = Integer.parseInt(splitDate[0]);

			int monthNow = Integer.parseInt(splitDate[1]);

			int dayNow = Integer.parseInt(splitDate[2]);

			cal1.set(yearNow, monthNow, dayNow);

			String dateDB = ServiceDBDate.lastDate(model);

			String splitDBDate[] = dateDB.split("-");

			int yearDB = Integer.parseInt(splitDBDate[0]);

			int monthDB = Integer.parseInt(splitDBDate[1]);

			int dayDB = Integer.parseInt(splitDBDate[2]);

			cal2.set(yearDB, monthDB, dayDB);

			long millisNow = cal1.getTimeInMillis();

			long millisDB = cal2.getTimeInMillis();

			long diff = millisNow - millisDB;

			diffDays = diff / (24 * 60 * 60 * 1000);

			long diff180 = 180 - (diffDays % 180);

			daysRemaning = String.valueOf(diff180);

		} catch (SQLException e) {
			e.printStackTrace();
		}

		return daysRemaning;
	}

	
}
