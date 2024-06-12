package com.zybooks.musicbymood;



import android.graphics.drawable.Drawable;

import androidx.appcompat.app.AppCompatActivity;

/**
 * Author: Max Hoffman
 *
 * Date: Worked on from 4/8/2022 - 4/13/2022
 * I believe that I did spend around 7 hours on this project, like what was predicted in the Creation Strategy
 *
 * IMPORTANT: I planned out the MVC Architecture and it can be found in the drawables folder, the file is called mvc_flow_chart.png
 */
//Model Java file
public class MainActivity extends AppCompatActivity {


    //I descided to lock both xml activity's since it does fit my app well and I feel like it would be a lot more code to accomplish keeping the music playing on rotation. I used Zybooks for the syntax (android:screenOrientation)


    //Each Song 'Mood' has its own 4 arrays
    //All music is from the youtube audio library
    //All album art is from pexels.com, this is not the "real" album art but it will still give a representation of the feel of the music.
    //All assets are named with no spaces and no capital letters as well as beginning with the letter of the mood they are (c for calm, w for work, and r for rock)
    //Song ID's
     private static int[] calmSongs = {R.raw.cinmemoryofjeantalonminivandals,R.raw.cnocturneasherfulero,R.raw.cvaqueroperdidotheminivandals,R.raw.cvespersontheshoretheminivandals,R.raw.cwanderingsoulasherfulero};
     private static int[] workSongs = {R.raw.wallstarspatrickpatrikios,R.raw.wbetterannodominibeats,R.raw.wdiggypatrickpatrikios,R.raw.wmamaemmitfenn,R.raw.wmonogrampatrickpatrikios};
     private static int[] rockSongs = {R.raw.ramericanidlerkvc, R.raw.rbluedayfreedomtrailstudio,R.raw.rpressurecookerjeremykorpas,R.raw.rthebeaconzachariahhickman,R.raw.rtropicalthunderrkvc};

     //Song Names
     private static String[] calmSongNames = {"In Memory of Jean Talon", "Nocturne", "Vaquero Perdido", "Vesperson The Shore","Wandering Soul"};
     private static String[] workSongNames = {"All Stars","Better","Diggy","Mama","Monogram"};
     private static String[] rockSongNames = {"American Idle","Blue Day","Pressure Cooker", "The Beacon","Tropical Thunder"};

     //Song Album Artwork ID's
     private static int[] calmArt = {R.drawable.cjeanart,R.drawable.cnocturneart,R.drawable.cperdidoart,R.drawable.cshoreart,R.drawable.csoulart};
     private static int[] workArt = {R.drawable.wstarart,R.drawable.wbetterart,R.drawable.wdiggyart,R.drawable.wmamaart,R.drawable.wmonoart};
     private static int[] rockArt = {R.drawable.ridleart,R.drawable.rblueart,R.drawable.rpressureart,R.drawable.rbeaconart,R.drawable.rthunderart};

     //Song Artist's Names
     private static String[] calmArtists = {"Mini Vandals", "Asher Fulero","The Mini Vandals", "The Mini Vandals", "Asher Fulero"};
     private static String[] workArtists = {"Patrick Patrikios","Anno Domini Beats","Patrick Patrikios","Emmit Fenn","Patrick Patrikios"};
     private static String[] rockArtists = {"RKVC","Freedom Trail Studio","Jeremy Korpas","Zachariah Hickman","RKVC"};


     //public variables to be used across functions here to store and access data from the dialog box
     private static String moodText = "Calm";
     private static int moodint = 0;


     //public accessor to get the songID data depending on the index of the numberPicker, this design is repeated for each array type (artist name, song name, album art id)
    public static int getSongID(int index) {
        int songID;
        int currMood = getMoodInt();
        if(currMood == 0) {
            songID = calmSongs[index];
        }
        else if(currMood == 1) {
            songID = workSongs[index];
        }
        else {
            songID = rockSongs[index];
        }

        return songID;
    }

    //accessor for artist name
    public static String getSongArtist(int songIndex) {
        String artistName;
        int currMood = getMoodInt();
        if(currMood == 0) {
            artistName = calmArtists[songIndex];
        }
        else if(currMood == 1) {
            artistName = workArtists[songIndex];
        }
        else {
            artistName = rockArtists[songIndex];
        }
        return artistName;
    }

    //accessor for song Name
    public static String getSongName(int songIndex) {
        String songName;
        int currMood = getMoodInt();
        if(currMood == 0) {
            songName = calmSongNames[songIndex];
        }
        else if(currMood == 1) {
            songName = workSongNames[songIndex];
        }
        else {
            songName = rockSongNames[songIndex];
        }
        return songName;
    }

    //gets the albumart ID as an int
    public static int getAlbumArt(int songIndex) {
        int albumID;
        int currMood = getMoodInt();
        if(currMood == 0) {
            albumID = calmArt[songIndex];
        }
        else if(currMood == 1) {
            albumID = workArt[songIndex];
        }
        else {
            albumID = rockArt[songIndex];
        }
        return albumID;


    }
    //stores the mood from the dialog into a variable in MainActivity to be used by the other methods to access the data depending on the mood
    public static void storeMood(int moodFromDialog) {
        moodint = moodFromDialog;

    }

    //similar to storeMood() we store a string for each mood, this is used in the music player to display the current mood in text form for the user
    public static void storeMoodText(int moodFromDialog) {
        if(moodFromDialog == 0) {
            moodText = "Calm";
        }
        if(moodFromDialog == 1) {
            moodText = "Work";
        }
        else if(moodFromDialog == 2) {
            moodText = "Rock";
        }

    }

    //accessors to get the data stored from the store functions from the dialog box
    public static int getMoodInt() {
        return moodint;
    }
    public static String getMoodText() {
        return moodText;
    }

}