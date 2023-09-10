package com.example.smarthomeapp;


import static com.example.smarthomeapp.ui.home.HomeFragment.time_error;
import static java.lang.Math.abs;
import static java.lang.String.valueOf;

import android.content.Context;
import android.graphics.Color;
import android.graphics.Typeface;
import android.os.AsyncTask;
import android.os.Build;
import android.view.Gravity;
import android.view.View;
import android.widget.ImageView;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.RequiresApi;

import com.example.smarthomeapp.ui.home.HomeFragment;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;
import java.text.DecimalFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

public class BackgroundWorker extends AsyncTask<String,Void,String> {
    public static List<String> LIST1 = new ArrayList<>();//заполнение StreetTemp
    public static List<String> LIST2 = new ArrayList<>();//заполнение StreetHum
    public static List<String> LIST3 = new ArrayList<>();//заполнение Rain
    public static List<String> LIST4 = new ArrayList<>();//заполнение BatteryCharge
    public static List<String> LIST5 = new ArrayList<>();//заполнение WindDirect
    public static List<String> LIST6 = new ArrayList<>();//заполнение WindSpeed
    public static List<String> LIST7 = new ArrayList<>();//заполнение HomeTemp
    public static List<String> LIST8 = new ArrayList<>();//заполнение HomeHum
    public static List<String> LIST9 = new ArrayList<>();//заполнение Pressuare
    public static List<String> LIST10 = new ArrayList<>();//заполнение Time для домаш параметров
    public static List<String> LIST11 = new ArrayList<>();//заполнение Time для уличн параметров

    public static int ChartDaysMode = 1;

    public static boolean StopFlag;
    public static String result = "";
    public static boolean EndFlag = false;
    CalculateDetails calculateDetails = new CalculateDetails();

