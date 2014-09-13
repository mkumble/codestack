package com.android.sayak.trade;

import com.android.sayak.R;
import com.android.sayak.R.layout;
import com.android.sayak.util.CheckInternetConnection;
import com.android.sayak.util.RestClient;

import android.app.Activity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import android.view.View;
import android.view.View.OnClickListener;
import android.app.ProgressDialog;
import com.android.sayak.util.ReadProperty;
import android.widget.ImageButton;
import android.content.Context;

public class RegisterActivity extends Activity implements OnClickListener, Runnable{

	private EditText id;
	private EditText region;
	private EditText mobile;
	private Button register;
	private Button reset;
	private CheckInternetConnection conn;
	ProgressDialog pDialog;
	String location=null;
	String response=null;
	private ImageButton home;
	private ImageButton help;
	private Context mContext;
	
	
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.tab1);
		
		mContext=this;
		//Prepare the EditText
		id=(EditText)findViewById(R.id.FarmerId);
		region=(EditText)findViewById(R.id.FarmerRegion);
		mobile=(EditText)findViewById(R.id.mobile);
		
		//Prepare the Button
		register=(Button)findViewById(R.id.regSubmit);
		reset=(Button)findViewById(R.id.regReject);
		register.setOnClickListener(this);
		reset.setOnClickListener(this);
		
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
	public void onClick(View v) {
		// TODO Auto-generated method stub
		if(v.getId()==R.id.regSubmit)
		{
			conn = new CheckInternetConnection(this);
			// Check is connection is there then download the recent price list
			if (conn.isConnected()) {
				pDialog = ProgressDialog.show(this, "Registering...",
						"Please Wait", true);
				Thread download = new Thread(this);
				download.start();
				
			} else {
				Toast.makeText(this, "NO INTERNET CONNECTION", 50).show();
			}
			
		}
		else if(v.getId()==R.id.regReject)
		{
			id.setText("");
			region.setText("");
			mobile.setText("");
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
	public void run() {
		// TODO Auto-generated method stub
		
		String FramerId=id.getEditableText().toString();
		String FarmerRegion=region.getEditableText().toString();
		String mobileNO=region.getEditableText().toString();
		
		
		
		RestClient client=new RestClient(location);
		
	}
	
	
	
}
