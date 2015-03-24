package com.android.sayak.weather;


import android.app.Activity;
import android.app.ProgressDialog;

import java.net.MalformedURLException;
import java.net.URL;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.content.Context;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;

import java.util.Iterator;
import java.util.List;

import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ImageView;
import android.graphics.drawable.Drawable;

import java.io.IOException;
import java.io.InputStream;
import android.widget.LinearLayout;

import com.android.sayak.R;
import com.android.sayak.market.PriceActivity.priceAdapter;
import com.android.sayak.util.CheckInternetConnection;
import com.android.sayak.util.RestClient;

import java.util.Properties;

import org.json.JSONException;
import org.json.JSONObject;

public class WeatherActivity extends Activity implements Runnable,OnClickListener {
	
	private static  Context CONTEXT;
	Properties locProp = new Properties();
	private CheckInternetConnection conn;
	LocationBroker broker;
	private String location = null;
	LinearLayout mainLayout;
	private ProgressDialog pDialog;
	FeedReader reader;
	private double fLatitude;
	private double fLongitude;
	private ImageButton home;
	private ImageButton help;
	
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.weather);
        CONTEXT=this;
        
        mainLayout=(LinearLayout)findViewById(R.id.weatherlayout);
        pDialog = ProgressDialog.show(this, "Fetching Weather ...",
				"Please Wait", true);
        broker=new LocationBroker(this);
        broker.RegisterGeoLocation();
        home=(ImageButton)findViewById(R.id.home);
		help=(ImageButton)findViewById(R.id.help);
		home.setOnClickListener(this);
		help.setOnClickListener(this);
        
        
        
        
    }
    
    public static Context getContext()
    {
    	return CONTEXT;
    }
    
    protected void onStop()
    {
    	super.onStop();
    	broker.UnRegisterGeoLocation();
    }
    
    public void onLocationChanged(double latitude,double longitude)
    {
    	downloadData();
    	fLatitude=latitude;
    	fLongitude=longitude;
    }
    
    public void downloadData()
    {
    	// Load the location of the Price server from properties file
		try {

			InputStream in = getResources().openRawResource(R.raw.location);
			locProp.load(in);
			location = locProp.getProperty("weatherAPI");

		} catch (Exception e) {
			e.printStackTrace();
		}

		conn = new CheckInternetConnection(CONTEXT);
		// Check is connection is there then download the recent price list
		if (conn.isConnected()) {
			
			
			
			Thread download = new Thread(this);
			download.start();
			    
							
		} else {
			Toast.makeText(CONTEXT, "NO INTERNET CONNECTION", 50).show();
			pDialog.dismiss();
		}
    }
	
    public void run() {
		// TODO Auto-generated method stub

		     reader=new FeedReader("http://api.wxbug.net/getForecastRSS.aspx?ACode=A5346964462&lat="+fLatitude+"&long="+fLongitude+"&unittype=1");
			 handler.sendEmptyMessage(0);
		}
			
	
    private Handler handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			
			// handle the result here
			List<WeatherMessage> list=reader.parse();
			for(int i=0;i<5;i++){
				WeatherMessage day=list.get(i);
				InputStream is=null;
				try {
				 is=(InputStream)new URL(day.getImageURL()).getContent();
				} 
				catch(Exception e){
					e.printStackTrace();
				}
				Drawable dr=Drawable.createFromStream(is,"sample");
				ImageView view=new ImageView(CONTEXT);
				TextView text=new TextView(CONTEXT);
				String prediction=day.getDescription()+" "+day.getSprediction();
				text.setText(prediction);
				view.setImageDrawable(dr);
				TransparentPanel panel=new TransparentPanel(CONTEXT);
				panel.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT,LayoutParams.WRAP_CONTENT));
				
				panel.setPadding(10,10,10,10);
				panel.addView(view);
				panel.addView(text);
			    mainLayout.addView(panel);
			    pDialog.dismiss();
			   
			}
		}

	};

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		
		if(v.getId()==R.id.home)
		{
			finish();
		
		}
		else if(v.getId()==R.id.help)
		{
			Toast.makeText(CONTEXT,"help",50).show();
		}
		
	}
    
    
}    
    