package com.example.smarthomeapp;

import android.content.Context;
import android.os.AsyncTask;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TableLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.example.smarthomeapp.ui.chart_settings.ChartSettingFragment;
import com.github.mikephil.charting.charts.LineChart;

public class BackgroundWorkerForCharts extends AsyncTask<View,Void,String> {
    LineChart street_temp_chart;
    LineChart street_hum_chart;
    LineChart rain_chart;
    LineChart vbat_chart;
    LineChart wind_speed_chart;
    LineChart home_temp_chart;
    LineChart home_hum_chart;
    LineChart pressure_chart;
    TableLayout wind_direct_table;
    TextView error_message;

    TextView home_temp_name;
    TextView street_temp_name;
    TextView street_hum_name;
    TextView home_hum_name;
    TextView pressure_name;
    TextView wind_speed_name;
    TextView rain_name;
    TextView vbat_name;
    TextView table_name;

    int count = 0;

    Context context;
    //BackgroundWorker backgroundWorker = new BackgroundWorker(context);

    public BackgroundWorkerForCharts(Context ctx){
        context = ctx;
    }
    @Override
    protected String doInBackground(View... params) {
        street_temp_chart = (LineChart) params[0];
        street_hum_chart = (LineChart) params[1];
        rain_chart = (LineChart) params[2];
        vbat_chart = (LineChart) params[3];
        wind_speed_chart = (LineChart) params[4];
        home_temp_chart = (LineChart) params[5];
        home_hum_chart = (LineChart) params[6];
        pressure_chart = (LineChart) params[7];
        wind_direct_table = (TableLayout) params[8];
        error_message = (TextView) params[9];
        home_temp_name = (TextView) params[10];
        street_temp_name = (TextView) params[11];
        street_hum_name = (TextView) params[12];
        home_hum_name = (TextView) params[13];
        pressure_name = (TextView) params[14];
        wind_speed_name = (TextView) params[15];
        rain_name = (TextView) params[16];
        vbat_name = (TextView) params[17];
        table_name = (TextView) params[18];

        return null;
    }

    @Override
    protected void onPreExecute() {

    }

