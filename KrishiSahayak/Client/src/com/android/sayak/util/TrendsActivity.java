package com.android.sayak.util;

import com.android.sayak.R;

import android.app.Activity;
import android.os.Bundle;
import android.widget.ImageButton;
import android.widget.Spinner;
import android.widget.ArrayAdapter;
import android.widget.Toast;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.AdapterView;
import android.view.View;
import android.view.View.OnClickListener;
import android.content.Context;
import android.webkit.WebSettings;
import android.webkit.WebView;



public class TrendsActivity extends Activity implements OnClickListener{
      
	 private Spinner spinner1;
	 private Spinner spinner2;
	 private ArrayAdapter<CharSequence> spinner1_adapter;
	 private ArrayAdapter<CharSequence> spinner2_adapter;
	 private Context mContext;
	 private ImageButton home;
	 private ImageButton help;
	 private WebView webview;
	 private String type=null;
	
	/*This is called each time the Activity starts*/
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		setContentView(R.layout.graph);
		
		mContext=this;
		
		home=(ImageButton)findViewById(R.id.home);
		help=(ImageButton)findViewById(R.id.help);
		home.setOnClickListener(this);
		help.setOnClickListener(this);
		
		webview=(WebView)findViewById(R.id.webview);
		WebSettings set=webview.getSettings();
		set.setJavaScriptEnabled(true);
		webview.loadUrl("file:///android_asset/first.html");
		
		
		
		
		
		spinner1=(Spinner)findViewById(R.id.spinner1);
		spinner2=(Spinner)findViewById(R.id.spinner2);
		
		//set the data for the first spinner
	    spinner1_adapter=ArrayAdapter.createFromResource(this,R.array.type_name,android.R.layout.simple_spinner_item);
		spinner1_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		spinner1.setAdapter(spinner1_adapter);
		
		//set the data for the second spinner 
	    spinner2_adapter=ArrayAdapter.createFromResource(this,R.array.area_name,android.R.layout.simple_spinner_item);
		spinner2_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		spinner2.setAdapter(spinner2_adapter);
		
		//set onItemClickListener for the first Item
		spinner1.setOnItemSelectedListener(new MyOnItemSelectedListener1());
		
		//set onItemClickListener for second Item
		spinner2.setOnItemSelectedListener(new MyOnItemSelectedListener2());
	}
	
	//Implementatopn of itemclicklistener1
	public class MyOnItemSelectedListener1 implements OnItemSelectedListener {

	    public void onItemSelected(AdapterView<?> parent,
	        View view, int pos, long id) {
	    	
	     type=parent.getItemAtPosition(pos).toString();
	      if(pos==0){
	    	  spinner2_adapter=ArrayAdapter.createFromResource(mContext,R.array.area_name,android.R.layout.simple_spinner_item);
	  		  spinner2_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
	  		  spinner2.setAdapter(spinner2_adapter);
	  		  
	      }
	      else if(pos==1){
	    	  spinner2_adapter=ArrayAdapter.createFromResource(mContext,R.array.crop_name,android.R.layout.simple_spinner_item);
	  		  spinner2_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
	  		  spinner2.setAdapter(spinner2_adapter);
	    	  
	      }
	    }

	    public void onNothingSelected(AdapterView parent) {
	      // Do nothing.
	    }
	}
	
	//Implementation of itemclicklistener2
	public class MyOnItemSelectedListener2 implements OnItemSelectedListener {

	    public void onItemSelected(AdapterView<?> parent,
	        View view, int pos, long id) {
	    	if(type.equals("Area"))
	    	{
	    		if(pos==0){
	    			webview.loadUrl("file:///android_asset/first.html");
	    		}
	    		else if(pos==1){ webview.loadUrl("file:///android_asset/second.html");}
	    	}
	    	else if(type.equals("Crop")){
	    		if(pos==0){ webview.loadUrl("file:///android_asset/third.html");}
	    		else if(pos==1){webview.loadUrl("file:///android_asset/fourth.html");}
	    	}
	      
	    }

	    public void onNothingSelected(AdapterView parent) {
	      // Do nothing.
	    }
	}
	
	protected void onStop(){
		super.onStop();
		finish();
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
			Toast.makeText(mContext,"help",100).show();
		}
	}
	
}
