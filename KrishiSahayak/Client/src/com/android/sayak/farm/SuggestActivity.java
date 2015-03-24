package com.android.sayak.farm;

import org.json.JSONArray;
import org.json.JSONException;

import com.android.sayak.R;
import com.android.sayak.util.CheckInternetConnection;
import com.android.sayak.util.RestClient;
import com.android.sayak.util.TrendsActivity;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import android.view.View;
import android.view.View.OnClickListener;
import android.content.Intent;
import android.content.Context;
import android.widget.ImageButton;
import android.widget.Spinner;
import android.widget.AdapterView.OnItemSelectedListener;
import com.android.sayak.util.ReadProperty;

public class SuggestActivity extends Activity implements OnClickListener,OnItemSelectedListener,Runnable{
	
		private Button trends;
		private Button askSuggestion;
		private Context mContext;
		private ImageButton home;
		private ImageButton help;
		private ArrayAdapter<String> climate_adapter;
		private ArrayAdapter<String> irrigation_adapter;
		private ArrayAdapter<String> soil_adapter;
		
		private Spinner Climate;
		private Spinner Soil;
		private Spinner Irrigation;
		private CheckInternetConnection conn;
		
		private String climate;
		private String soil;
		private String irrigation;
		private ProgressDialog pDialog;
		
		private String location=null;
		private String response=null;
		
		private AlertDialog.Builder dialog;
		
		String[] climate_type={"humid","sunny","wet"};
		
		String[] irrigation_humid={"medium","low"};
		String[] irrigation_sunny={"low","medium"};
		String[] irrigation_wet={"high","medium"};
		
		String[] soil_humid={"black","red","alluvial"};
		String[] soil_sunny={"black","alluvial"};
		String[] soil_wet={"red","black"};
	
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		setContentView(R.layout.suggest);
		
		//Set up the Spinner
		Climate=(Spinner)findViewById(R.id.ClimateId);
		Soil=(Spinner)findViewById(R.id.SoilId);
		Irrigation=(Spinner)findViewById(R.id.irri);
		
		
		trends=(Button)findViewById(R.id.suggShow);
		askSuggestion=(Button)findViewById(R.id.suggSubmit);
		home=(ImageButton)findViewById(R.id.home);
		help=(ImageButton)findViewById(R.id.help);
		home.setOnClickListener(this);
		help.setOnClickListener(this);
		trends.setOnClickListener(this);
		askSuggestion.setOnClickListener(this);
		mContext=this;
		
		climate_adapter=new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,climate_type);
		climate_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		Climate.setAdapter(climate_adapter);
		
		soil_adapter=new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,soil_humid);
		soil_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		Soil.setAdapter(soil_adapter);
		
		irrigation_adapter=new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,irrigation_humid);
		irrigation_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		Irrigation.setAdapter(irrigation_adapter);
		
		Climate.setOnItemSelectedListener(this);
		Soil.setOnItemSelectedListener(listener1);
		Irrigation.setOnItemSelectedListener(listener2);
		
		ReadProperty property=new ReadProperty(mContext);
		location=property.get("cropLocation");
		
		dialog=new AlertDialog.Builder(mContext);
	}
	
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		if(v.getId()==R.id.suggShow)
		{
			Intent graph=new Intent(mContext,TrendsActivity.class);
			startActivity(graph);
		}
		else if(v.getId()==R.id.suggSubmit)
		{
			conn = new CheckInternetConnection(mContext);
			// Check is connection is there then download the recent price list
			if (conn.isConnected()) {
				pDialog = ProgressDialog.show(this, "Fetching a suggestion",
						"Please Wait", true);
				Thread download = new Thread(this);
				download.start();
			} else {
				Toast.makeText(mContext, "NO INTERNET CONNECTION", 50).show();
			}
			
		}
		else if(v.getId()==R.id.home)
		{
			finish();
		}
		else if(v.getId()==R.id.help)
		{
			Toast.makeText(mContext,"help",100).show();
		}
		
	}

	@Override
	public void onItemSelected(AdapterView<?> parent, View view, int pos,
			long arg3) {
		// TODO Auto-generated method stub
		
			climate=parent.getItemAtPosition(pos).toString();
			if(pos==0){
				soil_adapter=new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,soil_humid);
				soil_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
				Soil.setAdapter(soil_adapter);
				
				irrigation_adapter=new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,irrigation_humid);
				irrigation_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
				Irrigation.setAdapter(irrigation_adapter);
			}
			else if(pos==1)
			{
				soil_adapter=new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,soil_sunny);
				soil_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
				Soil.setAdapter(soil_adapter);
				
				irrigation_adapter=new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,irrigation_sunny);
				irrigation_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
				Irrigation.setAdapter(irrigation_adapter);
			}
			else if(pos==2){
				soil_adapter=new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,soil_wet);
				soil_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
				Soil.setAdapter(soil_adapter);
				
				irrigation_adapter=new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,irrigation_wet);
				irrigation_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
				Irrigation.setAdapter(irrigation_adapter);
			}
		
		
		
	}

	@Override
	public void onNothingSelected(AdapterView<?> arg0) {
		// TODO Auto-generated method stub
		
	}

	private OnItemSelectedListener listener1=new OnItemSelectedListener(){

		@Override
		public void onItemSelected(AdapterView<?> parent, View arg1, int pos,
				long arg3) {
			// TODO Auto-generated method stub
			soil=parent.getItemAtPosition(pos).toString();
		}

		@Override
		public void onNothingSelected(AdapterView<?> parent) {
			// TODO Auto-generated method stub
			
		}
		
	};
	
	private OnItemSelectedListener listener2=new OnItemSelectedListener(){

		@Override
		public void onItemSelected(AdapterView<?> parent, View arg1, int pos,
				long arg3) {
			// TODO Auto-generated method stub
			irrigation=parent.getItemAtPosition(pos).toString();
			
		}

		@Override
		public void onNothingSelected(AdapterView<?> arg0) {
			// TODO Auto-generated method stub
			
		}
		
	};

	@Override
	public void run() {
		// TODO Auto-generated method stub
		
		try {
			Thread.sleep(300);
			RestClient client = new RestClient(location);
			client.addParam("climate",climate);
			client.addParam("irrigation",irrigation);
			client.addParam("soil",soil);
			if(client.Execute(RestClient.RequestMethod.GET))
			{
				response = client.getResponse();
			}
			else
			{
				response=null;
			}

		} catch (Exception e) {
			e.printStackTrace();
		}

		handler.sendEmptyMessage(0);
		
	}
	
	private Handler handler=new Handler(){
		public void handleMessage(Message msg){
			pDialog.dismiss();
			if(response!=null){
				JSONArray array=null;
				StringBuffer suggestion=new StringBuffer("");
				try {
					array = new JSONArray(response);
					for(int i=0;i<array.length();i++){
						suggestion.append(array.get(i));
						suggestion.append(" ");
					}
					
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				dialog.setMessage("Suggestions "+suggestion.toString());
				dialog.setPositiveButton("OK",null );
				dialog.show();
				
			}
			else
			{
				Toast.makeText(mContext,"NO RESPONCE FROM SERVER",100).show();
			}
		}
	};
	

}
