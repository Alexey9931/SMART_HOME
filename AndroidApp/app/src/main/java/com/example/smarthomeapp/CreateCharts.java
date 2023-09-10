package com.example.smarthomeapp;

import android.content.Context;
import android.graphics.Color;
import android.view.Gravity;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.charts.PieChart;
import com.github.mikephil.charting.components.Legend;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.data.PieData;
import com.github.mikephil.charting.data.PieDataSet;
import com.github.mikephil.charting.data.PieEntry;
import com.github.mikephil.charting.formatter.IndexAxisValueFormatter;

import java.util.ArrayList;
import java.util.List;

public class CreateCharts {
    public void Fill_Street_Temp_Chart(LineChart chart)
    {
        ArrayList<Entry> entries = new ArrayList<>();
        for (int i = 0; i < BackgroundWorker.LIST1.size();i++)
        {
            entries.add(new Entry(i, Float.parseFloat(BackgroundWorker.LIST1.get(i))));
        }

        // На основании массива точек создадим первую линию с названием
        LineDataSet dataset = new LineDataSet(entries, "Уличная температура, °C");
        dataset.setDrawFilled(true);
        dataset.setFillColor(Color.RED);
        dataset.setColor(Color.RED);
        dataset.setCircleColor(Color.RED);
        // Создадим переменную данных для графика
        LineData data = new LineData(dataset);
        // Передадим данные для графика в сам график
        chart.setData(data);
        chart.animateX(500);
        chart.setDrawBorders(true);
        chart.setDrawMarkers(true);
        chart.getDescription().setEnabled(false);

        //chart.setVisibleXRangeMaximum(4);
        //chart.setScaleXEnabled(false);

        YAxis yAxisRight = chart.getAxisRight();
        yAxisRight.setTextColor(Color.WHITE);

        YAxis yAxis = chart.getAxisLeft();
        yAxis.setTextSize(12f);

        XAxis xAxis = chart.getXAxis();
        xAxis.setValueFormatter(new IndexAxisValueFormatter(BackgroundWorker.LIST11));
        xAxis.setGranularityEnabled(true);
        xAxis.setLabelCount(3, false);
        xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);
        xAxis.setDrawGridLines(true);
        xAxis.setTextSize(12f);
        // Не забудем отправить команду на перерисовку кадра, иначе график не отобразится
        chart.invalidate();
    }
    public void Fill_Street_HUM_Chart(LineChart chart)
    {
        ArrayList<Entry> entries = new ArrayList<>();
        for (int i = 0; i < BackgroundWorker.LIST2.size();i++)
        {
            entries.add(new Entry(i, Float.parseFloat(BackgroundWorker.LIST2.get(i))));
        }

        // На основании массива точек создадим первую линию с названием
        LineDataSet dataset = new LineDataSet(entries, "Уличная влажность, %");
        dataset.setDrawFilled(true);
        dataset.setColor(Color.BLUE);
        dataset.setFillColor(Color.BLUE);
        dataset.setCircleColor(Color.BLUE);
        // Создадим переменную данных для графика
        LineData data = new LineData(dataset);
        // Передадим данные для графика в сам график
        chart.setData(data);
        chart.animateX(500);
        chart.setDrawBorders(true);
        chart.setDrawMarkers(true);
        chart.getDescription().setEnabled(false);

        //chart.setVisibleXRangeMaximum(4);
        //chart.setScaleXEnabled(false);

        YAxis yAxisRight = chart.getAxisRight();
        yAxisRight.setTextColor(Color.WHITE);

        YAxis yAxis = chart.getAxisLeft();
        yAxis.setTextSize(12f);

        XAxis xAxis = chart.getXAxis();
        xAxis.setValueFormatter(new IndexAxisValueFormatter(BackgroundWorker.LIST11));
        xAxis.setGranularityEnabled(true);
        xAxis.setLabelCount(3, false);
        xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);
        xAxis.setDrawGridLines(true);
        xAxis.setTextSize(12f);
        // Не забудем отправить команду на перерисовку кадра, иначе график не отобразится
        chart.invalidate();
    }
    public void Fill_Rain_Chart(LineChart chart)
    {
        ArrayList<Entry> entries = new ArrayList<>();
        for (int i = 0; i < BackgroundWorker.LIST3.size();i++)
        {
            //entries.add(new Entry(i, 100 - 100 * Float.parseFloat(Firebase.LIST3.get(i))));
            entries.add(new Entry(i, Float.parseFloat(BackgroundWorker.LIST3.get(i))));
        }

        // На основании массива точек создадим первую линию с названием
        LineDataSet dataset = new LineDataSet(entries, "Количество осадков, %");
        dataset.setDrawFilled(true);
        dataset.setColor(Color.GREEN);
        dataset.setFillColor(Color.GREEN);
        dataset.setCircleColor(Color.GREEN);
        // Создадим переменную данных для графика
        LineData data = new LineData(dataset);
        // Передадим данные для графика в сам график
        chart.setData(data);
        chart.animateX(500);
        chart.setDrawBorders(true);
        chart.setDrawMarkers(true);
        chart.getDescription().setEnabled(false);

        //chart.setVisibleXRangeMaximum(4);
        //chart.setScaleXEnabled(false);

        YAxis yAxisRight = chart.getAxisRight();
        yAxisRight.setTextColor(Color.WHITE);

        YAxis yAxis = chart.getAxisLeft();
        yAxis.setTextSize(12f);

        XAxis xAxis = chart.getXAxis();
        xAxis.setValueFormatter(new IndexAxisValueFormatter(BackgroundWorker.LIST11));
        xAxis.setGranularityEnabled(true);
        xAxis.setLabelCount(3, false);
        xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);
        xAxis.setDrawGridLines(true);
        xAxis.setTextSize(12f);
        // Не забудем отправить команду на перерисовку кадра, иначе график не отобразится
        chart.invalidate();
    }
    public void Fill_VBAT_Chart(LineChart chart)
    {
        ArrayList<Entry> entries = new ArrayList<>();
        for (int i = 0; i < BackgroundWorker.LIST4.size();i++)
        {
            entries.add(new Entry(i, Float.parseFloat(BackgroundWorker.LIST4.get(i))));
        }

        // На основании массива точек создадим первую линию с названием
        LineDataSet dataset = new LineDataSet(entries, "Заряд аккумулятора, В");
        dataset.setDrawFilled(true);
        dataset.setColor(Color.CYAN);
        dataset.setFillColor(Color.CYAN);
        dataset.setCircleColor(Color.CYAN);
        // Создадим переменную данных для графика
        LineData data = new LineData(dataset);
        // Передадим данные для графика в сам график
        chart.setData(data);
        chart.animateX(500);
        chart.setDrawBorders(true);
        chart.setDrawMarkers(true);
        chart.getDescription().setEnabled(false);

        //chart.setVisibleXRangeMaximum(4);
        //chart.setScaleXEnabled(false);

        YAxis yAxisRight = chart.getAxisRight();
        yAxisRight.setTextColor(Color.WHITE);

        YAxis yAxis = chart.getAxisLeft();
        yAxis.setTextSize(12f);

        XAxis xAxis = chart.getXAxis();
        xAxis.setValueFormatter(new IndexAxisValueFormatter(BackgroundWorker.LIST11));
        xAxis.setGranularityEnabled(true);
        xAxis.setLabelCount(3, false);
        xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);
        xAxis.setDrawGridLines(true);
        xAxis.setTextSize(12f);
        // Не забудем отправить команду на перерисовку кадра, иначе график не отобразится
        chart.invalidate();
    }
    public void Fill_Wind_Direct_Table(TableLayout wind_direct_table, Context context) {
        //int ROWS = 3;
        int COLS = 4;

        //for (int i = 0; i < (Firebase.LIST10.size()/4) + 1; i++)
        int i = 0;
        int k = 0;
        while(k < (BackgroundWorker.LIST11.size()/4) + 1)
        {
            //строка с временем
            TableRow tableRow = new TableRow(context);
            tableRow.setBackgroundColor(Color.parseColor("#0079D6"));
            tableRow.setLayoutParams(new TableLayout.LayoutParams(
                    TableLayout.LayoutParams.MATCH_PARENT,
                    TableLayout.LayoutParams.WRAP_CONTENT));
            for (int j = 0; j < COLS; j++)
            {
                TextView textView1 = new TextView(context);
                textView1.setTextColor(Color.BLACK);
                textView1.setGravity(Gravity.CENTER);
                if ((j + COLS*k) < BackgroundWorker.LIST11.size())
                {
                    textView1.setText(BackgroundWorker.LIST11.get(j + COLS*k));
                }
                tableRow.addView(textView1, new TableRow.LayoutParams(
                        TableRow.LayoutParams.WRAP_CONTENT, TableRow.LayoutParams.WRAP_CONTENT, 0.5f));
            }
            wind_direct_table.addView(tableRow, i);
            i++;
            //строка с данными
            TableRow tableRow1 = new TableRow(context);
            tableRow1.setBackgroundColor(Color.parseColor("#DAE8FC"));
            tableRow1.setLayoutParams(new TableLayout.LayoutParams(
                    TableLayout.LayoutParams.MATCH_PARENT,
                    TableLayout.LayoutParams.WRAP_CONTENT));
            for (int j = 0; j < COLS; j++) {
                TextView textView2 = new TextView(context);
                textView2.setTextColor(Color.BLACK);
                textView2.setGravity(Gravity.CENTER);
                if ((j + COLS*k) < BackgroundWorker.LIST5.size())
                {
                    textView2.setText(BackgroundWorker.LIST5.get(j + COLS*k));
                }
                tableRow1.addView(textView2, new TableRow.LayoutParams(
                        TableRow.LayoutParams.WRAP_CONTENT, TableRow.LayoutParams.WRAP_CONTENT, 0.5f));
            }
            wind_direct_table.addView(tableRow1, i);
            k++;
            i++;
        }
    }
    public void Fill_Wind_Direct_Table_New(TableLayout wind_direct_table, Context context) {
        int total_amount = 6;//кол-во интервалов
        int num = 0;
        int ind = 0;
        List<String> PART1 = new ArrayList<>();
        List<String> data = new ArrayList<>();
        while(num < (BackgroundWorker.LIST11.size()/total_amount))
        {
            PART1.add(ind,BackgroundWorker.LIST5.get(num));
            num++;
            ind++;
        }
        data.add(0,BackgroundWorker.LIST11.get(0)+" - "+BackgroundWorker.LIST11.get(BackgroundWorker.LIST11.size()/total_amount));
        ind = 0;
        List<String> PART2 = new ArrayList<>();
        while(num < (2*BackgroundWorker.LIST11.size()/total_amount))
        {
            PART2.add(ind,BackgroundWorker.LIST5.get(num));
            num++;
            ind++;
        }
        data.add(1,BackgroundWorker.LIST11.get(1+BackgroundWorker.LIST11.size()/total_amount)+" - "+BackgroundWorker.LIST11.get(1+2*BackgroundWorker.LIST11.size()/total_amount));
        ind = 0;
        List<String> PART3 = new ArrayList<>();
        while(num < (3*BackgroundWorker.LIST11.size()/total_amount))
        {
            PART3.add(ind,BackgroundWorker.LIST5.get(num));
            num++;
            ind++;
        }
        data.add(2,BackgroundWorker.LIST11.get(1+2*BackgroundWorker.LIST11.size()/total_amount)+" - "+BackgroundWorker.LIST11.get(1+3*BackgroundWorker.LIST11.size()/total_amount));
        ind = 0;
        List<String> PART4 = new ArrayList<>();
        while(num < (4*BackgroundWorker.LIST11.size()/total_amount))
        {
            PART4.add(ind,BackgroundWorker.LIST5.get(num));
            num++;
            ind++;
        }
        data.add(3,BackgroundWorker.LIST11.get(1+3*BackgroundWorker.LIST11.size()/total_amount)+" - "+BackgroundWorker.LIST11.get(1+4*BackgroundWorker.LIST11.size()/total_amount));
        ind = 0;
        List<String> PART5 = new ArrayList<>();
        while(num < (5*BackgroundWorker.LIST11.size()/total_amount))
        {
            PART5.add(ind,BackgroundWorker.LIST5.get(num));
            num++;
            ind++;
        }
        data.add(4,BackgroundWorker.LIST11.get(1+4*BackgroundWorker.LIST11.size()/total_amount)+" - "+BackgroundWorker.LIST11.get(1+5*BackgroundWorker.LIST11.size()/total_amount));
        ind = 0;
        List<String> PART6 = new ArrayList<>();
        while(num < BackgroundWorker.LIST11.size())
        {
            PART6.add(ind,BackgroundWorker.LIST5.get(num));
            num++;
            ind++;
        }
        data.add(5,BackgroundWorker.LIST11.get(1+5*BackgroundWorker.LIST11.size()/total_amount)+" - "+BackgroundWorker.LIST11.get(BackgroundWorker.LIST11.size()-1));
        //int ROWS = 3;
        int COLS = 2;
        //for (int i = 0; i < (Firebase.LIST10.size()/4) + 1; i++)
        int i = 0;
        int k = 0;
        while(k < 3)
        {
            //строка с временем
            TableRow tableRow = new TableRow(context);
            tableRow.setBackgroundColor(Color.parseColor("#FF3700B3"));
            tableRow.setLayoutParams(new TableLayout.LayoutParams(TableLayout.LayoutParams.MATCH_PARENT, TableLayout.LayoutParams.WRAP_CONTENT));
            for (int j = 0; j < COLS; j++)
            {
                TextView textView1 = new TextView(context);
                textView1.setTextSize(13);
                textView1.setTextColor(Color.WHITE);
                textView1.setGravity(Gravity.CENTER);
                textView1.setText(data.get(j + COLS*k));
                tableRow.addView(textView1, new TableRow.LayoutParams(TableRow.LayoutParams.WRAP_CONTENT, TableRow.LayoutParams.WRAP_CONTENT, 0.5f));
            }
            wind_direct_table.addView(tableRow, i);
            i++;
            //строка с данными
            TableRow tableRow1 = new TableRow(context);
            tableRow1.setBackgroundColor(Color.parseColor("#FF804FF1"));
            tableRow1.setLayoutParams(new TableLayout.LayoutParams(TableLayout.LayoutParams.MATCH_PARENT, TableLayout.LayoutParams.WRAP_CONTENT));
            for (int j = 0; j < COLS; j++) {
                CalculateDetails calculateDetails = new CalculateDetails();
                switch (j + COLS*k)
                {
                    case 0:calculateDetails.Find_Wind_Direct_Percent(PART1);
                        break;
                    case 1:calculateDetails.Find_Wind_Direct_Percent(PART2);
                        break;
                    case 2:calculateDetails.Find_Wind_Direct_Percent(PART3);
                        break;
                    case 3:calculateDetails.Find_Wind_Direct_Percent(PART4);
                        break;
                    case 4:calculateDetails.Find_Wind_Direct_Percent(PART5);
                        break;
                    case 5:calculateDetails.Find_Wind_Direct_Percent(PART6);
                        break;
                }
                /*TextView textView2 = new TextView(context);
                textView2.setTextColor(Color.BLACK);
                textView2.setGravity(Gravity.CENTER);*/
                List<PieEntry> yVals = new ArrayList<>();
                List<Integer> colors = new ArrayList<>();

                PieChart WindDirectDiagram = new PieChart(context);
                WindDirectDiagram.setUsePercentValues(true);
                WindDirectDiagram.getDescription().setEnabled(false);
                WindDirectDiagram.getLegend().setTextColor(Color.WHITE);
                WindDirectDiagram.getLegend().setYOffset(15f);
                WindDirectDiagram.getLegend().setTextSize(10);
                WindDirectDiagram.getLegend().setHorizontalAlignment(Legend.LegendHorizontalAlignment.LEFT);
                WindDirectDiagram.getLegend().setWordWrapEnabled(true);

                if (calculateDetails.NW_Count != 0)
                {
                    yVals.add(new PieEntry((float)calculateDetails.NW_Count, "N-W"));
                    colors.add(Color.parseColor("#FF2A18"));
                }
                if (calculateDetails.N_Count != 0)
                {
                    yVals.add(new PieEntry((float)calculateDetails.N_Count, "N"));
                    colors.add(Color.parseColor("#FF9118"));
                }
                if (calculateDetails.S_Count != 0)
                {
                    yVals.add(new PieEntry((float)calculateDetails.S_Count, "S"));
                    colors.add(Color.parseColor("#FFE918"));
                }
                if (calculateDetails.E_Count != 0)
                {
                    yVals.add(new PieEntry((float)calculateDetails.E_Count, "E"));
                    colors.add(Color.parseColor("#ADFF18"));
                }
                if (calculateDetails.W_Count != 0)
                {
                    yVals.add(new PieEntry((float)calculateDetails.W_Count, "W"));
                    colors.add(Color.parseColor("#1AFF18"));
                }
                if (calculateDetails.NE_Count != 0)
                {
                    yVals.add(new PieEntry((float)calculateDetails.NE_Count, "N-E"));
                    colors.add(Color.parseColor("#18EBFF"));
                }
                if (calculateDetails.SW_Count != 0)
                {
                    yVals.add(new PieEntry((float)calculateDetails.SW_Count, "S-W"));
                    colors.add(Color.parseColor("#1829FF"));
                }
                if (calculateDetails.SE_Count != 0)
                {
                    yVals.add(new PieEntry((float)calculateDetails.SE_Count, "S-E"));
                    colors.add(Color.parseColor("#FF18ED"));
                }

                PieDataSet pieDataSet = new PieDataSet(yVals, "");
                pieDataSet.setColors(colors);
                pieDataSet.setValueTextSize(14f);
                PieData pieData = new PieData(pieDataSet);
                WindDirectDiagram.setData(pieData);
                tableRow1.addView(WindDirectDiagram, new TableRow.LayoutParams(500, 700, 0.5f));
            }
            wind_direct_table.addView(tableRow1, i);
            k++;
            i++;
        }
    }
    public void Fill_Wind_Speed_Chart(LineChart chart)
    {
        ArrayList<Entry> entries = new ArrayList<>();
        for (int i = 0; i < BackgroundWorker.LIST6.size();i++)
        {
            entries.add(new Entry(i, Float.parseFloat(BackgroundWorker.LIST6.get(i))));
        }

        // На основании массива точек создадим первую линию с названием
        LineDataSet dataset = new LineDataSet(entries, "Скорость ветра, м/с");
        dataset.setDrawFilled(true);
        dataset.setColor(Color.MAGENTA);
        dataset.setFillColor(Color.MAGENTA);
        dataset.setCircleColor(Color.MAGENTA);
        // Создадим переменную данных для графика
        LineData data = new LineData(dataset);
        // Передадим данные для графика в сам график
        chart.setData(data);
        chart.animateX(500);
        chart.setDrawBorders(true);
        chart.setDrawMarkers(true);
        chart.getDescription().setEnabled(false);

        //chart.setVisibleXRangeMaximum(4);
        //chart.setScaleXEnabled(false);

        YAxis yAxisRight = chart.getAxisRight();
        yAxisRight.setTextColor(Color.WHITE);

        YAxis yAxis = chart.getAxisLeft();
        yAxis.setTextSize(12f);

        XAxis xAxis = chart.getXAxis();
        xAxis.setValueFormatter(new IndexAxisValueFormatter(BackgroundWorker.LIST11));
        xAxis.setGranularityEnabled(true);
        xAxis.setLabelCount(3, false);
        xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);
        xAxis.setDrawGridLines(true);
        xAxis.setTextSize(12f);
        // Не забудем отправить команду на перерисовку кадра, иначе график не отобразится
        chart.invalidate();
    }
    public void Fill_Home_Temp_Chart(LineChart chart)
    {
        ArrayList<Entry> entries = new ArrayList<>();
        for (int i = 0; i < BackgroundWorker.LIST7.size();i++)
        {
            entries.add(new Entry(i, Float.parseFloat(BackgroundWorker.LIST7.get(i))));
        }

        // На основании массива точек создадим первую линию с названием
        LineDataSet dataset = new LineDataSet(entries, "Комнатная температура, °C");
        dataset.setDrawFilled(true);
        dataset.setColor(Color.LTGRAY);
        dataset.setFillColor(Color.LTGRAY);
        dataset.setCircleColor(Color.LTGRAY);
        // Создадим переменную данных для графика
        LineData data = new LineData(dataset);
        // Передадим данные для графика в сам график
        chart.setData(data);
        chart.animateX(500);
        chart.setDrawBorders(true);
        chart.setDrawMarkers(true);
        chart.getDescription().setEnabled(false);

        //chart.setVisibleXRangeMaximum(4);
        //chart.setScaleXEnabled(false);

        YAxis yAxisRight = chart.getAxisRight();
        yAxisRight.setTextColor(Color.WHITE);

        YAxis yAxis = chart.getAxisLeft();
        yAxis.setTextSize(12f);

        XAxis xAxis = chart.getXAxis();
        xAxis.setValueFormatter(new IndexAxisValueFormatter(BackgroundWorker.LIST10));
        xAxis.setGranularityEnabled(true);
        xAxis.setLabelCount(3, false);
        xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);
        xAxis.setDrawGridLines(true);
        xAxis.setTextSize(12f);
        // Не забудем отправить команду на перерисовку кадра, иначе график не отобразится
        chart.invalidate();
    }
    public void Fill_Home_Hum_Chart(LineChart chart)
    {
        ArrayList<Entry> entries = new ArrayList<>();
        for (int i = 0; i < BackgroundWorker.LIST8.size();i++)
        {
            entries.add(new Entry(i, Float.parseFloat(BackgroundWorker.LIST8.get(i))));
        }

        // На основании массива точек создадим первую линию с названием
        LineDataSet dataset = new LineDataSet(entries, "Комнатная влажность, %");
        dataset.setDrawFilled(true);
        dataset.setColor(Color.rgb(255, 150,0));
        dataset.setFillColor(Color.rgb(255, 150,0));
        dataset.setCircleColor(Color.rgb(255, 150,0));
        // Создадим переменную данных для графика
        LineData data = new LineData(dataset);
        // Передадим данные для графика в сам график
        chart.setData(data);
        chart.animateX(500);
        chart.setDrawBorders(true);
        chart.setDrawMarkers(true);
        chart.getDescription().setEnabled(false);

        //chart.setVisibleXRangeMaximum(4);
        //chart.setScaleXEnabled(false);

        YAxis yAxisRight = chart.getAxisRight();
        yAxisRight.setTextColor(Color.WHITE);

        YAxis yAxis = chart.getAxisLeft();
        yAxis.setTextSize(12f);

        XAxis xAxis = chart.getXAxis();
        xAxis.setValueFormatter(new IndexAxisValueFormatter(BackgroundWorker.LIST10));
        xAxis.setGranularityEnabled(true);
        xAxis.setLabelCount(3, false);
        xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);
        xAxis.setDrawGridLines(true);
        xAxis.setTextSize(12f);
        // Не забудем отправить команду на перерисовку кадра, иначе график не отобразится
        chart.invalidate();
    }
    public void Fill_Pressure_Chart(LineChart chart)
    {
        ArrayList<Entry> entries = new ArrayList<>();
        for (int i = 0; i < BackgroundWorker.LIST9.size();i++)
        {
            entries.add(new Entry(i, Float.parseFloat(BackgroundWorker.LIST9.get(i))));
        }

        // На основании массива точек создадим первую линию с названием
        LineDataSet dataset = new LineDataSet(entries, "Атмосферное давление, мм.рт.ст");
        dataset.setDrawFilled(true);
        dataset.setColor(Color.DKGRAY);
        dataset.setFillColor(Color.DKGRAY);
        dataset.setCircleColor(Color.DKGRAY);
        // Создадим переменную данных для графика
        LineData data = new LineData(dataset);
        // Передадим данные для графика в сам график
        chart.setData(data);
        chart.animateX(500);
        chart.setDrawBorders(true);
        chart.setDrawMarkers(true);
        chart.getDescription().setEnabled(false);

        //chart.setVisibleXRangeMaximum(4);
        //chart.setScaleXEnabled(false);

        YAxis yAxisRight = chart.getAxisRight();
        yAxisRight.setTextColor(Color.WHITE);

        YAxis yAxis = chart.getAxisLeft();
        yAxis.setTextSize(12f);

        XAxis xAxis = chart.getXAxis();
        xAxis.setValueFormatter(new IndexAxisValueFormatter(BackgroundWorker.LIST10));
        xAxis.setGranularityEnabled(true);
        xAxis.setLabelCount(3, false);
        xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);
        xAxis.setDrawGridLines(true);
        xAxis.setTextSize(12f);
        // Не забудем отправить команду на перерисовку кадра, иначе график не отобразится
        chart.invalidate();
    }

}

