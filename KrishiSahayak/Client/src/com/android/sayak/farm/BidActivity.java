package com.android.sayak.farm;

import com.android.sayak.R;
import com.android.sayak.trade.BiddingActivity;
import com.android.sayak.trade.QueryActivity;
import com.android.sayak.trade.RegisterActivity;

import android.app.TabActivity;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Bundle;
import android.widget.TabHost;

public class BidActivity extends TabActivity{

	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		setContentView(R.layout.trade);
		
		Resources res = getResources(); // Resource object to get Drawables
	    TabHost tabHost = getTabHost();  // The activity TabHost
	    TabHost.TabSpec spec;  // Resusable TabSpec for each tab
	    Intent intent;  // Reusable Intent for each tab

	    // Create an Intent to launch an Activity for the tab (to be reused)
	    intent = new Intent().setClass(this, RegisterActivity.class);

	    // Initialize a TabSpec for each tab and add it to the TabHost
	    spec = tabHost.newTabSpec("Register").setIndicator("",
	                      res.getDrawable(R.drawable.register))
	                  .setContent(intent);
	    tabHost.addTab(spec);

	    // Do the same for the other tabs
	    intent = new Intent().setClass(this, BiddingActivity.class);
	    spec = tabHost.newTabSpec("Bid").setIndicator("",
	                      res.getDrawable(R.drawable.bid))
	                  .setContent(intent);
	    tabHost.addTab(spec);

	    intent = new Intent().setClass(this, QueryActivity.class);
	    spec = tabHost.newTabSpec("query").setIndicator("",
	                      res.getDrawable(R.drawable.query))
	                  .setContent(intent);
	    tabHost.addTab(spec);

	    tabHost.setCurrentTab(2);
		
	}
}
