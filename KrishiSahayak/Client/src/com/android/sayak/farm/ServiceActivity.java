package com.android.sayak.farm;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.widget.ImageButton;
import android.widget.EditText;
import android.widget.Button;
import android.widget.Toast;
import android.view.View;
import android.view.View.OnClickListener;
import org.json.JSONObject;
import org.json.JSONException;

import com.android.sayak.R;
import com.android.sayak.util.CheckInternetConnection;
import com.android.sayak.util.ReadProperty;
import com.android.sayak.util.RestClient;
import android.content.Context;
import android.webkit.JsPromptResult;
import android.webkit.JsResult;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;





public class ServiceActivity extends Activity implements OnClickListener,Runnable {
	
	private Button showHealth;
	private Button getService;
	private ImageButton home;
	private ImageButton help;
	private EditText hourMeter;
	private ProgressDialog pDialog;
	private CheckInternetConnection conn;
	private Context mContext;
	private String location=null;
	private String response;
	private String meterReading;
	
	private WebView graph;
	private AlertDialog.Builder dialog;

	private double gearOilKey;
	private double maintainKey;
	private double oilKey;
	private boolean service;
	private String serviceDate;
	
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		setContentView(R.layout.service);
		
		//Get handles for the UI elements 
		home=(ImageButton)findViewById(R.id.home);
		help=(ImageButton)findViewById(R.id.help);
		showHealth=(Button)findViewById(R.id.showHealth);
		getService=(Button)findViewById(R.id.getService);
		hourMeter=(EditText)findViewById(R.id.hourMeter);
		
		mContext=this;
		
		ReadProperty property=new ReadProperty(mContext);
		location=property.get("serviceLocation");
		serviceDate=property.get("serviceDate");
		
		//Add onClickListeners to the UI Elements
		home.setOnClickListener(this);
		help.setOnClickListener(this);
		showHealth.setOnClickListener(this);
		getService.setOnClickListener(this);
		hourMeter.setOnClickListener(this);
		dialog=new AlertDialog.Builder(this);
		
		
	}
	
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		if(v.getId()==R.id.home)
		{
			finish();
		
		}
		else if(v.getId()==R.id.help)
		{
			Toast.makeText(mContext,"help",50).show();
		}
		else if(v.getId()==R.id.showHealth)
		{
			conn = new CheckInternetConnection(mContext);
			// Check is connection is there then download the recent price list
			if (conn.isConnected()) {
				pDialog = ProgressDialog.show(this, "Fetching Tractor Health...",
					"Please Wait", true);
				 meterReading=hourMeter.getText().toString();
				 service=true;
				Thread download = new Thread(this);
				download.start();
			} else {
				Toast.makeText(mContext, "NO INTERNET CONNECTION", 50).show();
			}
		}
		else if(v.getId()==R.id.getService)
		{
			conn = new CheckInternetConnection(mContext);
			// Check is connection is there then download the recent price list
			if (conn.isConnected()) {
				pDialog = ProgressDialog.show(this, "Fetching Service Date",
					"Please Wait", true);
				 
				
				Thread download = new Thread(this);
				download.start();
			} else {
				Toast.makeText(mContext, "NO INTERNET CONNECTION", 50).show();
			}
		}
		else if(v.getId()==R.id.hourMeter)
		{
			hourMeter.setText("");
		}
	}
	
	
	
	
	//This will map the tractor health onto to a graph 
	public void plotGraph(){
		
			
		setContentView(R.layout.health);
		dialog = new AlertDialog.Builder(this);
		graph=(WebView)findViewById(R.id.graphView);
		graph.setWebChromeClient(new MyWebChromeClient());
		WebSettings set=graph.getSettings();
		set.setJavaScriptEnabled(true);
		graph.loadUrl("file:///android_asset/index.html");
		graph.addJavascriptInterface(this,"Health");
		home=(ImageButton)findViewById(R.id.home);
		help=(ImageButton)findViewById(R.id.help);
		home.setOnClickListener(this);
		help.setOnClickListener(this);
		
		
	}
	
	protected void onStop()
	{
		super.onStop();
		finish();
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub
		
		RestClient client=null;
		if(service){
			client=new RestClient(location);	
		client.addParam("hourMeterFromVehicle",meterReading);
		}
		else
		{
			client=new RestClient(serviceDate);
			client.addParam("model","super1000" );
		}
		try {
			if(client.Execute(RestClient.RequestMethod.GET)){
				response=client.getResponse();
			}
			else
			{
				response=null;
				
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		 
	    
		
		handler.sendEmptyMessage(0);
	}
	
	private Handler handler=new Handler(){
		public void handleMessage(Message msg){
			pDialog.dismiss();
			if(response!=null && service){
			JSONObject tractorHealth;
						
			try {
				
			    tractorHealth=new JSONObject(response);
			    
			    gearOilKey=tractorHealth.getDouble("gearOilkey");
			    maintainKey=tractorHealth.getDouble("maintainKey");
			    oilKey=tractorHealth.getDouble("oilKey");
				plotGraph();	   
			    	 		    
				
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
				service=false;
			}
			else if(response!=null && !service){
				dialog.setMessage(response+" days remaining for service");
				dialog.setTitle("Service Date");
				dialog.setPositiveButton("OK",null);
				dialog.show();
			}
			else
			{
				Toast.makeText(mContext,"No Response from Server",100).show();
			}		
		}
	};

	/** Called to provide to default implementation for the Alert */
	class MyWebChromeClient extends WebChromeClient {
		public boolean onJSAlert(WebView view, String url, String message,
				JsResult result) {
			dialog.setMessage(message);
			dialog.setTitle("ALert!");
			dialog.show();
			return true;
		}

		public boolean onJSPrompt(WebView view, String url, String message,
				String defaultValue, final JsPromptResult result) {

			return true;
		}

	}
	
	public double getGearKey(){
		return this.gearOilKey;
	}
	public double getOilKey(){
		return this.oilKey;
	}
	public double getMaintainKey(){
		return this.maintainKey;
	}

}
