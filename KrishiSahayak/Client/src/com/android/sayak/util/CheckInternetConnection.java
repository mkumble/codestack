package com.android.sayak.util;

import android.content.Context;
import android.net.ConnectivityManager;
/*
 * This class will test for internet connection is available on android or not
 */

public class CheckInternetConnection {
	
	
		private Context mContext;
		
		public CheckInternetConnection(Context mContext)
		{
			this.mContext=mContext;
		}
		
		public  boolean isConnected(){
			
			ConnectivityManager cm = (ConnectivityManager) mContext. getSystemService(Context.CONNECTIVITY_SERVICE);
		    // test for connection
		    if (cm.getActiveNetworkInfo() != null
		            && cm.getActiveNetworkInfo().isAvailable()
		            && cm.getActiveNetworkInfo().isConnected()) {
		        return true;
		    } else {
		        
		        return false;
		    }
		}

}
