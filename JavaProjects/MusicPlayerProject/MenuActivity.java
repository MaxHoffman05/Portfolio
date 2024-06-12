package com.zybooks.musicbymood;
import static com.zybooks.musicbymood.MainActivity.getMoodInt;
import static com.zybooks.musicbymood.MainActivity.getMoodText;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

/**
 * Author: Max Hoffman
 *
 * Date: Worked on from 4/8/2022 - 4/13/2022
 * I believe that I did spend around 7 hours on this project, like what was predicted in the Creation Strategy
 *
 * IMPORTANT: I planned out the MVC Architecture and it can be found in the drawables folder, the file is called mvc_flow_chart.png
 */


//Controller Activity
public class MenuActivity extends AppCompatActivity {



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);

    }

    //button to open up the Music Player Activity
    public void onStartMusicActivity(View view) {
        Intent intent = new Intent(this, MusicPlayer.class);
        startActivity(intent);
    }

    //Button that brings up the mood dialog box, to allow the user to change the mood of the music.
    public void onMoodSelectorClick(View view) {
        //create the dialog box, and then we create a new thread that will run with the main thread, this thread shows the dialog and exectues the dialog's code
        //This allows the app to run drastically smoother and will prevent a frozen screen for a couple of seconds.
        MoodDialogFragment moodDialog = new MoodDialogFragment();
        Thread moodDialogThread = new Thread(() -> {

            moodDialog.show(getSupportFragmentManager(), "mood");
        });
        moodDialogThread.start();

    }




}
