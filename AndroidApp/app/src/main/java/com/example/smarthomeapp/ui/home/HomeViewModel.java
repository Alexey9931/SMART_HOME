package com.example.smarthomeapp.ui.home;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class HomeViewModel extends ViewModel {

    private final MutableLiveData<String> mText;
    public HomeViewModel() {
        mText = new MutableLiveData<>();
        //   mText.setValue(MainActivity.street_temp+" °С");
    }

    public LiveData<String> getText() {
        return mText;
    }
}