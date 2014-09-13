package dao;
/*
 * This takes hourMeter reading as inputfrom  maintainenceservlet and tells the health based on our assumption values from computation
 * */
public class Maintainence {

	public static double oilChange(double reading) {

		double mark = 50;
		double result = 0;

		if (reading <= mark) {
			result = reading / mark;
		}

		else {
			double temp = reading % mark;

			result = temp / mark;

		}
		double output = 1.0 - result;

		return output*10;
	}

	public static double gearOilChange(double reading) {

		double mark = 150;
		double result = 0;

		if (reading <= mark) {
			result = reading / mark;
		}

		else {
			double temp = reading % mark;

			result = temp / mark;

		}
		double output = 1.0 - result;

		System.out.println(output);
		return output*10;
	}

	public static double maintainence(double reading) {

		double mark = 250;
		double result = 0;

		if (reading <= mark) {
			result = reading / mark;
		}

		else {
			double temp = reading % mark;

			result = temp / mark;

		}
		double output = 1.0 - result;

		return output*10;
	}

}
