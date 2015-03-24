package com.android.sayak.util;

import java.io.InputStream;
import java.util.Properties;
import android.content.Context;

import com.android.sayak.R;

public class ReadProperty {

	private Properties locProp=new Properties();
	private Context mContext;
	private String location=null;
	
	public ReadProperty(Context mContext)
	{
		this.mContext=mContext;
	}
	public String get(String property_name)
	{
		try {

			InputStream in = mContext.getResources().openRawResource(R.raw.location);
			locProp.load(in);
			location = locProp.getProperty(property_name);

		} catch (Exception e) {
			e.printStackTrace();
		}
		return location;
	}
}
