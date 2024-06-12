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

import static java.lang.Math.random;
import static java.lang.Thread.sleep;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.RippleDrawable;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.view.View;
import android.widget.Button;
import android.widget.GridLayout;
import android.widget.GridView;
import android.widget.RadioButton;
import android.widget.TextView;
import android.widget.Toast;

import java.lang.Object;
import java.util.Objects;
import java.util.Random;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.FragmentManager;

public class GameActivity extends AppCompatActivity {

    TextView timerText;
    GridLayout playGrid;


    //Colors of the buttons
    int waterColor;
    int shipColor;

    //Constants are set here for our different difficulties
    static final int EASY_DIFF = 20;
    static final int MED_DIFF = 10;
    static final int HARD_DIFF = 5;
    static int currDiff;

    //Both of these variables are involved in the point system of the game
    static int playerPts;
    static int ptThreshold;


    //each of our 9 buttons on the grid
    Button b1;
    Button b2;
    Button b3;
    Button b4;
    Button b5;
    Button b6;
    Button b7;
    Button b8;
    Button b9;
    TextView displayPts;
// Also from zybooks I found out how to lock the screen in portrait orientation, I also found out from this link it has to be in the manifest xml: https://stackoverflow.com/questions/15718869/how-to-lock-android-apps-orientation-to-portrait-in-phones-and-landscape-in-tab


    //method called by radio buttons in MainActivity.java to set our difficulty, We also set the point threshold for each difficulty which is used in a later method
    public static void setDifficulty(String difficulty) {
        if (difficulty.equals("E")) {
            currDiff = EASY_DIFF;
            ptThreshold = 50;
        }
        else if (difficulty.equals("M")) {
            currDiff = MED_DIFF;
            ptThreshold = 250;
        }
        else if(difficulty.equals("H")) {
            currDiff = HARD_DIFF;
            ptThreshold = 400;
        }

    }


    //method first called when this activity is created
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.game_activity);
        //initialize player points to 0
        playerPts = 0;

        //setting variables to our colors for the grid, this was taken from Lights Out
        waterColor = ContextCompat.getColor(this, R.color.gamebuttonon);
        shipColor = ContextCompat.getColor(this, R.color.gamebuttonoff);
        playGrid = findViewById(R.id.gridLayout);
        displayPts = findViewById(R.id.points);


        //We call this method that will populate all buttons on the grid
        onGameStart();

        //I left this commented out code to show the work I had done in trying to get the SavedInstanceState to Work, at the end of this java file is the rest of the SavedInstanceState Code I tried to implement
