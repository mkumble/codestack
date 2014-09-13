package com.android.sayak.market;

import com.android.sayak.R;
import com.android.sayak.util.RestClient;
import com.android.sayak.util.CheckInternetConnection;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.AbsListView.LayoutParams;
import android.widget.BaseAdapter;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.app.ProgressDialog;
import android.content.Context;
import android.graphics.Color;
import android.widget.Toast;
import java.io.InputStream;
import java.util.Iterator;
import java.util.Properties;
import android.os.Message;
import org.json.JSONObject;
import org.json.JSONException;

public class PriceActivity extends Activity implements Runnable,OnClickListener {

	private ProgressDialog pDialog;
	private CheckInternetConnection conn;
	private String response = null;
	private Context mContext;
	private ImageButton home;
	private ImageButton help;
	Properties locProp = new Properties();
	private String location = null;
	private ListView list;
	private String[] crop_name = new String[6];
	private String[] crop_price = new String[6];
	private String[] crop_trend = new String[6];

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.price);

		mContext = this;

		// Handles for the UI Elements
		list = (ListView) findViewById(R.id.priceList);
		home=(ImageButton)findViewById(R.id.home);
		help=(ImageButton)findViewById(R.id.help);
		home.setOnClickListener(this);
		help.setOnClickListener(this);

		// Load the location of the Price server from properties file
		try {

			InputStream in = mContext.getResources().openRawResource(R.raw.location);
			locProp.load(in);
			location = locProp.getProperty("priceLocation");

		} catch (Exception e) {
			e.printStackTrace();
		}

		conn = new CheckInternetConnection(mContext);
		// Check is connection is there then download the recent price list
		if (conn.isConnected()) {
			downloadData();
		} else {
			Toast.makeText(mContext, "NO INTERNET CONNECTION", 50).show();
		}
	}

	protected void onStop() {
		super.onStop();
		finish();
	}

	// This will download the current price list from the server
	private void downloadData() {
		pDialog = ProgressDialog.show(this, "Fetching Prices...",
				"Please Wait", true);
		Thread download = new Thread(this);
		download.start();
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub

		try {
			Thread.sleep(300);
			RestClient client = new RestClient(location);
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

	private Handler handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			pDialog.dismiss();
			// handle the result here
			if (response != null) {

				JSONObject crop = null;
				String loc = null;
				try {
					crop = new JSONObject(response);
					Iterator<String> keys = crop.keys();
					for (int i = 0; i < crop.length(); i++) {
						loc = keys.next();
						crop_name[i] = loc;
						JSONObject cprice = new JSONObject(crop.getJSONObject(
								loc).toString());
						crop_price[i] = cprice.getString("price");
						crop_trend[i] = cprice.getString("trend");
					}

				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				list.setAdapter(new priceAdapter());

			}
			else
			{
			Toast.makeText(mContext, "Connection with server Down",100).show();
			}

		}

	};

	public class priceAdapter extends BaseAdapter {

		@Override
		public int getCount() {
			// TODO Auto-generated method stub
			return crop_name.length;
		}

		@Override
		public Object getItem(int position) {
			// TODO Auto-generated method stub
			return null;
		}

		@Override
		public long getItemId(int position) {
			// TODO Auto-generated method stub
			return 0;
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			// TODO Auto-generated method stub

			LinearLayout layout = new LinearLayout(mContext);
			layout.setOrientation(LinearLayout.HORIZONTAL);
			layout.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT,
					LayoutParams.WRAP_CONTENT));

			TextView text1 = new TextView(mContext);
			text1.setText(crop_name[position]);
			text1.setTextSize(20);
			text1.setPadding(0, 0, 70, 10);
			text1.setTextColor(Color.BLUE);

			TextView text2 = new TextView(mContext);
			text2.setText(crop_price[position]);
			text2.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT,
					LayoutParams.WRAP_CONTENT));
			text2.setTextSize(20);
			text2.setGravity(Gravity.RIGHT);

			if (crop_trend[position].equals("0")) {
				text2.setTextColor(Color.RED);
			} else {
				text2.setTextColor(Color.GREEN);
			}

			layout.addView(text1);
			layout.addView(text2);

			return layout;
		}

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