    Context context;
    //AlertDialog alertDialog;
    DecimalFormat df = new DecimalFormat("#.#");
    public BackgroundWorker(Context ctx){
        context = ctx;
    }
    @Override
    protected String doInBackground(String... params) {
        String type = params[4];
        String login_url = "";
        int id = 1;
        String line = "";
        StopFlag = false;

        switch (ChartDaysMode) {
            case 1:
                login_url = params[0];
                break;
            case 3:
                login_url = params[1];
                break;
            case 5:
                login_url = params[2];
                break;
            case 7:
                login_url = params[3];
                break;
            default:
                break;
        }

        try {
            URL url = new URL(login_url);
            HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
            httpURLConnection.setRequestMethod("POST");
            httpURLConnection.setDoOutput(true);
            httpURLConnection.setDoInput(true);
            OutputStream outputStream = httpURLConnection.getOutputStream();
            BufferedWriter bufferedWriter = new BufferedWriter(new OutputStreamWriter(outputStream, "UTF-8"));

            String post_data = "";
            if (type.equals("register")) {
                post_data = URLEncoder.encode("id", "UTF-8") + "=" + URLEncoder.encode(Integer.toString(id), "UTF-8");
            } else if (type.equals("login")) {
                post_data = URLEncoder.encode("id", "UTF-8") + "=" + URLEncoder.encode(Integer.toString(id), "UTF-8");
            }

            bufferedWriter.write(post_data);
            bufferedWriter.flush();
            bufferedWriter.close();
            outputStream.close();
            InputStream inputStream = httpURLConnection.getInputStream();

            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream, "UTF-8"));
            result = "";
            line = "";
            while ((line = bufferedReader.readLine()) != null) {
                result += line;
            }
            bufferedReader.close();
            inputStream.close();
            httpURLConnection.disconnect();
            FillAllParamsNew(result);
        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return null;
    }

    @Override
    protected void onPreExecute() {
        //alertDialog = new AlertDialog.Builder(context).create();
        //alertDialog.setTitle("Login Status");
        LIST1.clear();
        LIST2.clear();
        LIST3.clear();
        LIST4.clear();
        LIST5.clear();
        LIST6.clear();
        LIST7.clear();
        LIST8.clear();
        LIST9.clear();
        LIST10.clear();
        LIST11.clear();
        Toast.makeText(context.getApplicationContext(),"Пожалуйста подождите, идет синхронизация с базой данных...",Toast.LENGTH_LONG).show();
    }

    @Override
    protected void onPostExecute(String result) {
        try {
            FillDisplayParam();
            DisplayWeathForecast();
            Toast.makeText(context.getApplicationContext(),"Загрузка информации с БД завершена!",Toast.LENGTH_LONG).show();
        }
        catch (Exception e)
        {
            Toast.makeText(context.getApplicationContext(),"ОШИБКА загрузки информации с БД!",Toast.LENGTH_LONG).show();
        }
        EndFlag = true;
    }
    @Override
    protected void onProgressUpdate(Void... values) {
        super.onProgressUpdate(values);
    }
    @RequiresApi(api = Build.VERSION_CODES.O)
    public void FillAllParamsNew(String result)
    {
        int flag = 0;
        int i = 0;
        int k = 0;
        int id = 1;//для домашних параметров
        int id1 = 1;//для уличных параметров, т.к. их кол-во может быть разным
        String Result;
        //for(int id = 1; id <= 2000; id++)
        while (i < result.length())
        {
            //заполнение HomeTemp
            while(result.charAt(i) != '|')
            {
                i++;
            }
            Result = result.substring(k,i);
            if (!Result.equals("NULL")) {LIST7.add(id-1, Result);}
            i++;
            k = i;
            //заполнение StreetTemp
            while(result.charAt(i) != '|')
            {
                i++;
            }
            Result = result.substring(k,i);
            if (!Result.equals("NULL")) {LIST1.add(id1-1, Result);}
            i++;
            k = i;
            //заполнение HomeHum
            while(result.charAt(i) != '|')
            {
                i++;
            }
            Result = result.substring(k,i);
            if (!Result.equals("NULL")) {LIST8.add(id-1, String.valueOf(Float.parseFloat(Result)));}
            i++;
            k = i;
            //заполнение StreetHum
            while(result.charAt(i) != '|')
            {
                i++;
            }
            Result = result.substring(k,i);
            if (!Result.equals("NULL")) {LIST2.add(id1-1, String.valueOf(Float.parseFloat(Result)));}
            i++;
            k = i;
            //заполнение Pressuare
            while(result.charAt(i) != '|')
            {
                i++;
            }
            Result = result.substring(k,i);
            if (!Result.equals("NULL")) {LIST9.add(id-1, Result);}
            i++;
            k = i;
            //заполнение WindSpeed
            while(result.charAt(i) != '|')
            {
                i++;
            }
            Result = result.substring(k,i);
            if (!Result.equals("NULL")) {LIST6.add(id1-1, Result);}
            i++;
            k = i;
            //заполнение WindDirect
            while(result.charAt(i) != '|')
            {
                i++;
            }
            Result = result.substring(k,i);
            if (!Result.equals("NULL")) {LIST5.add(id1-1, Result);}
            i++;
            k = i;
            //заполнение Rain
            while(result.charAt(i) != '|')
            {
                i++;
            }
            Result = result.substring(k,i);
            //if(100 - 100 * Float.parseFloat(Result) < 0f) Result = "1.0";
            if (!Result.equals("NULL")) {LIST3.add(id1-1, String.valueOf(Float.parseFloat(Result)));}
            i++;
            k = i;
            //заполнение BatteryCharge
            while(result.charAt(i) != '|')
            {
                i++;
            }
            Result = result.substring(k,i);
            if (!Result.equals("NULL")) {LIST4.add(id1-1, Result); id1++; flag = 1;}
            i++;
            k = i;
            //заполнение Time
            while(result.charAt(i) != '|')
            {
                i++;
            }
            Result = result.substring(k,i);
            if (!Result.equals("NULL")) {
                LIST10.add(id-1, Result);
                if (flag == 1)
                {
                    LIST11.add(id1-2, Result);
                    flag = 0;
                }
            }
            i=i+6;
            k = i;

            id++;
        }
        //Форматирование строки времени для отображение на главном окне
        String[] parts = LIST10.get(LIST10.size() - 1).split("/",3);
        switch(parts[1])
        {
            case "1":
                parts[1] = "Янв";
                break;
            case "2":
                parts[1] = "Фев";
                break;
            case "3":
                parts[1] = "Мар";
                break;
            case "4":
                parts[1] = "Апр";
                break;
            case "5":
                parts[1] = "Май";
                break;
            case "6":
                parts[1] = "Июн";
                break;
            case "7":
                parts[1] = "Июл";
                break;
            case "8":
                parts[1] = "Авг";
                break;
            case "9":
                parts[1] = "Сен";
                break;
            case "10":
                parts[1] = "Окт";
                break;
            case "11":
                parts[1] = "Ноя";
                break;
            case "12":
                parts[1] = "Дек";
                break;
        }
        //LIST10.set(LIST10.size() - 1, parts[0]+"/"+parts[1]+"/"+"20"+parts[2]);
        if (i != 0)
        {
            SimpleDateFormat format = new SimpleDateFormat("HH:mm:ss dd/MM/yy");
            HomeFragment.time_for_display = (parts[0]+"/"+parts[1]+"/"+"20"+parts[2]).replace(',', '\n');
            Date current_time = new Date();
            Date display_time = null;
            //System.out.println("time"+LIST10.get(LIST10.size() - 1).replace(',', ' '));
            try {
                display_time = (Date)format.parse( LIST10.get(LIST10.size() - 1).replace(',', ' '));
                long delta = abs((current_time.getTime()- display_time.getTime()))/1000;//разница в секундах
                System.out.println("time"+delta);
                if (delta > 600)
                {
                    time_error.setVisibility(View.VISIBLE);
                }
                else
                {
                    time_error.setVisibility(View.INVISIBLE);
                }
            } catch (ParseException e) {
                e.printStackTrace();
            }
        }
        //Форматирование времени для отображения на графиках
        for(int n = 0; n < LIST10.size(); n++)
        {
            String[] all_part = LIST10.get(n).replace(',', ' ').split(" ",2);
            String time = all_part[0];
            String date = all_part[1];
            String[] date_part = date.split("/", 3);
            switch(date_part[1])
            {
                case "1":
                    date_part[1] = "Янв";
                    break;
                case "2":
                    date_part[1] = "Фев";
                    break;
                case "3":
                    date_part[1] = "Мар";
                    break;
                case "4":
                    date_part[1] = "Апр";
                    break;
                case "5":
                    date_part[1] = "Май";
                    break;
                case "6":
                    date_part[1] = "Июн";
                    break;
                case "7":
                    date_part[1] = "Июл";
                    break;
                case "8":
                    date_part[1] = "Авг";
                    break;
                case "9":
                    date_part[1] = "Сен";
                    break;
                case "10":
                    date_part[1] = "Окт";
                    break;
                case "11":
                    date_part[1] = "Ноя";
                    break;
                case "12":
                    date_part[1] = "Дек";
                    break;
            }
            String[] time_part = time.split(":", 3);
            String hours = time_part[0];
            if (Integer.parseInt(hours) < 10) hours = "0"+hours;
            String minutes = time_part[1];
            if (Integer.parseInt(minutes) < 10) minutes = "0"+minutes;
            LIST10.set(n,hours+":"+minutes+" ("+date_part[0]+" "+date_part[1]+")");
        }
        for(int m = 0; m < LIST11.size(); m++)
        {
            String[] all_part = LIST11.get(m).replace(',', ' ').split(" ",2);
            String time = all_part[0];
            String date = all_part[1];
            String[] date_part = date.split("/", 3);
            switch(date_part[1])
            {
                case "1":
                    date_part[1] = "Янв";
                    break;
                case "2":
                    date_part[1] = "Фев";
                    break;
                case "3":
                    date_part[1] = "Мар";
                    break;
                case "4":
                    date_part[1] = "Апр";
                    break;
                case "5":
                    date_part[1] = "Май";
                    break;
                case "6":
                    date_part[1] = "Июн";
                    break;
                case "7":
                    date_part[1] = "Июл";
                    break;
                case "8":
                    date_part[1] = "Авг";
                    break;
                case "9":
                    date_part[1] = "Сен";
                    break;
                case "10":
                    date_part[1] = "Окт";
                    break;
                case "11":
                    date_part[1] = "Ноя";
                    break;
                case "12":
                    date_part[1] = "Дек";
                    break;
            }
            String[] time_part = time.split(":", 3);
            String hours = time_part[0];
            if (Integer.parseInt(hours) < 10) hours = "0"+hours;
            String minutes = time_part[1];
            if (Integer.parseInt(minutes) < 10) minutes = "0"+minutes;
            LIST11.set(m,hours+":"+minutes+" ("+date_part[0]+" "+date_part[1]+")");
        }

    }
    public void FillDisplayParam()
    {
        try {
            HomeFragment.HOME_TEMP.setText(LIST7.get(LIST7.size() - 1));
            if (!LIST1.get(LIST1.size() - 1).equals("NULL")) HomeFragment.STREET_TEMP.setText(LIST1.get(LIST1.size() - 1));
            else HomeFragment.STREET_TEMP.setText("");
            HomeFragment.HOME_HUM.setText(LIST8.get(LIST8.size() - 1));
            if (!LIST2.get(LIST2.size() - 1).equals("NULL")) HomeFragment.STREET_HUM.setText(LIST2.get(LIST2.size() - 1));
            else HomeFragment.STREET_HUM.setText("");
            if (!LIST3.get(LIST3.size() - 1).equals("NULL")) HomeFragment.RAIN.setText(LIST3.get(LIST3.size() - 1));
            else HomeFragment.RAIN.setText("");
            if (!LIST4.get(LIST4.size() - 1).equals("NULL")) HomeFragment.VBat.setText(LIST4.get(LIST4.size() - 1));
            else HomeFragment.VBat.setText("");
            if (!LIST6.get(LIST6.size() - 1).equals("NULL")) HomeFragment.WIND_SPEED.setText(LIST6.get(LIST6.size() - 1));
            else HomeFragment.WIND_SPEED.setText("");
            if (!LIST5.get(LIST5.size() - 1).equals("NULL")) HomeFragment.WIND_DIRECTION.setText(LIST5.get(LIST5.size() - 1));
            else HomeFragment.WIND_DIRECTION.setText("");
            HomeFragment.PRESSURE.setText(LIST9.get(LIST9.size() - 1));
            HomeFragment.TIME.setText(HomeFragment.time_for_display);
            //вывод индикатора направления ветра
            switch (LIST5.get(LIST5.size() - 1)) {
                case "N-W":
                    HomeFragment.imagewind.setImageResource(R.drawable.nw);
                    break;
                case "N":
                    HomeFragment.imagewind.setImageResource(R.drawable.n);
                    break;
                case "S":
                    HomeFragment.imagewind.setImageResource(R.drawable.s);
                    break;
                case "E":
                    HomeFragment.imagewind.setImageResource(R.drawable.e);
                    break;
                case "W":
                    HomeFragment.imagewind.setImageResource(R.drawable.w);
                    break;
                case "N-E":
                    HomeFragment.imagewind.setImageResource(R.drawable.ne);
                    break;
                case "S-W":
                    HomeFragment.imagewind.setImageResource(R.drawable.sw);
                    break;
                case "S-E":
                    HomeFragment.imagewind.setImageResource(R.drawable.se);
                    break;
                default:
                    HomeFragment.imagewind.setImageResource(R.drawable.def);
                    break;
            }
            //вывод индикатора прогноза погоды
            /*if ((Float.parseFloat(LIST6.get(LIST6.size() - 1)) == 0f) && (Float.parseFloat(LIST3.get(LIST3.size() - 1)) <= 10f)) {
                HomeFragment.weath_forecast.setImageResource(R.drawable.weath_sunny);
            } else if ((Float.parseFloat(LIST6.get(LIST6.size() - 1)) >= 10f) && (Float.parseFloat(LIST3.get(LIST3.size() - 1)) <= 10f)) {
                HomeFragment.weath_forecast.setImageResource(R.drawable.weath_only_clouds);
            } else if ((Float.parseFloat(LIST6.get(LIST6.size() - 1)) < 10f) && (Float.parseFloat(LIST3.get(LIST3.size() - 1)) <= 10f)) {
                HomeFragment.weath_forecast.setImageResource(R.drawable.weath_cloud);
            } else if ((Float.parseFloat(LIST3.get(LIST3.size() - 1)) < 90f) && (Float.parseFloat(LIST3.get(LIST3.size() - 1)) > 15f)) {
                HomeFragment.weath_forecast.setImageResource(R.drawable.weath_rainy);
            } else {

            }*/
        }
        catch (Exception e)
        {

        }
    }
    public void FillAllParams(String result, int id)
    {
        int i = 0;
        int k = 0;
        //заполнение HomeTemp
        while(result.charAt(i) != '|')
        {
            i++;
        }
        LIST7.add(id- 1, result.substring(k,i));
        i++;
        k = i;
        //заполнение StreetTemp
        while(result.charAt(i) != '|')
        {
            i++;
        }
        LIST1.add(id- 1, result.substring(k,i));
        i++;
        k = i;
        //заполнение HomeHum
        while(result.charAt(i) != '|')
        {
            i++;
        }
        LIST8.add(id- 1, result.substring(k,i));
        i++;
        k = i;
        //заполнение StreetHum
        while(result.charAt(i) != '|')
        {
            i++;
        }
        LIST2.add(id- 1, result.substring(k,i));
        i++;
        k = i;
        //заполнение Pressuare
        while(result.charAt(i) != '|')
        {
            i++;
        }
        LIST9.add(id- 1, result.substring(k,i));
        i++;
        k = i;
        //заполнение WindSpeed
        while(result.charAt(i) != '|')
        {
            i++;
        }
        LIST6.add(id- 1, result.substring(k,i));
        i++;
        k = i;
        //заполнение WindDirect
        while(result.charAt(i) != '|')
        {
            i++;
        }
        LIST5.add(id- 1, result.substring(k,i));
        i++;
        k = i;
        //заполнение Rain
        while(result.charAt(i) != '|')
        {
            i++;
        }
        LIST3.add(id- 1, result.substring(k,i));
        i++;
        k = i;
        //заполнение BatteryCharge
        while(result.charAt(i) != '|')
        {
            i++;
        }
        LIST4.add(id- 1, result.substring(k,i));
        i++;
        k = i;
        //заполнение Time
        while(result.charAt(i) != '|')
        {
            i++;
        }
        LIST10.add(id- 1, result.substring(k,i));
    }
    public void DisplayWeathForecast()
    {
        int total_amount = 4;//кол-во интервалов
        int num = 0;
        int ind = 0;

        List<String> PART1_str_temp = new ArrayList<>();//ул темп
        List<String> PART2_str_temp = new ArrayList<>();
        List<String> PART3_str_temp = new ArrayList<>();
        List<String> PART4_str_temp = new ArrayList<>();
        List<String> PART1_pres = new ArrayList<>();//давление
        List<String> PART2_pres = new ArrayList<>();
        List<String> PART3_pres = new ArrayList<>();
        List<String> PART4_pres = new ArrayList<>();
        List<String> PART1_rain = new ArrayList<>();//осадки
        List<String> PART2_rain = new ArrayList<>();
        List<String> PART3_rain = new ArrayList<>();
        List<String> PART4_rain = new ArrayList<>();
        List<String> PART1_speed = new ArrayList<>();//скорость
        List<String> PART2_speed = new ArrayList<>();
        List<String> PART3_speed = new ArrayList<>();
        List<String> PART4_speed = new ArrayList<>();
        List<String> data = new ArrayList<>();
        List<Float> street_temp = new ArrayList<>();
        List<Float> pressure = new ArrayList<>();
        List<Float> rain = new ArrayList<>();
        List<Float> speed = new ArrayList<>();

        while(num < (BackgroundWorker.LIST11.size()/total_amount))
        {
            PART1_str_temp.add(ind,BackgroundWorker.LIST1.get(num));
            PART1_pres.add(ind,BackgroundWorker.LIST9.get(num));
            PART1_rain.add(ind,BackgroundWorker.LIST3.get(num));
            PART1_speed.add(ind,BackgroundWorker.LIST6.get(num));
            num++;
            ind++;
        }
        street_temp.add(0,calculateDetails.CalculateAverage(PART1_str_temp));
        pressure.add(0,calculateDetails.CalculateAverage(PART1_pres));
        rain.add(0,calculateDetails.CalculateAverage(PART1_rain));
        speed.add(0,calculateDetails.CalculateAverage(PART1_speed));
        data.add(0,BackgroundWorker.LIST11.get(0).substring(0,5)+" - "+BackgroundWorker.LIST11.get(BackgroundWorker.LIST11.size()/total_amount).substring(0,5));
        ind = 0;
        while(num < (2*BackgroundWorker.LIST11.size()/total_amount))
        {
            PART2_str_temp.add(ind,BackgroundWorker.LIST1.get(num));
            PART2_pres.add(ind,BackgroundWorker.LIST9.get(num));
            PART2_rain.add(ind,BackgroundWorker.LIST3.get(num));
            PART2_speed.add(ind,BackgroundWorker.LIST6.get(num));
            num++;
            ind++;
        }
        street_temp.add(1,calculateDetails.CalculateAverage(PART2_str_temp));
        pressure.add(1,calculateDetails.CalculateAverage(PART2_pres));
        rain.add(1,calculateDetails.CalculateAverage(PART2_rain));
        speed.add(1,calculateDetails.CalculateAverage(PART2_speed));
        data.add(1,BackgroundWorker.LIST11.get(1+BackgroundWorker.LIST11.size()/total_amount).substring(0,5)+" - "+BackgroundWorker.LIST11.get(1+2*BackgroundWorker.LIST11.size()/total_amount).substring(0,5));
        ind = 0;
        while(num < (3*BackgroundWorker.LIST11.size()/total_amount))
        {
            PART3_str_temp.add(ind,BackgroundWorker.LIST1.get(num));
            PART3_pres.add(ind,BackgroundWorker.LIST9.get(num));
            PART3_rain.add(ind,BackgroundWorker.LIST3.get(num));
            PART3_speed.add(ind,BackgroundWorker.LIST6.get(num));
            num++;
            ind++;
        }
        street_temp.add(2,calculateDetails.CalculateAverage(PART3_str_temp));
        pressure.add(2,calculateDetails.CalculateAverage(PART3_pres));
        rain.add(2,calculateDetails.CalculateAverage(PART3_rain));
        speed.add(2,calculateDetails.CalculateAverage(PART3_speed));
        data.add(2,BackgroundWorker.LIST11.get(1+2*BackgroundWorker.LIST11.size()/total_amount).substring(0,5)+" - "+BackgroundWorker.LIST11.get(1+3*BackgroundWorker.LIST11.size()/total_amount).substring(0,5));
        ind = 0;
        while(num < (4*BackgroundWorker.LIST11.size()/total_amount))
        {
            PART4_str_temp.add(ind,BackgroundWorker.LIST1.get(num));
            PART4_pres.add(ind,BackgroundWorker.LIST9.get(num));
            PART4_rain.add(ind,BackgroundWorker.LIST3.get(num));
            PART4_speed.add(ind,BackgroundWorker.LIST6.get(num));
            num++;
            ind++;
        }
        street_temp.add(3,calculateDetails.CalculateAverage(PART4_str_temp));
        pressure.add(3,calculateDetails.CalculateAverage(PART4_pres));
        rain.add(3,calculateDetails.CalculateAverage(PART4_rain));
        speed.add(3,calculateDetails.CalculateAverage(PART4_speed));
        data.add(3,BackgroundWorker.LIST11.get(1+3*BackgroundWorker.LIST11.size()/total_amount).substring(0,5)+" - "+BackgroundWorker.LIST11.get(BackgroundWorker.LIST11.size()-1).substring(0,5));


        int COLS = 4;
        //for (int i = 0; i < (Firebase.LIST10.size()/4) + 1; i++)
        int i = 0;
        //строка с временем
        TableRow tableRow = new TableRow(context);
        tableRow.setBackgroundColor(Color.WHITE);
        tableRow.setLayoutParams(new TableLayout.LayoutParams(TableLayout.LayoutParams.MATCH_PARENT, TableLayout.LayoutParams.WRAP_CONTENT));
        for (int j = 0; j < COLS; j++)
        {
            TextView textView1 = new TextView(context);
            textView1.setTextSize(context.getResources().getDimension(R.dimen.text_size_13));
            textView1.setTextColor(Color.parseColor("#FF3700B3"));
            textView1.setTypeface(null, Typeface.BOLD_ITALIC);
            textView1.setGravity(Gravity.CENTER);
            textView1.setText(data.get(j));
            tableRow.addView(textView1, new TableRow.LayoutParams(TableRow.LayoutParams.WRAP_CONTENT, TableRow.LayoutParams.WRAP_CONTENT, 0.5f));
        }
        HomeFragment.Table.addView(tableRow, i);
        i++;
        //строка с картинками
        TableRow tableRow1 = new TableRow(context);
        tableRow1.setBackgroundColor(Color.parseColor("#FFFFFFFF"));
        tableRow1.setLayoutParams(new TableLayout.LayoutParams(TableLayout.LayoutParams.MATCH_PARENT, TableLayout.LayoutParams.WRAP_CONTENT));
        for (int j = 0; j < COLS; j++)
        {
            ImageView image = new ImageView(context);

            if ((speed.get(j) == 0f) && (rain.get(j) <= 10f))
            {
                image.setImageResource(R.drawable.weath_sunny);
            }
            else if ((speed.get(j) >= 10f) && (rain.get(j) <= 10f))
            {
                image.setImageResource(R.drawable.weath_only_clouds);
            }
            else if ((speed.get(j) < 10f) && (rain.get(j) <= 10f))
            {
                image.setImageResource(R.drawable.weath_cloud);
            }
            else if ((rain.get(j) < 90f) && (rain.get(j) > 15f))
            {
                image.setImageResource(R.drawable.weath_rainy);
            } else
            {
                image.setImageResource(R.drawable.weath_sunny);
            }

            //image.setImageResource(R.drawable.weath_sunny);

            tableRow1.addView(image, new TableRow.LayoutParams(500, 200, 0.5f));
        }
        HomeFragment.Table.addView(tableRow1, i);
        i++;
        //строка с ул температурой
        TableRow tableRow2 = new TableRow(context);
        tableRow2.setBackgroundColor(Color.WHITE);
        tableRow2.setLayoutParams(new TableLayout.LayoutParams(TableLayout.LayoutParams.MATCH_PARENT, TableLayout.LayoutParams.WRAP_CONTENT));
        for (int j = 0; j < COLS; j++)
        {
            TextView textView1 = new TextView(context);
            textView1.setTextSize(context.getResources().getDimension(R.dimen.text_size_13));
            textView1.setTextColor(Color.parseColor("#FF3700B3"));
            textView1.setTypeface(null, Typeface.BOLD);
            textView1.setGravity(Gravity.CENTER);
            textView1.setText(String.format(Locale.ROOT,"%.1f",street_temp.get(j)) + " °C");

            tableRow2.addView(textView1, new TableRow.LayoutParams(TableRow.LayoutParams.WRAP_CONTENT, TableRow.LayoutParams.WRAP_CONTENT, 0.5f));
        }
        HomeFragment.Table.addView(tableRow2, i);
        i++;
        //строка с давлением
        TableRow tableRow3 = new TableRow(context);
        tableRow3.setBackgroundColor(Color.WHITE);
        tableRow3.setLayoutParams(new TableLayout.LayoutParams(TableLayout.LayoutParams.MATCH_PARENT, TableLayout.LayoutParams.WRAP_CONTENT));
        for (int j = 0; j < COLS; j++)
        {
            TextView textView1 = new TextView(context);
            textView1.setTextSize(context.getResources().getDimension(R.dimen.text_size_13));
            textView1.setTextColor(Color.parseColor("#FF3700B3"));
            textView1.setTypeface(null, Typeface.BOLD);
            textView1.setGravity(Gravity.CENTER);
            textView1.setText(String.format(Locale.ROOT,"%.1f",pressure.get(j)) + " мм.р.с");

            tableRow3.addView(textView1, new TableRow.LayoutParams(TableRow.LayoutParams.WRAP_CONTENT, TableRow.LayoutParams.WRAP_CONTENT, 0.5f));
        }
        HomeFragment.Table.addView(tableRow3, i);
        i++;
        //строка с осадками
        TableRow tableRow4 = new TableRow(context);
        tableRow4.setBackgroundColor(Color.WHITE);
        tableRow4.setLayoutParams(new TableLayout.LayoutParams(TableLayout.LayoutParams.MATCH_PARENT, TableLayout.LayoutParams.WRAP_CONTENT));
        for (int j = 0; j < COLS; j++)
        {
            TextView textView1 = new TextView(context);
            textView1.setTextSize(context.getResources().getDimension(R.dimen.text_size_13));
            textView1.setTextColor(Color.parseColor("#FF3700B3"));
            textView1.setTypeface(null, Typeface.BOLD);
            textView1.setGravity(Gravity.CENTER);
            textView1.setText(String.format(Locale.ROOT,"%.1f",rain.get(j)) + " %");

            tableRow4.addView(textView1, new TableRow.LayoutParams(TableRow.LayoutParams.WRAP_CONTENT, TableRow.LayoutParams.WRAP_CONTENT, 0.5f));
        }
        HomeFragment.Table.addView(tableRow4, i);
        i++;
        //строка с скоростью
        TableRow tableRow5 = new TableRow(context);
        tableRow5.setBackgroundColor(Color.WHITE);
        tableRow5.setLayoutParams(new TableLayout.LayoutParams(TableLayout.LayoutParams.MATCH_PARENT, TableLayout.LayoutParams.WRAP_CONTENT));
        for (int j = 0; j < COLS; j++)
        {
            TextView textView1 = new TextView(context);
            textView1.setTextSize(context.getResources().getDimension(R.dimen.text_size_13));
            textView1.setTextColor(Color.parseColor("#FF3700B3"));
            textView1.setTypeface(null, Typeface.BOLD);
            textView1.setGravity(Gravity.CENTER);
            textView1.setText(String.format(Locale.ROOT,"%.1f",speed.get(j)) + " м/с");

            tableRow5.addView(textView1, new TableRow.LayoutParams(TableRow.LayoutParams.WRAP_CONTENT, TableRow.LayoutParams.WRAP_CONTENT, 0.5f));
        }
        HomeFragment.Table.addView(tableRow5, i);
        i++;
    }
}