/**
        if (savedInstanceState == null) {
            onGameStart();
        }
        else {
            String gameState = savedInstanceState.getString(Gstate);
            setState(gameState);
        }
 **/
    }







    //I used this source for helping me understand the random number generation: https://www.youtube.com/watch?v=3v7oyTqS0Xs
    //This method creates the starting field for the game of random states for each button
    @SuppressLint("ResourceAsColor")
    public void setButtonState(Button butId) {
        Random randomNumGenerator = new Random();
        int chanceToSpawn = 0;

        //These ifs will set chanceToSpawn as a different number depending on the difficulty
        if (currDiff == EASY_DIFF) {
            chanceToSpawn = 3;
        }
        else if (currDiff == MED_DIFF) {
            chanceToSpawn = 6;
        }
        else {
            chanceToSpawn = 10;
        }


        //we create a random integer from 0 to the variable chanceToSpawn
        int randomVal = randomNumGenerator.nextInt(chanceToSpawn);//create a random number 0 to chanceToSpawn

        //then if the variable randomVal is greater than or equal to 2 we set that button to the ship color, else to water
        if (randomVal >= 2) {
            butId.setBackgroundColor(shipColor);
        }
        else  {
            butId.setBackgroundColor(waterColor);
        }


    }
    

    //used these two sources to help me create and initialize the timer: https://developer.android.com/reference/android/os/CountDownTimer#java
    //https://www.geeksforgeeks.org/countdowntimer-in-android-with-example/
    //This is our game timer and also helps us call functions until the game is finished
    public void timerStart(TextView timerT) {
        new CountDownTimer(20000,100) { //we create a one minute timer that the player must survive
            public void onTick(long secondsLeft) {
                timerT.setText(": " + secondsLeft / 100 + " ticks");//this updates the text field to display the time
                setRandomButtons();//this method will continously set the buttons to different colors
                storePoints(playerPts);//this method will display the current points


            }
            public void onFinish() {//when the timer is finished we call endGame()
                timerT.setText(": 0 ticks");
                endGame();//call this method when the game is over
            }
        }.start();
    }


    //when game is over we call this method
    public void endGame() {


        //these ifs are our win conditons, if the points earned are bigger than the point threshold given by the difficulty chosen, the player wins. If that is not true then they will not win
        if(playerPts > ptThreshold) {
            Toast.makeText(this, "You Win! Points: " + playerPts, Toast.LENGTH_LONG).show();//I used Zybooks for toast syntax

        }
        else if(playerPts < ptThreshold) {
            Toast.makeText(this, "You Lost, get more Points to win. Points Needed to Win: " + (ptThreshold - playerPts), Toast.LENGTH_LONG).show();//This toast will also show how many more points the player needed to win
        }
            finish();//we then close the activity
        
    }


    //This method is called when each button is clicked
    public void onButtonClick(View view) {
        //we set the buttons background to ship color and give them 10 points
            view.setBackgroundColor(shipColor);

            playerPts += 10;




        /**
         * An example of what I wanted to use for this but it kept erroring
         *
         * if (view.background() == waterColor) {
         *      view.setBackgroundColor(shipColor);
         * }
         *
         * I tried many variations of this if statement but all would error or not impact the buttons color at all.
         * I think this is due to how I set up my code like we previously discussed or I am not familiar enough with a correct method to use here
         *
         *
         * other examples of how I tried to get the color id:
         * ColorDrawable button1C = (ColorDrawable) b1.getBackground();
         * int color1ID = button1C.getColor();
         *
         *then I tried using an if statmenet to compare these value. This was also from a source below
         *
         *examples of sources I used that all did not work for me, whether it was because of my execution being wrong or missing something small or them not applying to my project like I wanted them to:
         *https://stackoverflow.com/questions/26399670/android-graphics-drawable-colordrawable-cannot-be-cast-to-android-support-v7-wid
         *https://stackoverflow.com/questions/8089054/get-the-background-color-of-a-button-in-android#:~:text=ColorDrawable%20buttonColor%20%3D%20(ColorDrawable)%20button,int%20colorId%20%3D%20buttonColor.
         *https://stackoverflow.com/questions/14045131/what-does-view-method-getid-return
         *https://stackoverflow.com/questions/31234897/android-sdk-java-lang-nullpointerexception-attempt-to-invoke-virtual-method
         *https://developer.android.com/reference/android/widget/Button
         *
         */
    }




    //This method will display the current points during the game
    public void storePoints(int playerPts) {
        displayPts.setText("Points: " + playerPts);
    }


    //This method is called continously while the game is going on to keep setting buttons to the waterColor
    public void setRandomButtons() {
        Random randomPlayGenerator = new Random();//new random number generator
        int buttonToChange = randomPlayGenerator.nextInt(8); //we create a random number between 0 and 8, this generator is in charge of randomizing which button should be changed
        int chanceToSpawn = 0;

        //these if statments control how easy or harder the game is based on the chosen difficulty
        if (currDiff == EASY_DIFF) {
            chanceToSpawn = 15;
        }
        else if (currDiff == MED_DIFF) {
            chanceToSpawn = 7;
        }
        else {
            chanceToSpawn = 2;
        }




        int chanceToChange = randomPlayGenerator.nextInt(chanceToSpawn); //from 0 to 2



            if (chanceToChange == 0) {
                //switch case to change each button if the buttons number is chosen by the first random generator
                switch (buttonToChange) {
                    case 0:
                        b1.setBackgroundColor(waterColor);

                        break;
                    case 1:
                        b2.setBackgroundColor(waterColor);

                        break;
                    case 2:
                        b3.setBackgroundColor(waterColor);

                        break;
                    case 3:
                        b4.setBackgroundColor(waterColor);

                        break;
                    case 4:
                        b5.setBackgroundColor(waterColor);

                        break;
                    case 5:
                        b6.setBackgroundColor(waterColor);

                        break;
                    case 6:
                        b7.setBackgroundColor(waterColor);

                        break;
                    case 7:
                        b8.setBackgroundColor(waterColor);

                        break;
                    case 8:
                        b9.setBackgroundColor(waterColor);

                        break;
                }

        }


    }





//This method is called in onCreate() and sets the initial colors of the buttons
    public void onGameStart() {
        timerText = findViewById(R.id.textView4);
        timerStart(timerText);//call and start the timer


        //link each button created to each respective button in the xml
        b1 = findViewById(R.id.button1);
        b2 = findViewById(R.id.button2);
        b3 = findViewById(R.id.button3);
        b4 = findViewById(R.id.button4);
        b5 = findViewById(R.id.button5);
        b6 = findViewById(R.id.button6);
        b7 = findViewById(R.id.button7);
        b8 = findViewById(R.id.button8);
        b9 = findViewById(R.id.button9);
        //create an array with each button
        Button[] buttonArray = {b1, b2, b3, b4, b5, b6, b7, b8, b9};



        //call setButtonState passing each button to it, this randomizes all butons
        for (int i = 0; i < 9; i++) {
            setButtonState(buttonArray[i]);
        }

    }

    //These methods were code I created to try to make the savedInstanceState element of this project
/**
 public String getState() {
 StringBuilder gameString = new StringBuilder();
 for(int i = 0; i < 8; i++) {
 char toAdd = (char) buttonArr[i];
 gameString.append(toAdd);
 }

 return gameString.toString();
 }

 public void setState(String gameInfo) {
 int indexOfInfo = 0;

 for(int i = 0; i < 8; i++) {
 buttonArr[i] = 'T';
 gameInfo.charAt(i) = 'T';
 }
 }


 @Override
 protected void onSaveInstanceState(Bundle savedInstanceState) {
 super.onSaveInstanceState(savedInstanceState);
 savedInstanceState.putString("State",getState());
 }
 **/
    //Scrapped method that was going to be used to check the state of each button for the SavedInstance state
    /**
     public void checkBut(View view) {
     if(view == b1) {
     buttonArr[0] = 0;
     }
     if(view == b2) {
     buttonArr[1] = 0;
     }
     if(view == b3) {
     buttonArr[2] = 0;
     }
     if(view == b4) {
     buttonArr[3] = 0;
     }
     if(view == b5) {
     buttonArr[4] = 0;
     }
     if(view == b6) {
     buttonArr[5] = 0;
     }
     if(view == b7) {
     buttonArr[6] = 0;
     }
     if(view == b8) {
     buttonArr[7] = 0;
     }
     if(view == b9) {
     buttonArr[8] = 0;
     }


     }
     **/

}
