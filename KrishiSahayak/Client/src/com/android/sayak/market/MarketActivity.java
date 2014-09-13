package com.android.sayak.market;

import com.android.sayak.R;



import android.widget.GridView;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.app.Activity;
import android.os.Bundle;
import android.widget.BaseAdapter;
import android.content.Context;
import android.content.Intent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Toast;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;


public class MarketActivity extends Activity implements OnClickListener{

	private GridView myGridView;
	private Context mContext;
	private ImageButton home;
	private ImageButton help;
	
		
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.market);
		mContext=this;
		
		
		home=(ImageButton)findViewById(R.id.home);
		help=(ImageButton)findViewById(R.id.help);
		home.setOnClickListener(this);
		help.setOnClickListener(this);
		
		myGridView=(GridView)findViewById(R.id.marketgrid);
		myGridView.setAdapter(new ImageAdapter(mContext));
		myGridView.setOnItemClickListener(listener);
		
	}
	
	
	//An adapter for populating the gridview 
	public class ImageAdapter extends BaseAdapter {
	    private Context mContext;

	    public ImageAdapter(Context c) {
	        mContext = c;
	    }

	    public int getCount() {
	        return mThumbIds.length;
	    }

	    public Object getItem(int position) {
	        return null;
	    }

	    public long getItemId(int position) {
	        return 0;
	    }

	    // create a new ImageView for each item referenced by the Adapter
	    public View getView(int position, View convertView, ViewGroup parent) {
	        ImageView imageView;
	        if (convertView == null) {  // if it's not recycled, initialize some attributes
	            imageView = new ImageView(mContext);
	            imageView.setLayoutParams(new GridView.LayoutParams(85, 85));
	            imageView.setScaleType(ImageView.ScaleType.CENTER_CROP);
	            imageView.setPadding(8, 8, 8, 8);
	          } else {
	            imageView = (ImageView) convertView;
	        }

	        imageView.setImageResource(mThumbIds[position]);
	        return imageView;
	    }

	    // references to our images
	    private Integer[] mThumbIds = {
	            R.drawable.price,
	            R.drawable.query,
	            R.drawable.sales,
	            R.drawable.calculator
	    };
	}
	
	//Listener for on gridview item selected 
	OnItemClickListener listener=new OnItemClickListener(){

		@Override
		public void onItemClick(AdapterView<?> arg0, View arg1, int pos,
				long arg3) {
			// TODO Auto-generated method stub
			if(pos==0){
				Toast.makeText(mContext,"Market Prices",100).show();
				Intent price=new Intent(mContext,PriceActivity.class);
				startActivity(price);
			}
			else if(pos==1){
				Toast.makeText(mContext,"AgriLocator",100).show();
				Intent mapIntent1=new Intent(mContext,MapViewActivity.class);
				startActivity(mapIntent1);
				
			}
			else if(pos==2){
				Toast.makeText(mContext,"BuyerStatus",100).show(); 
			}
			else if(pos==3){
				Toast.makeText(mContext,"Tips",100).show();
				Intent tips=new Intent(mContext,TipsActivity.class);
				startActivity(tips);
			}
			
					
		}

		
	};
	
	
	
	


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
