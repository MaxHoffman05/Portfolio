package com.zybooks.musicbymood;


import static com.zybooks.musicbymood.MainActivity.getAlbumArt;
import static com.zybooks.musicbymood.MainActivity.getMoodInt;
import static com.zybooks.musicbymood.MainActivity.getMoodText;
import static com.zybooks.musicbymood.MainActivity.getSongArtist;
import static com.zybooks.musicbymood.MainActivity.getSongID;
import static com.zybooks.musicbymood.MainActivity.getSongName;


import android.animation.ValueAnimator;
import android.graphics.drawable.AnimationDrawable;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.view.View;

import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.OvershootInterpolator;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.NumberPicker;
import android.widget.TextView;

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
public class MusicPlayer extends AppCompatActivity {

    NumberPicker songSelector; //number picker for song selection
    MediaPlayer musicPlayer; //musicPlayer used to play music
    Button playButton; //button to start and stop music
    TextView currMoodText; //textview at the top of the activity that displays current mood of music
    TextView musicDesc;//data displays here for each music track
    ImageView albumArt; //album art is displayed here
    ImageView animationView; //image view for music animation
    AnimationDrawable speakerAnimation; //AnimationDrawable for musicAnimation
    boolean flag = true; //boolean is used to control the play and stop button's function
    String[] songNames;//current array of songs selected depending on mood

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //link variables to ID's
        songSelector = findViewById(R.id.numberSongPicker);
        playButton = findViewById(R.id.playbutton);
        currMoodText = findViewById(R.id.moodtextview);
        currMoodText.setText(getMoodText());
        musicDesc = findViewById(R.id.musicDescription);
        albumArt = findViewById(R.id.albumartview);

        //setup the numberpicker to display the string array of song names
        //I used this video to help me understand the setMin and setMax functions: https://youtu.be/dWq5CJDBDVE
        songSelector.setMinValue(0);
        songSelector.setMaxValue(4);
        int mood = getMoodInt();
        String[] setupArr = setNumberPickerValues(mood);


        //I also used this video for setNumberPickerValues() and for understanding the onValueChangedListener below: https://youtu.be/dWq5CJDBDVE
        StringBuilder builder = new StringBuilder();
        songSelector.setDisplayedValues(setupArr);


        //Setup the music animation here, I decided to go with an 'old-style' frame by frame animation like we did for the Animation Assignment. Code is based on the code I did for that assignment.
        //I also used Zybooks for syntax and ideas for the animation
        animationView = findViewById(R.id.musicAnimation);

        animationView.setBackgroundResource(R.drawable.musicanimation);
        speakerAnimation = (AnimationDrawable) animationView.getBackground();


        //leftover code that I was experimenting with, I descided to go with a ViewAnimation instead of a ValueAnimator or ObjectAnimator.
       /** animationView = findViewById(R.id.musicAnimation);
        ValueAnimator animator2 = ValueAnimator.ofFloat(0,300);
        animator2.setDuration(1000);
        animator2.setInterpolator(new OvershootInterpolator());
        animator2.addUpdateListener(animation -> {
            float translate = (float) animator2.getAnimatedValue();
            animationView.setTranslationX(translate);
        });
        animator2.start();
        **/






       //Thread to control the description update, I was going to have the imageview update as well but it is much smoother this way.
        Thread musicDetails = new Thread(() -> {


            //The on Value Changed Listener makes it so the description and album art update to show the user the music track before they even play it.
            //this allows a better user experience since the user will have a better understanding of the song before having to play it to find out what it is about.
            songSelector.setOnValueChangedListener(new NumberPicker.OnValueChangeListener() {

                @Override
                public void onValueChange(NumberPicker numberPicker, int i, int i1) {
                    StringBuilder builder = new StringBuilder();
                    builder.append("Song Info:\n").append(getSongName(getIndex())).append("\nBy: ").append(getSongArtist(getIndex()));
                    musicDesc.setText(builder);

                    //This android article helped me with the String Builder: https://developer.android.com/reference/java/lang/StringBuilder
                }
            });
        });
        musicDetails.start();

    }


    //I was getting a lot of lag even with another thread with the onValueChangedListener so I researched and found
    //out about runOnUiThread this code is based on code from this stack overflow article: https://stackoverflow.com/questions/11140285/how-do-we-use-runonuithread-in-android
    //All we are doing is making a function that will always run on the main thread and calling that in the background thread in the onClick for the play button. This splits the load time and makes it easier for the program to run
    public void setAlbumImageView() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {

                albumArt.setImageResource(getAlbumArt(getIndex()));
                speakerAnimation.start();
            }
        });
    }

    //resets the animation from setAlbumImageView()
    public void stopAnimation() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                speakerAnimation.stop();

            }
        });
    }


    //method used to get the song index we need from the placement of the numberpicker, this is used in playing the music and the onValueChangedListener
    public int getIndex() {
        int songfromPicker;
        int songID;
        songfromPicker = songSelector.getValue();//get the string value from the number picker
        String songName;
        songName = songNames[songfromPicker]; //I used this to help me figure out how to get the index: https://stackoverflow.com/questions/25472315/how-to-get-index-value-from-numberpicker-in-android
        int index = 0;

        for(int i = 0; i < songNames.length; i++) { //find the index where the song id will be, to then use the songId below in the mediaPlayer
            if (songName == songNames[i]) {
                index = i;
            }
        }
        return index;
    }

    //set songs to thier string array names depending on the mood, this video helped me understand the different functions the numberpicker has: https://youtu.be/dWq5CJDBDVE
    public String[] setNumberPickerValues(int currMood) {


        if(currMood == 0) {
            songNames = getResources().getStringArray(R.array.calmSongs);
        }
        else if( currMood == 1) {
            songNames = getResources().getStringArray(R.array.workSongs);
        }
        else if( currMood == 2) {
            songNames = getResources().getStringArray(R.array.rockSongs);
        }
        else {
            songNames = getResources().getStringArray(R.array.calmSongs);
        }


        return songNames;
    }

    //onclick for musicplayer, controls what music is being played and the controls for the user to stop and play the music
    public void onplayMusic(View view) {

        //We use a background thread to improve performance when playiing the music, I have noticed less pops and stutters in the music after doing this
        Thread musicThread = new Thread(() -> {

            //if else statement to flip the button's controls and turn off and on the music
            if (flag == true) {
                playButton.setText("Pause");

                musicPlayer = musicPlayer.create(this, getSongID(getIndex()));//we call getIndex() here and pass the index to getSongID which gets the ID depending on the index and chosen mood
                musicPlayer.start();
                setAlbumImageView();//sets the image view to the correct album art
                flag = false;
            }
            else {

                stopAnimation();//resets (stops) the animation
                flag = true;
                musicPlayer.stop();
                playButton.setText("Play");


            }


        });
        musicThread.start();


    }

    }