    @Override
    protected void onPostExecute(String result) {
        String mode = ChartSettingFragment.choose_chart_mode;
        if ((BackgroundWorker.LIST1.size() == 0) && (BackgroundWorker.LIST2.size() == 0) && (BackgroundWorker.LIST3.size() == 0) && (BackgroundWorker.LIST4.size() == 0) && (BackgroundWorker.LIST5.size() == 0) && (BackgroundWorker.LIST6.size() == 0) && (BackgroundWorker.LIST7.size() == 0) && (BackgroundWorker.LIST8.size() == 0) && (BackgroundWorker.LIST9.size() == 0) && (BackgroundWorker.LIST10.size() == 0))
        {
            Toast.makeText(context,"Ошибка отображения графиков! (База Данных пустая!)",Toast.LENGTH_LONG).show();
        }
        else if (mode == "nothing")
        {
            error_message.setVisibility(View.VISIBLE);
        }
        else if (mode == "all_other")
        {
            street_temp_chart.setVisibility(View.VISIBLE);
            street_hum_chart.setVisibility(View.VISIBLE);
            rain_chart.setVisibility(View.VISIBLE);
            vbat_chart.setVisibility(View.VISIBLE);
            wind_speed_chart.setVisibility(View.VISIBLE);
            home_temp_chart.setVisibility(View.VISIBLE);
            home_hum_chart.setVisibility(View.VISIBLE);
            pressure_chart.setVisibility(View.VISIBLE);
            wind_direct_table.setVisibility(View.VISIBLE);
            error_message.setVisibility(View.GONE);

            home_temp_name.setVisibility(View.VISIBLE);
            street_temp_name.setVisibility(View.VISIBLE);
            street_hum_name.setVisibility(View.VISIBLE);
            home_hum_name.setVisibility(View.VISIBLE);
            pressure_name.setVisibility(View.VISIBLE);
            wind_speed_name.setVisibility(View.VISIBLE);
            rain_name.setVisibility(View.VISIBLE);
            vbat_name.setVisibility(View.VISIBLE);
            table_name.setVisibility(View.VISIBLE);

            CreateCharts create_chart = new CreateCharts();

            RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(1300,900);
            layoutParams.leftMargin = 15;
            layoutParams.topMargin = 70;
            home_temp_chart.setLayoutParams(layoutParams);
            create_chart.Fill_Home_Temp_Chart(home_temp_chart);
            RelativeLayout.LayoutParams layoutParams0 = new RelativeLayout.LayoutParams(1300,150);
            layoutParams0.leftMargin = 15;
            layoutParams0.topMargin = -15;
            home_temp_name.setLayoutParams(layoutParams0);

            RelativeLayout.LayoutParams layoutParams1 = new RelativeLayout.LayoutParams(1300,900);
            layoutParams1.leftMargin = 15;
            layoutParams1.topMargin = 1070;
            street_temp_chart.setLayoutParams(layoutParams1);
            create_chart.Fill_Street_Temp_Chart(street_temp_chart);
            RelativeLayout.LayoutParams layoutParams11 = new RelativeLayout.LayoutParams(1300,150);
            layoutParams11.leftMargin = 15;
            layoutParams11.topMargin = 985;
            street_temp_name.setLayoutParams(layoutParams11);

            RelativeLayout.LayoutParams layoutParams2 = new RelativeLayout.LayoutParams(1300,900);
            layoutParams2.leftMargin = 15;
            layoutParams2.topMargin = 2070;
            street_hum_chart.setLayoutParams(layoutParams2);
            create_chart.Fill_Street_HUM_Chart(street_hum_chart);
            RelativeLayout.LayoutParams layoutParams22 = new RelativeLayout.LayoutParams(1300,150);
            layoutParams22.leftMargin = 15;
            layoutParams22.topMargin = 1985;
            street_hum_name.setLayoutParams(layoutParams22);

            RelativeLayout.LayoutParams layoutParams3 = new RelativeLayout.LayoutParams(1300,900);
            layoutParams3.leftMargin = 15;
            layoutParams3.topMargin = 3070;
            home_hum_chart.setLayoutParams(layoutParams3);
            create_chart.Fill_Home_Hum_Chart(home_hum_chart);
            RelativeLayout.LayoutParams layoutParams33 = new RelativeLayout.LayoutParams(1300,150);
            layoutParams33.leftMargin = 15;
            layoutParams33.topMargin = 2985;
            home_hum_name.setLayoutParams(layoutParams33);

            RelativeLayout.LayoutParams layoutParams4 = new RelativeLayout.LayoutParams(1300,900);
            layoutParams4.leftMargin = 15;
            layoutParams4.topMargin = 4070;
            pressure_chart.setLayoutParams(layoutParams4);
            create_chart.Fill_Pressure_Chart(pressure_chart);
            RelativeLayout.LayoutParams layoutParams44 = new RelativeLayout.LayoutParams(1300,150);
            layoutParams44.leftMargin = 15;
            layoutParams44.topMargin = 3985;
            pressure_name.setLayoutParams(layoutParams44);

            RelativeLayout.LayoutParams layoutParams5 = new RelativeLayout.LayoutParams(1300,900);
            layoutParams5.leftMargin = 15;
            layoutParams5.topMargin = 5070;
            wind_speed_chart.setLayoutParams(layoutParams5);
            create_chart.Fill_Wind_Speed_Chart(wind_speed_chart);
            RelativeLayout.LayoutParams layoutParams55 = new RelativeLayout.LayoutParams(1300,150);
            layoutParams55.leftMargin = 15;
            layoutParams55.topMargin = 4985;
            wind_speed_name.setLayoutParams(layoutParams55);

            RelativeLayout.LayoutParams layoutParams6 = new RelativeLayout.LayoutParams(1300,900);
            layoutParams6.leftMargin = 15;
            layoutParams6.topMargin = 6070;
            rain_chart.setLayoutParams(layoutParams6);
            create_chart.Fill_Rain_Chart(rain_chart);
            RelativeLayout.LayoutParams layoutParams66 = new RelativeLayout.LayoutParams(1300,150);
            layoutParams66.leftMargin = 15;
            layoutParams66.topMargin = 5985;
            rain_name.setLayoutParams(layoutParams66);

            RelativeLayout.LayoutParams layoutParams7 = new RelativeLayout.LayoutParams(1300,900);
            layoutParams7.leftMargin = 15;
            layoutParams7.topMargin = 7070;
            vbat_chart.setLayoutParams(layoutParams7);
            create_chart.Fill_VBAT_Chart(vbat_chart);
            RelativeLayout.LayoutParams layoutParams77 = new RelativeLayout.LayoutParams(1300,150);
            layoutParams77.leftMargin = 15;
            layoutParams77.topMargin = 6985;
            vbat_name.setLayoutParams(layoutParams77);

            RelativeLayout.LayoutParams layoutParams8 = new RelativeLayout.LayoutParams(1040,RelativeLayout.LayoutParams.WRAP_CONTENT);
            RelativeLayout.LayoutParams layoutParams9 = new RelativeLayout.LayoutParams(900,RelativeLayout.LayoutParams.WRAP_CONTENT);
            layoutParams8.leftMargin = 25;
            layoutParams8.topMargin = 8130;
            layoutParams9.leftMargin = 100;
            layoutParams9.topMargin = 8010;
            wind_direct_table.setLayoutParams(layoutParams8);
            create_chart.Fill_Wind_Direct_Table_New(wind_direct_table,context);
            table_name.setLayoutParams(layoutParams9);

        }
        else
        {
            error_message.setVisibility(View.GONE);
            if (ChartSettingFragment.TEMP_HOME_MODE == true)
            {
                RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(1300,900);
                layoutParams.leftMargin = 15;
                layoutParams.topMargin = 70 + 1000*count;
                home_temp_chart.setLayoutParams(layoutParams);
                RelativeLayout.LayoutParams layoutParams0 = new RelativeLayout.LayoutParams(1300,150);
                layoutParams0.leftMargin = 15;
                layoutParams0.topMargin = -15 + 1000*count;
                home_temp_name.setLayoutParams(layoutParams0);

                CreateCharts create_chart = new CreateCharts();
                home_temp_chart.setVisibility(View.VISIBLE);
                home_temp_name.setVisibility(View.VISIBLE);
                create_chart.Fill_Home_Temp_Chart(home_temp_chart);
                count++;
            }
            if (ChartSettingFragment.TEMP_STREET_MODE == true)
            {
                RelativeLayout.LayoutParams layoutParams1 = new RelativeLayout.LayoutParams(1300,900);
                layoutParams1.leftMargin = 15;
                layoutParams1.topMargin = 70 + 1000*count;
                street_temp_chart.setLayoutParams(layoutParams1);
                RelativeLayout.LayoutParams layoutParams11 = new RelativeLayout.LayoutParams(1300,150);
                layoutParams11.leftMargin = 15;
                layoutParams11.topMargin = -15 + 1000*count;
                street_temp_name.setLayoutParams(layoutParams11);

                CreateCharts create_chart = new CreateCharts();
                street_temp_chart.setVisibility(View.VISIBLE);
                street_temp_name.setVisibility(View.VISIBLE);
                create_chart.Fill_Street_Temp_Chart(street_temp_chart);
                count++;
            }
            if (ChartSettingFragment.HUM_STREET_MODE == true)
            {
                RelativeLayout.LayoutParams layoutParams2 = new RelativeLayout.LayoutParams(1300,900);
                layoutParams2.leftMargin = 15;
                layoutParams2.topMargin = 70 + 1000*count;
                street_hum_chart.setLayoutParams(layoutParams2);
                RelativeLayout.LayoutParams layoutParams22 = new RelativeLayout.LayoutParams(1300,150);
                layoutParams22.leftMargin = 15;
                layoutParams22.topMargin = -15 + 1000*count;
                street_hum_name.setLayoutParams(layoutParams22);

                CreateCharts create_chart = new CreateCharts();
                street_hum_chart.setVisibility(View.VISIBLE);
                street_hum_name.setVisibility(View.VISIBLE);
                create_chart.Fill_Street_HUM_Chart(street_hum_chart);
                count++;
            }
            if (ChartSettingFragment.HUM_HOME_MODE == true)
            {
                RelativeLayout.LayoutParams layoutParams3 = new RelativeLayout.LayoutParams(1300,900);
                layoutParams3.leftMargin = 15;
                layoutParams3.topMargin = 70 + 1000*count;
                home_hum_chart.setLayoutParams(layoutParams3);
                RelativeLayout.LayoutParams layoutParams33 = new RelativeLayout.LayoutParams(1300,150);
                layoutParams33.leftMargin = 15;
                layoutParams33.topMargin = -15 + 1000*count;
                home_hum_name.setLayoutParams(layoutParams33);

                CreateCharts create_chart = new CreateCharts();
                home_hum_chart.setVisibility(View.VISIBLE);
                home_hum_name.setVisibility(View.VISIBLE);
                create_chart.Fill_Home_Hum_Chart(home_hum_chart);
                count++;
            }
            if (ChartSettingFragment.PRESSURE_MODE == true)
            {
                RelativeLayout.LayoutParams layoutParams4 = new RelativeLayout.LayoutParams(1300,900);
                layoutParams4.leftMargin = 15;
                layoutParams4.topMargin = 70 + 1000*count;
                pressure_chart.setLayoutParams(layoutParams4);
                RelativeLayout.LayoutParams layoutParams44 = new RelativeLayout.LayoutParams(1300,150);
                layoutParams44.leftMargin = 15;
                layoutParams44.topMargin = -15 + 1000*count;
                pressure_name.setLayoutParams(layoutParams44);

                CreateCharts create_chart = new CreateCharts();
                pressure_chart.setVisibility(View.VISIBLE);
                pressure_name.setVisibility(View.VISIBLE);
                create_chart.Fill_Pressure_Chart(pressure_chart);
                count++;
            }
            if (ChartSettingFragment.WIND_SPEED_MODE == true)
            {
                RelativeLayout.LayoutParams layoutParams5 = new RelativeLayout.LayoutParams(1300,900);
                layoutParams5.leftMargin = 15;
                layoutParams5.topMargin = 70 + 1000*count;
                wind_speed_chart.setLayoutParams(layoutParams5);
                RelativeLayout.LayoutParams layoutParams55 = new RelativeLayout.LayoutParams(1300,150);
                layoutParams55.leftMargin = 15;
                layoutParams55.topMargin = -15 + 1000*count;
                wind_speed_name.setLayoutParams(layoutParams55);

                CreateCharts create_chart = new CreateCharts();
                wind_speed_chart.setVisibility(View.VISIBLE);
                wind_speed_name.setVisibility(View.VISIBLE);
                create_chart.Fill_Wind_Speed_Chart(wind_speed_chart);
                count++;
            }
            if (ChartSettingFragment.RAIN_MODE == true)
            {
                RelativeLayout.LayoutParams layoutParams6 = new RelativeLayout.LayoutParams(1300,900);
                layoutParams6.leftMargin = 15;
                layoutParams6.topMargin = 70 + 1000*count;
                rain_chart.setLayoutParams(layoutParams6);
                RelativeLayout.LayoutParams layoutParams66 = new RelativeLayout.LayoutParams(1300,150);
                layoutParams66.leftMargin = 15;
                layoutParams66.topMargin = -15 + 1000*count;
                rain_name.setLayoutParams(layoutParams66);

                CreateCharts create_chart = new CreateCharts();
                rain_chart.setVisibility(View.VISIBLE);
                rain_name.setVisibility(View.VISIBLE);
                create_chart.Fill_Rain_Chart(rain_chart);
                count++;
            }
            if (ChartSettingFragment.VBAT_MODE == true)
            {
                RelativeLayout.LayoutParams layoutParams7 = new RelativeLayout.LayoutParams(1300,900);
                layoutParams7.leftMargin = 15;
                layoutParams7.topMargin = 70 + 1000*count;
                vbat_chart.setLayoutParams(layoutParams7);
                RelativeLayout.LayoutParams layoutParams77 = new RelativeLayout.LayoutParams(1300,150);
                layoutParams77.leftMargin = 15;
                layoutParams77.topMargin = -15 + 1000*count;
                vbat_name.setLayoutParams(layoutParams77);

                CreateCharts create_chart = new CreateCharts();
                vbat_chart.setVisibility(View.VISIBLE);
                vbat_name.setVisibility(View.VISIBLE);
                create_chart.Fill_VBAT_Chart(vbat_chart);
                count++;
            }
            if (ChartSettingFragment.WIND_DIRECT_MODE == true)
            {
                RelativeLayout.LayoutParams layoutParams8 = new RelativeLayout.LayoutParams(1040,RelativeLayout.LayoutParams.WRAP_CONTENT);
                RelativeLayout.LayoutParams layoutParams9 = new RelativeLayout.LayoutParams(900,RelativeLayout.LayoutParams.WRAP_CONTENT);
                layoutParams8.leftMargin = 25;
                layoutParams8.topMargin = 100 + 1000*count+30;
                layoutParams9.leftMargin = 100;
                layoutParams9.topMargin = -15 + 1000*count;
                wind_direct_table.setLayoutParams(layoutParams8);
                table_name.setLayoutParams(layoutParams9);

                CreateCharts create_chart = new CreateCharts();
                wind_direct_table.setVisibility(View.VISIBLE);
                table_name.setVisibility(View.VISIBLE);
                create_chart.Fill_Wind_Direct_Table_New(wind_direct_table,context);
                count++;
            }
        }
    }
    @Override
    protected void onProgressUpdate(Void... values) {
        super.onProgressUpdate(values);

    }
}
