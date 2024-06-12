package com.zybooks.plugorsink;
/**
 * Created by: Max Hoffman
 *
 * From: 2/21/2022 to 2/27/2022
 *
 * Note: I estimate I spent more than 8 hours on this project, probably around 10 hours. Most of this time was spent on trying to fix the main bug of this program and
 * trying to implement the SavedInstanceState to save my app on rotation. Also another big part of this project was balancing, this refers to balancing the difficulties.
 * This was redone multiple times to ensure a fun yet challenging difficulty system.
 */
import static com.zybooks.plugorsink.GameActivity.setDifficulty;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.RadioButton;
import android.widget.TextView;
import android.widget.ToggleButton;



public class MainActivity extends AppCompatActivity {


    static int TotalPts = 0;
    TextView textPts;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    }


    //Code from Zybooks on Lights Out, this method starts GameActivity
    public void onStartClick(View view) {

        Intent intent = new Intent(this, GameActivity.class);
        startActivity(intent);
    }

    //Similar code from above but this starts the HelpActivity
    public void onHelpClick(View view) {

        Intent intent = new Intent(this, HelpActivity.class);
        startActivity(intent);
    }


    //Based off of this: https://developer.android.com/guide/topics/ui/controls/radiobutton
    //This passes a value to a static method in GameActivity to set the difficulty
    @SuppressLint("NonConstantResourceId")
    public void onRadioButtonClicked(View view) {
        boolean isRadioButtonChecked = ((RadioButton) view).isChecked();

        switch (view.getId()) {
            case R.id.EasyB:
                if (isRadioButtonChecked) {
                    setDifficulty("E");
                }
                break;
            case R.id.MediumB:
                if (isRadioButtonChecked) {
                    setDifficulty("M");
                }
                break;
            case R.id.HardB:
                if (isRadioButtonChecked) {
                    setDifficulty("H");
                }
                break;


        }
    }



}