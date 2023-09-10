package com.example.smarthomeapp;

import static android.app.PendingIntent.getActivity;

import android.os.Bundle;
import android.text.util.Linkify;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.example.smarthomeapp.databinding.ActivityMainBinding;

public class AboutActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_about);

        TextView firebase_link = findViewById(R.id.github_link);
        firebase_link.setText("https://github.com/Alexey9931/HOME_WEATHER_STATION.git");
        Linkify.addLinks(firebase_link, Linkify.ALL);
    }

}
