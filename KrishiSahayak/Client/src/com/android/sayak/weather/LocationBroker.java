package com.android.sayak.weather;

import android.location.Location;
import android.webkit.WebView;


public class LocationBroker {
	
	WeatherActivity weather;
	
	public LocationBroker(WeatherActivity weather)
	{
		this.weather=weather;
	}
	
	public void onLocationChanged(Location location){
		
		double latitude=location.getLatitude();
		double longitude=location.getLongitude();
		
		weather.onLocationChanged(latitude, longitude);
		
	}
	
	public  double[] getCurrentLocation(){
		Location location_GPS=GPSManager.getCurrentLocation();
		Location location_NET=NetworkManager.getCurrentLocation();
		double latitude=0;
		double longitude=0;
		if(location_GPS!=null && location_NET!=null){
		   double  latitudeN=location_NET.getLatitude();
		   double  longitudeN=location_NET.getLongitude();
		   long TimestampN=location_NET.getTime();
		   double  latitudeG=location_GPS.getLatitude();
		   double  longitudeG=location_GPS.getLongitude();
		   long TimestampG=location_GPS.getTime();
		   if(TimestampN>TimestampG){
			   latitude=latitudeN;
			   longitude=longitudeN;
		   }
		   else
		   {
			   latitude=latitudeG;
			   longitude=longitudeG;
		   }
		   
		}
		else if(location_GPS==null && location_NET!=null){
			latitude=location_NET.getLatitude();
			longitude=location_NET.getLongitude();
			
		}
		else if(location_GPS!=null && location_NET==null){
			latitude=location_GPS.getLatitude();
			longitude=location_GPS.getLongitude();
		}
		
		double[] location={latitude,longitude};
		
		return location;
		
	}
	
	public  void RegisterGeoLocation(){
		if(NetworkManager.isSupported()){
			NetworkManager.startListening(this);
		}
		if(GPSManager.isSupported()){
			GPSManager.startListening(this);
		}
		
	}
	public void UnRegisterGeoLocation(){
		if(NetworkManager.isListening()){
			NetworkManager.stopListening();
		}
		if(GPSManager.isListening()){
			GPSManager.stopListening();
		}
	}

}
