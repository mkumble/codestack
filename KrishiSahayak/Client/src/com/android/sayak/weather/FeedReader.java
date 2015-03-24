package com.android.sayak.weather;

import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.io.InputStream;
import java.util.List;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.parsers.SAXParser;


/*This Class will read the RSS Feed from WeatherBug API and will Parse it store it as an ArrayList*/
/*This uses a SAX PARSER*/

public class FeedReader {
	
		
	private URL feedURL;
	
	List<WeatherMessage> messages;

	public FeedReader(String feedURL){
		try {
			this.feedURL=new URL(feedURL);
		} catch (MalformedURLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	private  InputStream getInputStream(){
		
			InputStream in=null;
			try {
				in=feedURL.openConnection().getInputStream();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			return in;
		
	}
	
	public List<WeatherMessage> parse(){
		SAXParserFactory factory=SAXParserFactory.newInstance();
		try{
			SAXParser parser=factory.newSAXParser();
			RSSHandler handler=new RSSHandler();
			parser.parse(getInputStream(),handler);
			messages= handler.getMessages();
		}
		catch(Exception e){
			e.printStackTrace();
		}
		return messages;
	}
	
	
	
}