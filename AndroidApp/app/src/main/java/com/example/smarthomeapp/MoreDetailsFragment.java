package com.example.smarthomeapp;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;

import com.example.smarthomeapp.databinding.FragmentMoreDetailsBinding;
import com.github.mikephil.charting.charts.PieChart;

public class MoreDetailsFragment extends Fragment {
    private FragmentMoreDetailsBinding binding;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {

        binding = FragmentMoreDetailsBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        final PieChart WindDirectDiagram = root.findViewById(R.id.WindDirectDiagram);
        WindDirectDiagram.setVisibility(View.INVISIBLE);

        BackgroundWorker backgroundWorker = new BackgroundWorker(getContext());
        backgroundWorker.execute(   "http://alexgorlov99.ru/validateData-1day.php",
                "http://alexgorlov99.ru/validateData-3days.php",
                "http://alexgorlov99.ru/validateData-5days.php",
                "http://alexgorlov99.ru/validateData-7days.php","login");

        BackgroundWorkerForDetails backgroundWorkerForDetails = new BackgroundWorkerForDetails(getContext());
        backgroundWorkerForDetails.execute( root.findViewById(R.id.textView_avg_str_temp),root.findViewById(R.id.textView_avg_home_temp),
                                            root.findViewById(R.id.textView_avg_str_hum), root.findViewById(R.id.textView_avg_home_hum),
                                            root.findViewById(R.id.textView_avg_rainfall),root.findViewById(R.id.textView_avg_wind_speed),
                                            root.findViewById(R.id.textView_max_str_temp),root.findViewById(R.id.textView_max_home_temp),
                                            root.findViewById(R.id.textView_max_str_hum), root.findViewById(R.id.textView_max_home_hum),
                                            root.findViewById(R.id.textView_max_rainfall),root.findViewById(R.id.textView_max_wind_speed),
                                            root.findViewById(R.id.textView_max_bat_charge),root.findViewById(R.id.textView_min_str_temp),
                                            root.findViewById(R.id.textView_min_home_temp), root.findViewById(R.id.textView_min_str_hum),
                                            root.findViewById(R.id.textView_min_home_hum),root.findViewById(R.id.textView_min_rainfall),
                                            root.findViewById(R.id.textView_min_wind_speed),root.findViewById(R.id.textView_min_bat_charge),
                                            root.findViewById(R.id.WindDirectDiagram),root.findViewById(R.id.textView_start_time),
                                            root.findViewById(R.id.textView_end_time),root.findViewById(R.id.textView_number_mesuamer));



        return root;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}
