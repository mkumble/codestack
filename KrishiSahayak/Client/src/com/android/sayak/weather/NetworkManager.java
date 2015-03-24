package com.android.sayak.weather;


import android.location.Location;
import android.location.LocationManager;
import android.location.LocationListener;
import android.content.Context;
import android.os.Bundle;


public class NetworkManager {
	
	//GeoListener
	private static LocationBroker broker;
	
	//LocationManager Declarations
	private static LocationManager locationManager;
	
	//indicated whether the geolocation is Running or Not 
	private static Boolean supported;
	
	//indicates whether the geolocation is Supported or Not
	private static Boolean running=false;
	
	public static boolean isListening(){
		return running;
	}
	
	//This is used to unregister 
	public static  void stopListening(){
		running=false;
		try{
			if(locationManager!=null && locationListener!=null){
				
				locationManager.removeUpdates(locationListener);
			}
		}
		catch(Exception e){	}
				
	}
	public static boolean isSupported(){
		if(supported==null){
			if(WeatherActivity.getContext()!=null){
				locationManager=(LocationManager)WeatherActivity.getContext().getSystemService(Context.LOCATION_SERVICE);
				supported=locationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER);
				
			}
			else{
				supported=Boolean.FALSE;
			}
		}
		return supported;
		
	}
	
	public static void startListening(LocationBroker locationBroker){
		locationManager=(LocationManager)WeatherActivity.getContext().getSystemService(Context.LOCATION_SERVICE);
		
		if(locationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER)){
			locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER,10000,10f,locationListener);
			running=Boolean.TRUE;
			broker=locationBroker;	
		}
		
	}
	
	public static Location getCurrentLocation(){
		
		Location location=null;
		if(running==Boolean.TRUE)
		{	
	    location=locationManager.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);
		
		}
		return location;
	}
	
	private static LocationListener locationListener=new LocationListener(){
		
		public void onLocationChanged(Location location){
			broker.onLocationChanged(location);
		}
		
		public void onProviderDisabled(String provider){ }
		
		public void onProviderEnabled(String provider){ }
		
		public void onStatusChanged(String provider,int status,Bundle extras){}
		
		
	};

}
