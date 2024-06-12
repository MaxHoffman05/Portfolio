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
import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

public class HelpActivity extends AppCompatActivity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.help_activity);

    }
}
