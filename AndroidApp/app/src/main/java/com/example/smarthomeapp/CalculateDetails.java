package com.example.smarthomeapp;

import java.util.ArrayList;
import java.util.List;

public class CalculateDetails {
    public static float Biggest = 0f;
    public static float Smallest = Float.MAX_VALUE;

    public static int NW_Count = 0;
    public static int N_Count = 0;
    public static int S_Count = 0;
    public static int E_Count = 0;
    public static int W_Count = 0;
    public static int NE_Count = 0;
    public static int SW_Count = 0;
    public static int SE_Count = 0;


    public float CalculateAverage(List<String> LIST)
    {
        float Average = 0f;
        for(int i = 0; i < LIST.size(); i++)
        {
            Average += Float.parseFloat(LIST.get(i));
        }
        Average /= LIST.size();
        return Average;
    }
    public float CalculateMax(List<String> LIST)
    {
        Biggest = 0f;
        float Max = 0f;
        ArrayList<Float> ArrayList = new ArrayList<Float>();
        for (int i = 0; i < LIST.size(); i++)
        {
            ArrayList.add(i, Float.parseFloat(LIST.get(i)));
        }
        Max = find_Max(ArrayList);
        return Max;
    }
    public static float find_Max(ArrayList<Float> ArrayList)
    {
        if (ArrayList.size() == 0)
        {
            return Biggest;
        }
        else
        {
            if (ArrayList.get(0) > Biggest)
            {
                Biggest = ArrayList.get(0);
            }
            ArrayList.addAll(decrease_arraylist(ArrayList));
            return find_Max(ArrayList);
        }
    }
    public static ArrayList<Float> decrease_arraylist(ArrayList<Float> ArrayList)
    {
        ArrayList<Float> newArrayList = new ArrayList<Float>();

        for (int i = 0; i < ArrayList.size()-1; i++)
        {
            newArrayList.add(i,ArrayList.get(i+1));
        }
        ArrayList.clear();
        return newArrayList;
    }
    public float CalculateMin(List<String> LIST)
    {
        Smallest = Float.MAX_VALUE;
        float Min = 0f;
        ArrayList<Float> ArrayList = new ArrayList<Float>();
        for (int i = 0; i < LIST.size(); i++)
        {
            ArrayList.add(i, Float.parseFloat(LIST.get(i)));
        }
        Min = find_Min(ArrayList);
        return Min;
    }
    public static float find_Min(ArrayList<Float> ArrayList)
    {
        if (ArrayList.size() == 0)
        {
            return Smallest;
        }
        else
        {
            if (ArrayList.get(0) < Smallest)
            {
                Smallest = ArrayList.get(0);
            }
            ArrayList.addAll(decrease_arraylist(ArrayList));
            return find_Min(ArrayList);
        }
    }
    public static void Find_Wind_Direct_Percent(List<String> LIST)
    {
        NW_Count = 0;
        N_Count = 0;
        S_Count = 0;
        E_Count = 0;
        W_Count = 0;
        NE_Count = 0;
        SW_Count = 0;
        SE_Count = 0;
        for(int i = 0; i < LIST.size(); i++)
        {
            switch (LIST.get(i))
            {
                case "N-W": NW_Count++;
                    break;
                case "N": N_Count++;
                    break;
                case "S": S_Count++;
                    break;
                case "E": E_Count++;
                    break;
                case "W": W_Count++;
                    break;
                case "N-E": NE_Count++;
                    break;
                case "S-W": SW_Count++;
                    break;
                case "S-E": SE_Count++;
                    break;
            }
        }
        /*NW_Count /= LIST.size();
        NW_Count *= 100;
        N_Count /= LIST.size();
        N_Count *= 100;
        S_Count /= LIST.size();
        S_Count *= 100;
        E_Count /= LIST.size();
        E_Count *= 100;
        W_Count /= LIST.size();
        W_Count *= 100;
        NE_Count /= LIST.size();
        NE_Count *= 100;
        SW_Count /= LIST.size();
        SW_Count *= 100;
        SE_Count /= LIST.size();
        SE_Count *= 100;*/
    }
}
