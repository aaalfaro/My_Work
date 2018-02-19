package com.example.andyalfaro.smalltalk_rollcall;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;

import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;

import android.content.Context;
import android.os.AsyncTask;
import android.widget.Toast;

import static java.util.logging.Level.parse;

public class HTTPHelper extends AsyncTask<String, Void,String>  {
    private Context context;

    public HTTPHelper(Context context) {
        this.context = context;

    }
    @Override
    protected String doInBackground(String... params) {

            try{
                String Var1 = params[0];    //The three parameters we sent are separated and given variables
                String Var2 = params[1];
                String page = params[2];
                String data = URLEncoder.encode("Var1", "UTF-8")+ "=" + URLEncoder.encode(Var1, "UTF-8"); //Var 1 is encoded to UTF-8

                data += "&" + URLEncoder.encode("Var2", "UTF-8") + "=" + URLEncoder.encode(Var2, "UTF-8"); //Var 2 is encoded to UTF-8

                URL url = new URL("http://pong.msmary.edu/"+page+".php");   //Give our URL a variable
                HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection(); //Connects to the URL
                urlConnection.setDoOutput(true);
                OutputStreamWriter wr = new OutputStreamWriter(urlConnection.getOutputStream()); //Allows to send an outputstream
                wr.write(data); //sends the two variables we encoded earlier
                wr.flush();
                try {
                    InputStream in = new BufferedInputStream(urlConnection.getInputStream()); //Allows the app to receive an data from the URL
                    return readStream(in);
                } finally {
                    urlConnection.disconnect();
                }
            } catch(Exception e){
                return new String("Error: " + e.getMessage());
            }
        }

    private String readStream(InputStream is) { //This function is used to read from the URL
        try {
            ByteArrayOutputStream bo = new ByteArrayOutputStream();
            int i = is.read();
            while(i != -1) {
                bo.write(i);
                i = is.read();
            }
            return bo.toString();

        } catch (IOException e) {
            return "";
        }
    }

    @Override
    protected void onPostExecute(String result){ //This function is called when background is complete
        result = result.trim();
        if(result.equals("success"))
            MainActivity.qrScan.initiateScan(); //Camera is activated
        else
            Toast.makeText(context,result,Toast.LENGTH_LONG).show(); //Creates a Toast message that displays what the PHP echoed back.
    }


}
