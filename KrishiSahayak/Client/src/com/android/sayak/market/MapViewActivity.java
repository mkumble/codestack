package com.android.sayak.market;

import com.android.sayak.R;
import android.app.Activity;

import android.widget.LinearLayout;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.AdapterView.OnItemClickListener;
import android.view.animation.Animation;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.DecelerateInterpolator;
import android.webkit.WebView;
import android.webkit.WebSettings;


public class MapViewActivity extends Activity implements View.OnClickListener{
	
	WebView mapview;
	ListView list;
	LinearLayout mapLayout;
	LinearLayout map;
	String map_name[]={"Peer Locator","Expert Locator","Back To Menu",""};
	ArrayAdapter<String> listAdapter=null;
	
	
	public void  onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.maplist);
		
		//Prepare the ListView
		list=(ListView)findViewById(R.id.mapType);
		listAdapter=new ArrayAdapter<String>(this,R.layout.listitem1, map_name);
		list.setAdapter(listAdapter);
		list.setOnItemClickListener(listener2);
		
		//Handle the main Container
		mapLayout=(LinearLayout)findViewById(R.id.maplayout);
		map=(LinearLayout)findViewById(R.id.map);
		map.setVisibility(LinearLayout.INVISIBLE);
		
		mapview=(WebView)findViewById(R.id.mapview);
		mapview.setClickable(true);
		mapview.setVerticalScrollBarEnabled(false);
		WebSettings set=mapview.getSettings();
		set.setJavaScriptEnabled(true);
		
		
		//Since the views are large put them in a cache
		mapLayout.setPersistentDrawingCache(ViewGroup.PERSISTENT_ANIMATION_CACHE);
		
    	}

	
	
	
	//onItemclicklistener for ListView
	OnItemClickListener listener2=new OnItemClickListener()
	{

		@Override
		public void onItemClick(AdapterView<?> arg0, View arg1, int pos,
				long arg3) {
			// TODO Auto-generated method stub
			if(pos==0)
			{
				mapview.loadUrl("file:///android_asset/peer.html");
				applyRotation(pos, 0, 90);
			}
			else if(pos==1)
			{
				mapview.loadUrl("file:///android_asset/expert.html");
				applyRotation(pos, 0, 90);
			}
			else if(pos==2){
				finish();
			}
		}
		
	};

    //Handle onClickListener for the MapView
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		
		applyRotation(-1, 180, 90);
	}
	
	//This is used to apply Rotation to the views
	private void applyRotation(int position,float start,float end){
		 // Find the center of the container
        final float centerX = mapLayout.getWidth() / 2.0f;
        final float centerY = mapLayout.getHeight() / 2.0f;

        // Create a new 3D rotation with the supplied parameter
        // The animation listener is used to trigger the next animation
        final Rotate3dAnimation rotation =
                new Rotate3dAnimation(start, end, centerX, centerY, 310.0f, true);
        rotation.setDuration(500);
        rotation.setFillAfter(true);
        rotation.setInterpolator(new AccelerateInterpolator());
        rotation.setAnimationListener(new DisplayNextView(position));

        mapLayout.startAnimation(rotation);
	}
	
	/**
     * This class listens for the end of the first half of the animation.
     * It then posts a new action that effectively swaps the views when the container
     * is rotated 90 degrees and thus invisible.
     */
    private final class DisplayNextView implements Animation.AnimationListener {
        private final int mPosition;

        private DisplayNextView(int position) {
            mPosition = position;
        }

        public void onAnimationStart(Animation animation) {
        }

        public void onAnimationEnd(Animation animation) {
            mapLayout.post(new SwapViews(mPosition));
        }

        public void onAnimationRepeat(Animation animation) {
        }
    }
    
    
    //This is used to swap between listview and the mapview
    private final class SwapViews implements Runnable {
        private final int mPosition;

        public SwapViews(int position) {
            mPosition = position;
        }

        public void run() {
            final float centerX = mapLayout.getWidth() / 2.0f;
            final float centerY = mapLayout.getHeight() / 2.0f;
            Rotate3dAnimation rotation;

            if (mPosition > -1) {
                list.setVisibility(View.GONE);
                map.setVisibility(View.VISIBLE);
                map.requestFocus();

                rotation = new Rotate3dAnimation(0,-360, centerX, centerY, 310.0f, false);
            } else {
                map.setVisibility(View.GONE);
                list.setVisibility(View.VISIBLE);
                list.requestFocus();

                rotation = new Rotate3dAnimation(90, 0, centerX, centerY, 310.0f, false);
            }

            rotation.setDuration(500);
            rotation.setFillAfter(true);
            rotation.setInterpolator(new DecelerateInterpolator());

            mapLayout.startAnimation(rotation);
        }
    
    }

}
