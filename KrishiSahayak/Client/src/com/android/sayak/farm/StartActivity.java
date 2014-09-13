package com.android.sayak.farm;


import com.android.sayak.R;
import com.android.sayak.R.anim;
import com.android.sayak.R.id;
import com.android.sayak.R.layout;
import com.android.sayak.market.MarketActivity;
import com.android.sayak.weather.WeatherActivity;

import android.app.Activity;
import android.os.Bundle;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.ArrayAdapter;
import android.widget.AdapterView.OnItemClickListener;
import android.view.View;
import android.content.Context;
import android.content.Intent;
import android.widget.ViewFlipper;
import android.widget.Button;
import android.view.View.OnClickListener;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;


public class StartActivity extends Activity implements OnClickListener,OnItemClickListener {

	private ListView listApp;
	private ViewFlipper mainView;
	private Button buttonApp;
	private Button marketApp;
	private Context mContext;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.list);
        
        /*Intialize the variables and views*/
        mContext=this;
        buttonApp=(Button)findViewById(R.id.farm);
        marketApp=(Button)findViewById(R.id.market);
        mainView=(ViewFlipper)findViewById(R.id.flipper);
        listApp=(ListView)findViewById(R.id.list1);
        
        //Populate the list view 
        String moduleNames[]={"Accessories","Service","Crop Suggestor","Weather","Back"};
        ArrayAdapter<String> adapter=new ArrayAdapter<String>(this,R.layout.list_item,moduleNames);
        listApp.setAdapter(adapter);
        
        //set onclicklisteners for the list and button
        listApp.setOnItemClickListener(this);
        buttonApp.setOnClickListener(this);
        marketApp.setOnClickListener(this);
        
        //This will set the siide in and slide out animations for View Flipper 
        Animation s_in=AnimationUtils.loadAnimation(mContext,R.anim.slide_in);
        Animation s_out=AnimationUtils.loadAnimation(mContext,R.anim.slide_out);
        mainView.setInAnimation(s_in);
        mainView.setOutAnimation(s_out);
    }
    
   @Override
public void onItemClick(AdapterView<?> arg0, View arg1, int position, long arg3) {
	// TODO Auto-generated method stub
	if(position==0)
	{
		Intent access=new Intent(mContext,AccessActivity.class);
		startActivity(access);
	}
	else if(position==1){
		Intent service=new Intent(mContext,ServiceActivity.class);
		startActivity(service);
	}
	else if(position==2){
		Intent service=new Intent(mContext,SuggestActivity.class);
		startActivity(service);
	}
	else if(position==3){
		Intent weather=new Intent(mContext,WeatherActivity.class);
		startActivity(weather);
	}
	else if(position==4){
		mainView.showNext();
	}
	else if(position==5){
		
	}
	else if(position==6){
		
	}

	
}

@Override
public void onClick(View v) {
	// TODO Auto-generated method stub
	if(v.getId()==R.id.market)
	{
		Intent market_start=new Intent(mContext,MarketActivity.class);
		startActivity(market_start);
	}
	else if(v.getId()==R.id.farm)
	{
		mainView.showNext(); //Will show the farm application 
	}
	
}
    
}