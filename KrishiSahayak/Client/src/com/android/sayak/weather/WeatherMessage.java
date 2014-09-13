package com.android.sayak.weather;

public class WeatherMessage {
	
	private String title;
	private String sprediction;
	private String imageURL;
	private String description;
	private String prediction;
	private String high;
	private String low;
	
	//Setter Methods
	public void setTitle(String title){
		this.title=title;
	}
	
	public void setSprediction(String sprediction){
		this.sprediction=sprediction;
	}
	
	public void setImageURL(String imageURL){
		this.imageURL=imageURL;
	}
	
	public void setDescription(String description){
		this.description=description;
	}
	
	public void setPrediction(String prediction){
		this.prediction=prediction;
	}
	
	public void setHigh(String high){
		this.high=high;
	}
	
	public void setLow(String low){
		this.low=low;
	}
	
	//Getters Methods
	public String getTitle(){
		return title;
	}
	
	public String getSprediction(){
		return sprediction;
	}
	
	public String getImageURL(){
		return imageURL;
	}
	
	public String getDescription(){
		return description;
	}
	
	public String getPrediction(){
		return prediction;
	}
	
	public String getHigh(){
		return high;
	}
	
	public String getLow(){
		return low;
	}
	
}
