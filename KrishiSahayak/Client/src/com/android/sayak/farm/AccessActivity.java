package com.android.sayak.farm;
import java.io.InputStream;
import java.net.URL;
import java.util.Iterator;

import org.json.JSONException;
import org.json.JSONObject;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.app.ProgressDialog;
import android.widget.ImageButton;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.ImageView;
import android.widget.Toast;
import android.widget.Button;

import com.android.sayak.R;
import com.android.sayak.util.CheckInternetConnection;
import com.android.sayak.util.RestClient;
import com.google.zxing.ChecksumException;
import com.google.zxing.FormatException;
import com.google.zxing.NotFoundException;
import com.google.zxing.Result;
import com.google.zxing.Reader;
import com.google.zxing.MultiFormatReader;
import com.google.zxing.BinaryBitmap;
import com.google.zxing.RGBLuminanceSource;
import com.google.zxing.LuminanceSource;
import com.google.zxing.common.HybridBinarizer;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.content.Context;
import com.android.sayak.util.ReadProperty;
import com.android.sayak.weather.TransparentPanel;



public class AccessActivity extends Activity implements OnClickListener,Runnable{
    
	private ImageButton home;
	private ImageButton help;
	private Button scan;
	private CheckInternetConnection conn;
	private String tractor_power=null;
	private LinearLayout modelLayout;
	private TextView modelType;
	private TextView modelNumber;
	private TextView power;
	private TextView year;
	private Button access;
	private Context mContext;
	private ProgressDialog pDialog;
	private String location=null;
	private String response=null;
	private String imageURL=null;
	private LinearLayout layout;
	
	
	public void onCreate(Bundle savedInstance){
		super.onCreate(savedInstance);
		setContentView(R.layout.main);
		
		
		
		
		//Provide the handle for the UI Elements
		home=(ImageButton)findViewById(R.id.home);
		help=(ImageButton)findViewById(R.id.help);
		scan=(Button)findViewById(R.id.scan);
		modelLayout=(LinearLayout)findViewById(R.id.access_layout);
		modelType=(TextView)findViewById(R.id.ModelType);
		modelNumber=(TextView)findViewById(R.id.ModelNumber);
		power=(TextView)findViewById(R.id.power);
		year=(TextView)findViewById(R.id.year);
		access=(Button)findViewById(R.id.access);
		
		layout=(LinearLayout)findViewById(R.id.accesslayout2);
		mContext=this;
		
		
		ReadProperty properties=new ReadProperty(mContext);
		location=properties.get("accessLocation");
		imageURL=properties.get("imageURL");
		
		//OnClickListener 
		home.setOnClickListener(this);
		help.setOnClickListener(this);
		scan.setOnClickListener(this);
		access.setOnClickListener(this);
		
		//Hide the model Layout
		modelLayout.setVisibility(LinearLayout.INVISIBLE);
		
	}
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		if(v.getId()==R.id.home){ finish(); }
		
		else if(v.getId()==R.id.help)
		{
			Toast.makeText(mContext,"Help",100).show();
		}
		
		else if(v.getId()==R.id.scan)
		{
			layout.setVisibility(LinearLayout.GONE);
			layout.removeAllViews();
			Reader reader=new MultiFormatReader();
			Result result=null;
			//In Future this will be feed by Camera  and the barcode picture is residing on mobile itself as pf now 
			// We use zxing librzry for barcode decoding 
			Bitmap image=BitmapFactory.decodeResource(getResources(),R.drawable.bar1);
			image.getWidth();
			
			LuminanceSource source=new RGBLuminanceSource(image);
			BinaryBitmap bitmap=new BinaryBitmap(new HybridBinarizer(source));
			try {
			 result=reader.decode(bitmap);
			} catch (NotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (ChecksumException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (FormatException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			String text=result.getText();
			
			String[] textElement=text.split(":");	
			for(int i=0;i<textElement.length;i++){
				modelType.setText("MODEL TYPE"+" : "+textElement[0]);
				modelNumber.setText("MODEL NUMBER"+" : "+textElement[1]);
				year.setText("YEAR OF MANUFACTURE"+" : "+textElement[2]);
				power.setText("POWER OF TRACTOR"+" : "+textElement[3]);
				tractor_power=textElement[3];
				
				//Set the text Color and gravity
				
				modelType.setGravity(Gravity.CENTER_VERTICAL);
				modelNumber.setGravity(Gravity.CENTER_VERTICAL);
				year.setGravity(Gravity.CENTER_VERTICAL);
				power.setGravity(Gravity.CENTER_VERTICAL);
				
				modelType.setTextColor(Color.BLUE);
				modelNumber.setTextColor(Color.BLUE);
				year.setTextColor(Color.BLUE);
				power.setTextColor(Color.BLUE);
				
				modelLayout.setVisibility(LinearLayout.VISIBLE);
			}
			
			
		}
		else if(v.getId()==R.id.access)
		{
			conn = new CheckInternetConnection(mContext);
			// Check is connection is there then download the recent price list
			if (conn.isConnected()) {
				pDialog = ProgressDialog.show(this, "Fetching Accessories For This Model...",
						"Please Wait", true);
				Thread download = new Thread(this);
				download.start();
			} else {
				Toast.makeText(mContext, "NO INTERNET CONNECTION", 50).show();
			}
		}
	}
			
	protected void onStop(){
		super.onStop();
		finish();
	}
	@Override
	public void run() {
		// TODO Auto-generated method stub
		
		try {
			Thread.sleep(300);
			RestClient client = new RestClient(location);
			String[] power=tractor_power.split("W");
			client.addParam("powerID",power[0]);
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
				try {
					JSONObject access=new JSONObject(response);
					
					
					Iterator<String> keys=access.keys();
					for(int i=0;i<access.length();i++)
					{
						String accessory_name=keys.next();
						String accessory_url=access.getString(accessory_name);
						
						
						InputStream is=null;
						try {
						 is=(InputStream)new URL(imageURL+accessory_url).getContent();
						} 
						catch(Exception e){
							e.printStackTrace();
						}
						Drawable dr=Drawable.createFromStream(is,"sample");
						ImageView view=new ImageView(mContext);
						
						TextView text=new TextView(mContext);
						text.setText(accessory_name);
						text.setTextColor(Color.BLUE);
						view.setImageDrawable(dr);
						TransparentPanel panel=new TransparentPanel(mContext);
						panel.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT,LayoutParams.WRAP_CONTENT));
						
						panel.setPadding(10,10,10,10);
						panel.addView(view);
						panel.addView(text);
						layout.addView(panel);
						layout.setVisibility(LinearLayout.VISIBLE);
						modelLayout.setVisibility(LinearLayout.GONE);
						
					}
					
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
			}
			else
			{
				Toast.makeText(mContext,"Connection with Server Down",50).show();
			}
		}
	};

}
