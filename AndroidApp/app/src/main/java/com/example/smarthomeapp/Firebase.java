package com.example.smarthomeapp;

import static java.lang.String.valueOf;

import android.content.Context;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.GenericTypeIndicator;
import com.google.firebase.database.ValueEventListener;

import java.util.List;

public class Firebase {

    private FirebaseAuth mAuth;
    private DatabaseReference myRef;

    public static List<String> LIST1;
    public static List<String> LIST2;
    public static List<String> LIST3;
    public static List<String> LIST4;
    public static List<String> LIST5;
    public static List<String> LIST6;
    public static List<String> LIST7;
    public static List<String> LIST8;
    public static List<String> LIST9;
    public static List<String> LIST10;

    public static String street_temp;
    public static String street_hum;
    public static String home_temp;
    public static String home_hum;
    public static String wind_speed;
    public static String wind_direct;
    public static String v_bat;
    public static String amount_rain;
    public static String pressure;
    public static String time;

    public static boolean error = false;

    public void get_firebase(Context context, TextView textView1, TextView textView2, TextView textView3,
                             TextView textView4, TextView textView5, TextView textView6, TextView textView7, TextView textView8, TextView textView9, TextView textView10, ImageView imageview, ImageView weath_forecast)
    {
        myRef = FirebaseDatabase.getInstance().getReference();
        FirebaseUser user = mAuth.getInstance().getCurrentUser();


        myRef.child(user.getUid()).addValueEventListener(new ValueEventListener() {

            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                GenericTypeIndicator<List<String>> t = new GenericTypeIndicator<List<String>>() {};
                //заполнение полей главного окна
                try {
                    LIST1 = dataSnapshot.child("DATA").child("STREET_TEMP").getValue(t);
                    street_temp = LIST1.get(LIST1.size() - 1);
                    textView1.setText(street_temp);
                }
                catch(Exception e) {
                    error = true;
                }
                try {
                    LIST2 = dataSnapshot.child("DATA").child("STREET_HUM").getValue(t);
                    street_hum = LIST2.get(LIST2.size() - 1);
                    textView2.setText(street_hum + ".0");
                }
                catch(Exception e) {
                    error = true;
                }
                try {
                    LIST3 = dataSnapshot.child("DATA").child("RAIN").getValue(t);
                    for (int i = 0; i < LIST3.size(); i++) {
                        LIST3.set(i, valueOf(100 - 100 * Float.parseFloat(LIST3.get(i))).toString());
                    }
                    amount_rain = LIST3.get(LIST3.size() - 1);
                    //textView3.setText(valueOf(100 - 100 * Float.parseFloat(amount_rain)).toString());
                    textView3.setText(amount_rain);
                }
                catch(Exception e) {
                    error = true;
                }
                try {
                    LIST4 = dataSnapshot.child("DATA").child("VBAT").getValue(t);
                    v_bat = LIST4.get(LIST4.size() - 1);
                    textView4.setText(v_bat);
                }
                catch(Exception e) {
                    error = true;
                }
                try {
                    LIST5 = dataSnapshot.child("DATA").child("WIND_DIRECT").getValue(t);
                    wind_direct = LIST5.get(LIST5.size() - 1);
                    textView6.setText(wind_direct);
                }
                catch(Exception e) {
                    error = true;
                }
                try {
                    LIST6 = dataSnapshot.child("DATA").child("WIND_SPEED").getValue(t);
                    wind_speed = LIST6.get(LIST6.size() - 1);
                    textView5.setText(wind_speed);
                }
                catch(Exception e) {
                    error = true;
                }
                try {
                    LIST7 = dataSnapshot.child("DATA").child("HOME_TEMP").getValue(t);
                    home_temp = LIST7.get(LIST7.size() - 1);
                    textView7.setText(home_temp);
                }
                catch(Exception e) {
                    error = true;
                }
                try{
                    LIST8 = dataSnapshot.child("DATA").child("HOME_HUM").getValue(t);
                    home_hum = LIST8.get(LIST8.size()-1);
                    textView8.setText(home_hum + ".0");
                }
                catch(Exception e) {
                    error = true;
                }
                try{
                    LIST9 = dataSnapshot.child("DATA").child("PRESSURE").getValue(t);
                    pressure = LIST9.get(LIST9.size()-1);
                    textView9.setText(pressure);
                }
                catch(Exception e) {
                    error = true;
                }
                try {
                    LIST10 = dataSnapshot.child("DATA").child("TIME").getValue(t);
                    time = LIST10.get(LIST10.size() - 1);
                    time = time.replace(',', '\n');
                    textView10.setText(time);
                    for (int i = 0; i < LIST10.size(); i++) {
                        LIST10.set(i, LIST10.get(i).substring(0, 8));
                    }
                }
                catch(Exception e) {
                    error = true;
                }
                //вывод индикатора направления ветра
                switch (wind_direct)
                {
                    case "N-W": imageview.setImageResource(R.drawable.nw);
                        break;
                    case "N": imageview.setImageResource(R.drawable.n);
                        break;
                    case "S": imageview.setImageResource(R.drawable.s);
                        break;
                    case "E": imageview.setImageResource(R.drawable.e);
                        break;
                    case "W": imageview.setImageResource(R.drawable.w);
                        break;
                    case "N-E": imageview.setImageResource(R.drawable.ne);
                        break;
                    case "S-W": imageview.setImageResource(R.drawable.sw);
                        break;
                    case "S-E": imageview.setImageResource(R.drawable.se);
                        break;
                    default: imageview.setImageResource(R.drawable.def);
                        break;
                }
                //вывод индикатора прогноза погоды
                if (!error)
                {
                    if((Float.parseFloat(wind_speed) == 0f) && (Float.parseFloat(amount_rain) <= 10f))
                    {
                        weath_forecast.setImageResource(R.drawable.weath_sunny);
                    }
                    else if ((Float.parseFloat(wind_speed) >= 10f) && (Float.parseFloat(amount_rain) <= 10f))
                    {
                        weath_forecast.setImageResource(R.drawable.weath_only_clouds);
                    }
                    else if ((Float.parseFloat(wind_speed) < 10f) && (Float.parseFloat(amount_rain) <= 10f))
                    {
                        weath_forecast.setImageResource(R.drawable.weath_cloud);
                    }
                    else if ((Float.parseFloat(amount_rain) < 90f) && (Float.parseFloat(amount_rain) > 15f))
                    {
                        weath_forecast.setImageResource(R.drawable.weath_rainy);
                    }
                    else
                    {

                    }
                }
                if (error)
                {
                    Toast.makeText(context, "Ошибка в базе данных! Отсутствуют некоторые показания!", Toast.LENGTH_SHORT).show();
                }
            }
            @Override
            public void onCancelled(DatabaseError databaseError) {

            }
        });
    }

}