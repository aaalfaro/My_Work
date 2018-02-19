package com.example.andyalfaro.smalltalk_rollcall;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import com.google.zxing.integration.android.IntentIntegrator;
import com.google.zxing.integration.android.IntentResult;

public class MainActivity extends Activity implements View.OnClickListener{
    private EditText username;
    private EditText pass;
    static IntentIntegrator qrScan;
    private Button button;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        username = findViewById(R.id.Uname);       //Email is given a variable
        pass =  findViewById(R.id.Pword);           //Password is given a variable
        qrScan = new IntentIntegrator(this);        //Uses Zxing APK, makes it so that the app can switch to the device's camera
        button = findViewById(R.id.button);         //the button is given a variable

        button.setOnClickListener(this);            //When pushed the onClick function will be called
    }

    public void onClick(View view) {
        String email = username.getText().toString(); // Takes whatever text that is in email
        String password = pass.getText().toString();    //and password and converts them into strings

        new HTTPHelper(this).execute(email,password,"connection"); //Email and password are sent to the HTTPHelper class

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) { //This function reads what is in the QR code
        IntentResult result = IntentIntegrator.parseActivityResult(requestCode, resultCode, data);
        if (result != null) { //Camera will stay on until it scans a QR code

            if (result.getContents() == null) { //In case the QR code has nothing.
                Toast.makeText(this, "No result", Toast.LENGTH_LONG).show();
            }

            else {

                String email = username.getText().toString();   //we send the email that the student input earlier
                new HTTPHelper(this).execute(result.getContents(),email,"SignIn");  //We send the content of the QR code along with the students email.
            }
        }

        else {
            super.onActivityResult(requestCode, resultCode, data);
        }
    }
}


