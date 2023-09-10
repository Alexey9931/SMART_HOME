package com.example.smarthomeapp.ui.chart_settings;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;
import android.widget.RadioGroup;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;

import com.example.smarthomeapp.BackgroundWorker;
import com.example.smarthomeapp.R;
import com.example.smarthomeapp.databinding.FragmentChartSettingBinding;

public class ChartSettingFragment extends Fragment {

    CheckBox checkBox_view_mode1;
    CheckBox checkBox_TEMP_HOME;
    CheckBox checkBox_TEMP_STREET;
    CheckBox checkBox_HUM_STREET;
    CheckBox checkBox_HUM_HOME;
    CheckBox checkBox_PRESSURE;
    CheckBox checkBox_WIND_SPEED;
    CheckBox checkBox_WIND_DIRECT;
    CheckBox checkBox_RAIN;
    CheckBox checkBox_VBAT;

    private FragmentChartSettingBinding binding;

    public static String choose_chart_mode = "nothing";
    public static Boolean TEMP_HOME_MODE = false;
    public static Boolean TEMP_STREET_MODE = false;
    public static Boolean HUM_STREET_MODE = false;
    public static Boolean HUM_HOME_MODE = false;
    public static Boolean PRESSURE_MODE = false;
    public static Boolean WIND_SPEED_MODE = false;
    public static Boolean WIND_DIRECT_MODE = false;
    public static Boolean RAIN_MODE = false;
    public static Boolean VBAT_MODE = false;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {

        binding = FragmentChartSettingBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        checkBox_view_mode1 = root.findViewById(R.id.checkBox_view_mode1);
        checkBox_TEMP_HOME = root.findViewById(R.id.checkBox_TEMP_HOME);
        checkBox_TEMP_STREET = root.findViewById(R.id.checkBox_TEMP_STREET);
        checkBox_HUM_STREET = root.findViewById(R.id.checkBox_HUM_STREET);
        checkBox_HUM_HOME = root.findViewById(R.id.checkBox_HUM_HOME);
        checkBox_PRESSURE = root.findViewById(R.id.checkBox_PRESSURE);
        checkBox_WIND_SPEED = root.findViewById(R.id.checkBox_WIND_SPEED);
        checkBox_WIND_DIRECT = root.findViewById(R.id.checkBox_WIND_DIRECT);
        checkBox_RAIN = root.findViewById(R.id.checkBox_RAIN);
        checkBox_VBAT = root.findViewById(R.id.checkBox_VBAT);
        RadioGroup radioGroup_display_mode = root.findViewById(R.id.display_mode);

        checkBox_view_mode1.setOnClickListener(CheckBoxClickListener);
        checkBox_TEMP_HOME.setOnClickListener(CheckBoxClickListener);
        checkBox_TEMP_STREET.setOnClickListener(CheckBoxClickListener);
        checkBox_HUM_STREET.setOnClickListener(CheckBoxClickListener);
        checkBox_HUM_HOME.setOnClickListener(CheckBoxClickListener);
        checkBox_PRESSURE.setOnClickListener(CheckBoxClickListener);
        checkBox_WIND_SPEED.setOnClickListener(CheckBoxClickListener);
        checkBox_WIND_DIRECT.setOnClickListener(CheckBoxClickListener);
        checkBox_RAIN.setOnClickListener(CheckBoxClickListener);
        checkBox_VBAT.setOnClickListener(CheckBoxClickListener);

        radioGroup_display_mode.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                switch (checkedId) {
                    case R.id.radioButton_1day:
                        BackgroundWorker.ChartDaysMode = 1;
                        break;
                    case R.id.radioButton_3day:
                        BackgroundWorker.ChartDaysMode = 3;
                        break;
                    case R.id.radioButton_5day:
                        BackgroundWorker.ChartDaysMode = 5;
                        break;
                    case R.id.radioButton_7day:
                        BackgroundWorker.ChartDaysMode = 7;
                        break;
                    default:
                        break;
                }
            }
        });

        return root;
    }
    View.OnClickListener CheckBoxClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            CheckBox checkBox = (CheckBox) view;
            switch (checkBox.getId()) {
                case R.id.checkBox_view_mode1:
                    if (checkBox.isChecked()) {
                        choose_chart_mode = "all_other";
                        checkBox_TEMP_HOME.setChecked(true);
                        checkBox_TEMP_HOME.setEnabled(false);
                        checkBox_TEMP_STREET.setChecked(true);
                        checkBox_TEMP_STREET.setEnabled(false);
                        checkBox_HUM_STREET.setChecked(true);
                        checkBox_HUM_STREET.setEnabled(false);
                        checkBox_HUM_HOME.setChecked(true);
                        checkBox_HUM_HOME.setEnabled(false);
                        checkBox_PRESSURE.setChecked(true);
                        checkBox_PRESSURE.setEnabled(false);
                        checkBox_WIND_SPEED.setChecked(true);
                        checkBox_WIND_SPEED.setEnabled(false);
                        checkBox_WIND_DIRECT.setChecked(true);
                        checkBox_WIND_DIRECT.setEnabled(false);
                        checkBox_RAIN.setChecked(true);
                        checkBox_RAIN.setEnabled(false);
                        checkBox_VBAT.setChecked(true);
                        checkBox_VBAT.setEnabled(false);
                    } else {
                        choose_chart_mode = "";
                        checkBox_TEMP_HOME.setChecked(false);
                        checkBox_TEMP_HOME.setEnabled(true);
                        checkBox_TEMP_STREET.setChecked(false);
                        checkBox_TEMP_STREET.setEnabled(true);
                        checkBox_HUM_STREET.setChecked(false);
                        checkBox_HUM_STREET.setEnabled(true);
                        checkBox_HUM_HOME.setChecked(false);
                        checkBox_HUM_HOME.setEnabled(true);
                        checkBox_PRESSURE.setChecked(false);
                        checkBox_PRESSURE.setEnabled(true);
                        checkBox_WIND_SPEED.setChecked(false);
                        checkBox_WIND_SPEED.setEnabled(true);
                        checkBox_WIND_DIRECT.setChecked(false);
                        checkBox_WIND_DIRECT.setEnabled(true);
                        checkBox_RAIN.setChecked(false);
                        checkBox_RAIN.setEnabled(true);
                        checkBox_VBAT.setChecked(false);
                        checkBox_VBAT.setEnabled(true);
                        TEMP_HOME_MODE = false;
                        TEMP_STREET_MODE = false;
                        HUM_STREET_MODE = false;
                        HUM_HOME_MODE = false;
                        PRESSURE_MODE = false;
                        WIND_SPEED_MODE = false;
                        WIND_DIRECT_MODE = false;
                        RAIN_MODE = false;
                        VBAT_MODE = false;
                    }
                    break;
                case R.id.checkBox_TEMP_HOME:
                    choose_chart_mode = "";
                    if (checkBox.isChecked()) {
                        TEMP_HOME_MODE = true;
                    }
                    else
                    {
                        TEMP_HOME_MODE = false;
                    }
                    break;
                case R.id.checkBox_TEMP_STREET:
                    choose_chart_mode = "";
                    if (checkBox.isChecked()) {
                        TEMP_STREET_MODE = true;
                    }
                    else
                    {
                        TEMP_STREET_MODE = false;
                    }
                    break;
                case R.id.checkBox_HUM_STREET:
                    choose_chart_mode = "";
                    if (checkBox.isChecked()) {
                        HUM_STREET_MODE = true;
                    }
                    else
                    {
                        HUM_STREET_MODE = false;
                    }
                    break;
                case R.id.checkBox_HUM_HOME:
                    choose_chart_mode = "";
                    if (checkBox.isChecked()) {
                        HUM_HOME_MODE = true;
                    }
                    else
                    {
                        HUM_HOME_MODE = false;
                    }
                    break;
                case R.id.checkBox_PRESSURE:
                    choose_chart_mode = "";
                    if (checkBox.isChecked()) {
                        PRESSURE_MODE = true;
                    }
                    else
                    {
                        PRESSURE_MODE = false;
                    }
                    break;
                case R.id.checkBox_WIND_SPEED:
                    choose_chart_mode = "";
                    if (checkBox.isChecked()) {
                        WIND_SPEED_MODE = true;
                    }
                    else
                    {
                        WIND_SPEED_MODE = false;
                    }
                    break;
                case R.id.checkBox_WIND_DIRECT:
                    choose_chart_mode = "";
                    if (checkBox.isChecked()) {
                        WIND_DIRECT_MODE = true;
                    }
                    else
                    {
                        WIND_DIRECT_MODE = false;
                    }
                    break;
                case R.id.checkBox_RAIN:
                    choose_chart_mode = "";
                    if (checkBox.isChecked()) {
                        RAIN_MODE = true;
                    }
                    else
                    {
                        RAIN_MODE = false;
                    }
                    break;
                case R.id.checkBox_VBAT:
                    choose_chart_mode = "";
                    if (checkBox.isChecked()) {
                        VBAT_MODE = true;
                    }
                    else
                    {
                        VBAT_MODE = false;
                    }
                    break;
            }
        }
    };
    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}