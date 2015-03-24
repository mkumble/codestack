package com.android.sayak;




import com.android.sayak.farm.StartActivity;

import android.app.Activity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.content.Context;
import android.content.Intent;


public class SplashActivity extends Activity{
	
	
	private ImageView image;
	private Context CONTEXT;
	private Thread mSplashThread;
	
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		setContentView(R.layout.splash);
		
		CONTEXT=this;
		image = (ImageView) findViewById(R.id.SplashImageView);
		final Animation hyperspaceJump = AnimationUtils.loadAnimation(CONTEXT, R.anim.anim);
		image.post(new Runnable(){

			@Override
			public void run() {
				// TODO Auto-generated method stub
				image.startAnimation(hyperspaceJump);
			}
			
		});
		// The thread to wait for splash screen events
    	mSplashThread =  new Thread(){
    		@Override
    		public void run(){
    			try {
    				synchronized(this){
    					// Wait given period of time or exit on touch
    					wait(1000);
    				}
    			} 
    			catch(InterruptedException ex){    				
    			}

    			finish();
    			
    			// Run next activity
    			Intent intent = new Intent();
    			intent.setClass(CONTEXT, StartActivity.class);
    			startActivity(intent);
    			stop();     				
    		}
    	};
    	
    	mSplashThread.start();
		
		
	}
	
	
	/**
     * Processes splash screen touch events
     */
    @Override
    public boolean onTouchEvent(MotionEvent evt)
    {
    	if(evt.getAction() == MotionEvent.ACTION_DOWN)
    	{
    		synchronized(mSplashThread){
    			mSplashThread.notifyAll();
    		}
    	}
    	return true;
    }
	
	
}	
	
		
    
    
    
    

