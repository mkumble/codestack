package com.android.sayak.trade;

import com.android.sayak.R;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageButton;
import android.widget.Toast;


public class BiddingActivity extends Activity implements OnClickListener{
	
	private ImageButton home;
	private ImageButton help;
	private Context mContext;
	
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		setContentView(R.layout.tab2);
		
		mContext=this;
		home=(ImageButton)findViewById(R.id.home);
		help=(ImageButton)findViewById(R.id.help);
		home.setOnClickListener(this);
		help.setOnClickListener(this);
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
