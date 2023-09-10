package com.example.smarthomeapp.ui.charts;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TableLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;

import com.example.smarthomeapp.BackgroundWorker;
import com.example.smarthomeapp.BackgroundWorkerForCharts;
import com.example.smarthomeapp.R;
import com.example.smarthomeapp.databinding.FragmentChartsBinding;
import com.github.mikephil.charting.charts.LineChart;

public class ChartsFragment extends Fragment {

    private FragmentChartsBinding binding;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {

        binding = FragmentChartsBinding.inflate(inflater, container, false);
        View root = binding.getRoot();


        //int count = 0;

        final LineChart street_temp_chart = root.findViewById(R.id.street_temp_chart);
        final LineChart street_hum_chart = root.findViewById(R.id.street_hum_chart);
        final LineChart rain_chart = root.findViewById(R.id.rain_chart);
        final LineChart vbat_chart = root.findViewById(R.id.vbat_chart);
        final LineChart wind_speed_chart = root.findViewById(R.id.wind_speed_chart);
        final LineChart home_temp_chart = root.findViewById(R.id.home_temp_chart);
        final LineChart home_hum_chart = root.findViewById(R.id.home_hum_chart);
        final LineChart pressure_chart = root.findViewById(R.id.pressure_chart);
        final TableLayout wind_direct_table = root.findViewById(R.id.table);
        final TextView error_message = root.findViewById(R.id.error_message);

        final TextView home_temp_name = root.findViewById(R.id.home_temp_name);
        final TextView street_temp_name = root.findViewById(R.id.street_temp_name);
        final TextView street_hum_name = root.findViewById(R.id.street_hum_name);
        final TextView home_hum_name = root.findViewById(R.id.home_hum_name);
        final TextView pressure_name = root.findViewById(R.id.pressure_name);
        final TextView wind_speed_name = root.findViewById(R.id.wind_speed_name);
        final TextView rain_name = root.findViewById(R.id.rain_name);
        final TextView vbat_name = root.findViewById(R.id.vbat_name);
        final TextView table_name = root.findViewById(R.id.table_name);

        BackgroundWorker backgroundWorker = new BackgroundWorker(getContext());
        backgroundWorker.execute(   "http://alexgorlov99.ru/validateData-1day.php",
                "http://alexgorlov99.ru/validateData-3days.php",
                "http://alexgorlov99.ru/validateData-5days.php",
                "http://alexgorlov99.ru/validateData-7days.php","login");

        street_temp_chart.setVisibility(View.GONE);
        street_hum_chart.setVisibility(View.GONE);
        rain_chart.setVisibility(View.GONE);
        vbat_chart.setVisibility(View.GONE);
        wind_speed_chart.setVisibility(View.GONE);
        home_temp_chart.setVisibility(View.GONE);
        home_hum_chart.setVisibility(View.GONE);
        pressure_chart.setVisibility(View.GONE);
        wind_direct_table.setVisibility(View.GONE);
        error_message.setVisibility(View.GONE);

        home_temp_name.setVisibility(View.GONE);
        street_temp_name.setVisibility(View.GONE);
        street_hum_name.setVisibility(View.GONE);
        home_hum_name.setVisibility(View.GONE);
        pressure_name.setVisibility(View.GONE);
        wind_speed_name.setVisibility(View.GONE);
        rain_name.setVisibility(View.GONE);
        vbat_name.setVisibility(View.GONE);
        table_name.setVisibility(View.GONE);

        BackgroundWorkerForCharts backgroundWorkerForCharts = new BackgroundWorkerForCharts(getContext());
        backgroundWorkerForCharts.execute(  root.findViewById(R.id.street_temp_chart),root.findViewById(R.id.street_hum_chart),
                                            root.findViewById(R.id.rain_chart), root.findViewById(R.id.vbat_chart),
                                            root.findViewById(R.id.wind_speed_chart),root.findViewById(R.id.home_temp_chart),
                                            root.findViewById(R.id.home_hum_chart),root.findViewById(R.id.pressure_chart),
                                            root.findViewById(R.id.table), root.findViewById(R.id.error_message),
                                            root.findViewById(R.id.home_temp_name),root.findViewById(R.id.street_temp_name),
                                            root.findViewById(R.id.street_hum_name),root.findViewById(R.id.home_hum_name),
                                            root.findViewById(R.id.pressure_name),root.findViewById(R.id.wind_speed_name),
                                            root.findViewById(R.id.rain_name),root.findViewById(R.id.vbat_name),
                                            root.findViewById(R.id.table_name));


        /*String mode = ChartSettingFragment.choose_chart_mode;
        if ((backgroundWorker.LIST1.size() == 0) && (backgroundWorker.LIST2.size() == 0) && (backgroundWorker.LIST3.size() == 0) && (backgroundWorker.LIST4.size() == 0) && (backgroundWorker.LIST5.size() == 0) && (backgroundWorker.LIST6.size() == 0) && (backgroundWorker.LIST7.size() == 0) && (backgroundWorker.LIST8.size() == 0) && (backgroundWorker.LIST9.size() == 0) && (backgroundWorker.LIST10.size() == 0))
        {
            Toast.makeText(getContext(),"Ошибка отображения графиков! (База Данных пустая!)",Toast.LENGTH_LONG).show();
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

            RelativeLayout.LayoutParams layoutParams8 = new RelativeLayout.LayoutParams(900,RelativeLayout.LayoutParams.WRAP_CONTENT);
            RelativeLayout.LayoutParams layoutParams9 = new RelativeLayout.LayoutParams(900,RelativeLayout.LayoutParams.WRAP_CONTENT);
            layoutParams8.leftMargin = 100;
            layoutParams8.topMargin = 8130;
            layoutParams9.leftMargin = 100;
            layoutParams9.topMargin = 8010;
            wind_direct_table.setLayoutParams(layoutParams8);
            create_chart.Fill_Wind_Direct_Table(wind_direct_table,getContext());
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
                RelativeLayout.LayoutParams layoutParams8 = new RelativeLayout.LayoutParams(900,RelativeLayout.LayoutParams.WRAP_CONTENT);
                RelativeLayout.LayoutParams layoutParams9 = new RelativeLayout.LayoutParams(900,RelativeLayout.LayoutParams.WRAP_CONTENT);
                layoutParams8.leftMargin = 100;
                layoutParams8.topMargin = 100 + 1000*count+30;
                layoutParams9.leftMargin = 100;
                layoutParams9.topMargin = -15 + 1000*count;
                wind_direct_table.setLayoutParams(layoutParams8);
                table_name.setLayoutParams(layoutParams9);

                CreateCharts create_chart = new CreateCharts();
                wind_direct_table.setVisibility(View.VISIBLE);
                table_name.setVisibility(View.VISIBLE);
                create_chart.Fill_Wind_Direct_Table(wind_direct_table,getContext());
                count++;
            }
        }*/

        return root;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}