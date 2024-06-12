package com.zybooks.musicbymood;

import static com.zybooks.musicbymood.MainActivity.getMoodText;
import static com.zybooks.musicbymood.MainActivity.storeMood;
import static com.zybooks.musicbymood.MainActivity.storeMoodText;

import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;

import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.DialogFragment;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

/**
 * Author: Max Hoffman
 *
 * Date: Worked on from 4/8/2022 - 4/13/2022
 * I believe that I did spend around 7 hours on this project, like what was predicted in the Creation Strategy
 *
 * IMPORTANT: I planned out the MVC Architecture and it can be found in the drawables folder, the file is called mvc_flow_chart.png
 */
//I used both of these android studio links to base my dialog fragment code on
//https://developer.android.com/guide/topics/ui/dialogs#AddingButtons
//https://developer.android.com/guide/topics/ui/dialogs#ShowingADialog
public class MoodDialogFragment extends DialogFragment {

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder dialogBuilder = new AlertDialog.Builder((getActivity()));
        dialogBuilder.setTitle("What music fits your mood today?"); //sets the title and list of choices in the dialog box
        dialogBuilder.setItems(R.array.moodDialogList, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int which) {
                //these are our store functions that store the data from the dialog box
                storeMood(which);
                storeMoodText(which);

            }
        });
        return dialogBuilder.create();
    }


}