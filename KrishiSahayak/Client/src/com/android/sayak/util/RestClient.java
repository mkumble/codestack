package com.android.sayak.util;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.conn.ConnectTimeoutException;

import android.widget.Toast;


import java.io.*;
import java.util.ArrayList;
import java.net.URLEncoder;

public class RestClient {
	
	 private ArrayList<NameValuePair> params;
	 private ArrayList<NameValuePair> headers;
	 
	 private String url;
	 private String response;
	 private int responseCode;
	 private String errorMessage;
	 
	
	 
	
	 public  enum RequestMethod{
		 GET,
		 POST
	 }
	 
	 public String getResponse(){
		 return response;
	 }
	 
	 public int  getResponseCode(){
		 return responseCode;
	 }
	 
	 public String getErrorMessage(){
		 return errorMessage;
	 }
	 
	 public RestClient(String url){
		 
		 this.url=url;
		
		 params=new ArrayList<NameValuePair>();
		 headers=new ArrayList<NameValuePair>();
	 }
	 
	 public void addParam(String name,String value){
		 params.add(new BasicNameValuePair(name,value));
	 }
	 
	 public void addHeader(String name,String value){
		 headers.add(new BasicNameValuePair(name,value));
	 }
	 
	 public boolean Execute(RequestMethod method)throws  Exception {
		 
		 boolean CONNECTION_ESTABLISED=false;
		 switch(method){
		 case GET: 
		 {
			 String combinedParams="";
			 
			 if(!params.isEmpty()){
				 combinedParams+="?";
				 for(NameValuePair v: params){
					 String paramString=v.getName()+"="+URLEncoder.encode(v.getValue(),"UTF-8");
					 if(combinedParams.length()>1){
						 combinedParams+="&"+paramString;
					 }
					 else{
						 combinedParams+=paramString;
					 }
				 }
			 }
			 
			 HttpGet request=new HttpGet(url+combinedParams);
			 for(NameValuePair h: headers)
			 {
				 request.addHeader(h.getName(),h.getValue());
				 
			 }
			 CONNECTION_ESTABLISED= executeRequest(request,url);
			 
			 
			 break;
			 
		 }
		 case POST:
		 {
			 HttpPost request=new HttpPost(url);
			 for(NameValuePair h: headers)
			 {
				 request.addHeader(h.getName(),h.getValue());
				 
			 }
			 if(!params.isEmpty()){
				 request.setEntity(new UrlEncodedFormEntity(params,"UTF-8"));
			 }
			  CONNECTION_ESTABLISED = executeRequest(request,url);
			 break;
		 }	 
		 
		 }
		 return CONNECTION_ESTABLISED;
		 
	 }
	 
	 public boolean executeRequest(HttpUriRequest request,String url){
		 
		 HttpParams params=new BasicHttpParams();
		 HttpConnectionParams.setConnectionTimeout(params,10000);
		 boolean CONNECTION_ESTABLISHED=true;
		 HttpClient client=new DefaultHttpClient(params);
		 HttpResponse httpResponse;
		 try{
			 
			 httpResponse=client.execute(request);
			 responseCode=httpResponse.getStatusLine().getStatusCode();
			 errorMessage=httpResponse.getStatusLine().getReasonPhrase();
			 HttpEntity entity=httpResponse.getEntity();
			 if(entity!=null){
				 InputStream inStream=entity.getContent();
				 response=convertStreamToString(inStream);
				 inStream.close();
				 
			 }
		 }
		 catch(ClientProtocolException e){
			 client.getConnectionManager().shutdown();
			 e.printStackTrace();
		 }
		 catch(ConnectTimeoutException e){
			CONNECTION_ESTABLISHED=false;
		 }
		 catch(IOException e){
			 client.getConnectionManager().shutdown();
			 e.printStackTrace();
		 }
		 
		 return CONNECTION_ESTABLISHED;
		 
		 
	 }
	 
	 private String convertStreamToString(InputStream ins){
		 
		 BufferedReader buf=new BufferedReader(new InputStreamReader(ins));
		 StringBuilder sb=new StringBuilder();
		 String line=null;
		 try
		 {
			 while((line=buf.readLine())!=null){
				 sb.append(line+"\n");
				 			 }
			 
		 }
		 catch(IOException e)
		 {
			 e.printStackTrace();
		 }
		 finally{
		 try{
			 ins.close();
		 }
		 catch(Exception e){
			 e.printStackTrace();
		 }
		 }
		 
		 return sb.toString();
	 }
	 
	
	


}
