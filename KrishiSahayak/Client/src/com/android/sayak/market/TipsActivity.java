package com.android.sayak.market;

import com.android.sayak.R;
import com.android.sayak.util.ReadProperty;

import android.app.Activity;
import android.os.Bundle;
import android.webkit.WebView;


public class TipsActivity extends Activity{
	
		private WebView tips;
	public void onCreate(Bundle savedInstaceState){
		super.onCreate(savedInstaceState);
		setContentView(R.layout.web);
		tips=(WebView)findViewById(R.id.tipview);
		ReadProperty property=new ReadProperty(this);
		String location=property.get("tipLocation");
		tips.loadUrl(location);
	}

}
