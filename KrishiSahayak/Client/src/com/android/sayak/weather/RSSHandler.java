package com.android.sayak.weather;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;
import java.util.List;
import java.util.ArrayList;

public class RSSHandler extends DefaultHandler {
	
		private List<WeatherMessage> messages;
		private WeatherMessage currentMessage;
		private StringBuilder builder;
		
		private String FORECAST="forecast";
		private String TITLE="title";
		private String SPREDICTION="short-prediction";
		private String IMAGE_URL="image";
		private String DESCRIPTION="description";
		private String PREDICTION="prediction";
		private String HIGH="high";
		private String LOW="low";
		
		public List<WeatherMessage> getMessages(){
			return this.messages;
		}
		
		
		public void characters(char []ch,int start,int length)throws SAXException {
			super.characters(ch, start, length);
			builder.append(ch,start,length);
		}
		
		
		public void startDocument() throws SAXException{
			super.startDocument();
			messages=new ArrayList<WeatherMessage>();
			builder=new StringBuilder();
		}
		
		
		public void startElement(String uri,String localName,String name,Attributes attributes)throws SAXException
		{
			super.startElement(uri,localName,name,attributes);
			if(localName.equalsIgnoreCase(FORECAST)){
				this.currentMessage=new WeatherMessage();
			}
		}
		
		
		public void endElement(String uri,String localName,String qName)throws SAXException
		{
			super.endElement(uri, localName, qName);
			if(this.currentMessage !=null){
				
				if(localName.equalsIgnoreCase(TITLE)){
					currentMessage.setTitle(builder.toString());
				}
				else if(localName.equalsIgnoreCase(SPREDICTION)){
					currentMessage.setSprediction(builder.toString());
				}
				else if(localName.equalsIgnoreCase(IMAGE_URL)){
					currentMessage.setImageURL(builder.toString());
				}
				else if(localName.equalsIgnoreCase(DESCRIPTION)){
					currentMessage.setDescription(builder.toString());
				}
				else if(localName.equalsIgnoreCase(PREDICTION)){
					currentMessage.setPrediction(builder.toString());
				}
				else if(localName.equalsIgnoreCase(HIGH)){
					currentMessage.setHigh(builder.toString());
				}
				else if(localName.equalsIgnoreCase(LOW)){
					currentMessage.setLow(builder.toString());
				}
				else if(localName.equalsIgnoreCase(FORECAST)){
					messages.add(currentMessage);
				}
				builder.setLength(0);
				
			}
		}

}
