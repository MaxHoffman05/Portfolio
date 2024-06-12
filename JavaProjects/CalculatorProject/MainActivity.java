package com.zybooks.calculatorproject;

import static java.lang.Double.NaN;
import static java.lang.Double.isInfinite;
import static java.lang.Double.isNaN;
import static java.lang.Math.abs;
import static java.lang.Math.sqrt;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.SharedPreferences;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.util.ArrayList;
import java.lang.Math.*;

/**
 * Date: Started working on 4/25/2022 to 5/
 *
 * Author: Max Hoffman
 * 
 *
 * SOURCE LIST:
 * I used this source for copying and pasting in the actual mathmatatical symbols for the xml: https://unicode-table.com/en/
 *
 * I was having trouble getting my equal button to go across multiple collums this source solved it because I needed to change the layout gravity: https://stackoverflow.com/questions/24971153/grid-layout-column-span-not-works
 *
 * I used the math library and various functions from Android Studio's Website: https://developer.android.com/reference/java/lang/Math
 *
 * All sounds are from pixabay.com/sound-effects
 *
 * I also want to explain that I thought I found 2 errors one with adding and subtracting a negative number but I cannot replicate it.
 * The other I also cannot replicate. The other error I found related to the store and recall buttons not adding to the stored value.
 * But after testing I cannot find this error as well. So either it is a very specific error that I am having trouble to replicate or
 * there is no error and it was down to user error on my part. I just wanted to make you aware of this.
 *
 *
 * I also did fix all app crashes like not allowing the user to add a decimal point to infinity or crash the app in other ways. I have labeled where I fixed these errors throughout the code comments.
 *
 */
public class MainActivity extends AppCompatActivity {
    TextView displayArea; //Text box which is used for the display
    String numText; //String used to get the number from the text box
    String displayText;
    double lastAnswer; //integer varaible used if the user after doing an operation on 2 numbers and then pressed another operation key to add, subtract etc then the number is kept
    ArrayList<String> numberArray = new ArrayList<String>(); //Array List used to store the numbers and operation symbols to be decoded in the equals onclick
    // I used this source because I needed a way to append items to a list and you cant do that with a reqular array: https://stackoverflow.com/questions/15039519/how-to-dynamically-add-elements-to-string-array
    MediaPlayer operationMP;//three music players for each sound type (error, operation, or number button pressed)
    MediaPlayer numMP;
    MediaPlayer errorMP;
    boolean justCompleted = false;//boolean used with checking if the user wants to keep the last number to preform an operation on it

    /**
     Code originally used for finding out if a number is infinity or NaN.
     I then discovered that the Double class contains isNaN() and isInfinite(),
     from this source: https://developer.android.com/reference/java/lang/Double
    final double INFINITY = Math.pow(2, Double.MAX_EXPONENT);//constant used for infinity found from this source: https://developer.android.com/reference/java/lang/Float#MAX_EXPONENT
    final double NAN = NaN;
     Log.e("checkNAN", "NAN: " + NAN);
    **/

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        displayArea = findViewById(R.id.calcDisplay);

        //music player for each type of button and for our error sound
        operationMP = MediaPlayer.create(this, R.raw.click);
        numMP = MediaPlayer.create(this, R.raw.pop);
        errorMP = MediaPlayer.create(this, R.raw.error);
    }


    //onclick used for all number pad buttons 0-9. We differentiate between each by using xml tags on each button
    public void numBClick(View view) {
        displayText = String.valueOf(displayArea.getText());

        if(displayText.equals("0")) {//bulletproofing
            displayArea.setText(null);
        }
        numMP.start();//play sound
        justCompleted = false;
        String buttonNum = (String) view.getTag();//grab button tag
        displayArea.append(buttonNum);//add number from button to the text box
    }


    //method used for all the operation buttons as suggested by you, this was changed from my intermediate project
    public void operationClick(View view) {
        operationMP.start();
        String operationSymbol = (String) view.getTag();

        if(justCompleted == true) {//if the user preforms an operation from a previous operation we keep that previous number and add it to the array
            displayArea.setText(null);
            numberArray.add(getLastAnswer());
            Log.v("secadd","ran justCompleted +");
        }
        else {//else we add the new number to the list
            numText = String.valueOf(displayArea.getText());
            numberArray.add(numText);
        }

        displayArea.setText("0");
        switch(operationSymbol) {//adding various operation symbols to array depending on the button pressed
            case "+":
                numberArray.add("+");
                break;
            case "-":
                numberArray.add("-");
                break;
            case "×":
                numberArray.add("*");
                break;
            case "÷":
                numberArray.add("/");
                break;
        }
    }

    //method used for the equal button
    //We calculate the product from the array constructed by the other methods that contains what the user wants to preform
    //This is also where we setup the calculator to use the last number if applicable. (use the product in another calculation)
    //All calculations are preformed here
    public void equalClick(View view) {
        operationMP.start();
        double calculatedNum = 0.0;
        String finalAns;
        double secNum = 0.0;
        String currSym = null;

        numberArray.add(String.valueOf(displayArea.getText()));

        Log.v("arrayContents", "Array:" + numberArray);

        //for loop iterates through the array
        for (int i = 0; i < numberArray.size(); i++) {
            Log.v("index", "index: " + i);

            if ((i % 2) == 0) {//check to see if we are in an even index (where an operation is)

                if (i == 0) {//if we are at the first index we set calculatedNum to the first number
                    calculatedNum = Double.parseDouble(numberArray.get(i));
                    Log.v("num1", "num1:" + calculatedNum);
                }


            } else if ((i % 2) != 0) {//check to see if we are at an odd index (where a number is)
                currSym = numberArray.get(i);//grab the symbol of the operation
                Log.v("checksec1", "ran odd");
                    secNum = Double.parseDouble(numberArray.get(i+1));//grab the next number to be operated on
                    Log.v("checksec", "num2:" + secNum);
                    //if statements for each operation

                    if(currSym.equals("+")) {//Addition
                        calculatedNum += secNum;
                    }
                    else if(currSym.equals("-")) {//Subtraction
                        calculatedNum -= secNum;
                    }
                    else if(currSym.equals("*")) {//Multiplication
                        calculatedNum *= secNum;
                    }
                    else if(currSym.equals("/")) {//Division
                        calculatedNum /= secNum;
                    }
                    else if(currSym.equals("pyth")) {//PYTH

                        calculatedNum = (calculatedNum * calculatedNum) + (secNum * secNum);
                        calculatedNum = Math.sqrt(calculatedNum);
                    }
                    else if(currSym.equals("xtoy")) {//x^y
                        calculatedNum = Math.pow(calculatedNum, secNum);
                    }
            }
        }
        //display the final calculation
        finalAns = String.valueOf(calculatedNum);
        displayArea.setText(finalAns);

        //reset the array
        numberArray.clear();
        Log.v("checkArray", "arraycontents:" + numberArray);
        //store the result in case the user wants it
        storeAnswer(calculatedNum);
        Log.v("checkAns", "stored:" + calculatedNum);
        //set our boolean to true which allows the result to be used in a calculation
        justCompleted = true;
        checkForError(calculatedNum);
    }//end equalClick() method

    //onClick for clear button, clears array and the displayArea
    public void clearClick(View view) {
        operationMP.start();//play sound
        displayArea.setText("0");//set display area to 0 and clear array
        numberArray.clear();
        justCompleted = false;

    }

    //public accessor and storeing of the result to be used again if the user wants it to be used. These functions are different from the store and recall button's function
    public void storeAnswer(double answer) {
        lastAnswer = answer;
    }//end storeAnswer()

    public String getLastAnswer() {
        Log.v("checkNum","lastAns:" + String.valueOf(lastAnswer));
        return String.valueOf(lastAnswer);

    }//end getLastAnswer()

    //Method used for all unary buttons or buttons that only use one number
    public void unaryClick(View view) {
        operationMP.start();
        String operationTag = (String) view.getTag();
        double toBeSqrd;
        double toSqrt;
        double tobeFlipped;

        double numberfromDisplay = Double.parseDouble(String.valueOf(displayArea.getText()));//current number in display
        String currentText = String.valueOf(displayArea.getText());//current number in text form from display
        String result;

        switch(operationTag) {
            case "."://decimal button
                //get the current number and add a decimal point at the end of the number, then set the display area to that new number with the decimal point

                if(currentText.contains(".") || isNaN(numberfromDisplay) || isInfinite(numberfromDisplay)) {//code to prevent multiple decimalpoints which prevents crashing
                    result = currentText;//this if also prevents adding a decimal to an infinite number or NaN which prevents more crashing
                }
                else {
                    result = currentText + ".";
                }
                break;
            case "^2"://square button
                toBeSqrd = numberfromDisplay;
                toBeSqrd = toBeSqrd * toBeSqrd;
                result = String.valueOf(toBeSqrd);
                break;
            case "rt"://square root button
                toSqrt = numberfromDisplay;
                toSqrt = sqrt(toSqrt);
                result = String.valueOf(toSqrt);
                break;
            case "switch"://plus and minus button, or flip the sign button
                tobeFlipped = numberfromDisplay;
                if(tobeFlipped == 0) {//we dont set 0 to be negative to avoid erroring
                    result = "0";
                }
                else if(abs(tobeFlipped) == tobeFlipped) {//positive number flip to negative
                    result = "-" + currentText;
                }
                else {//negative number flip to positive
                    result = String.valueOf(abs(tobeFlipped));
                }
                break;
            case "%"://percent button, divides number by 100 ex 100% = 1

                result = String.valueOf(numberfromDisplay / 100);
                break;
            default:
                throw new IllegalStateException("Unexpected value: " + operationTag);
        }
        //at the end we set the text to the result and check if the number was infinity or NaN with the checkForError method
        displayArea.setText(result);
        checkForError(Double.parseDouble(result));
    }//end unaryClick()

    //onClick for pythagorean triple button.
    public void pythagoreanClick(View view) {
        String currentText = String.valueOf(displayArea.getText());

        operationMP.start();//play sound
        //add the current number then add a string pyth to the array to be used in the equal method
        numberArray.add(currentText);
        numberArray.add("pyth");//used later in equal method
        displayArea.setText("0");
    }//end pythagoreanCLick()

    //onClick for the x^y button
    public void xToYClick(View view) {
        String currentText = String.valueOf(displayArea.getText());

        operationMP.start();//play sound
        //add the current number in the displayArea and then add xtoy to the number array to be used in the equal method
        numberArray.add(currentText);
        numberArray.add("xtoy");//used later in equal method
        displayArea.setText("0");
    }//end xToYClick()


    //Method to check if a result is infinity or NaN, this runs in a background thread to reduce lag on the main UI thread
    public void checkForError(double answer) {
        //run on background thread
        Thread errorCheck = new Thread (() -> {
        //play error noise if the answer matches our infinity constant
        if (isInfinite(answer) || isNaN(answer)) {
            errorMP.start();
        }
        });
        errorCheck.start();
    }//end CheckForError()


    //store and recall button's onClick to save to SharedPrefs. Similar to the code from the BMI calculator Assignment
    public void storeRclClick(View view) {
        String type = (String) view.getTag();//gets the tag from the button we use this tag to determine which button is being pressed
        String storedNumber;

        //make sharedPref and set editor, also we set testIfEmpty to whatever the data is stored (which will always be empty
        SharedPreferences numberSharedPrefs = getSharedPreferences("user_num_data", Context.MODE_PRIVATE);
        String testIfEmpty = numberSharedPrefs.getString("userNum", null);
        SharedPreferences.Editor numberEditor = numberSharedPrefs.edit();

        if(type.equals("recall") && testIfEmpty == null) {//test if there is no stored number if so we store 0 if the user presses recall button before pressing the store button
            numberEditor.putString("userNum", "0");
            numberEditor.apply();
            displayArea.setText("0");

        }
        else if(type.equals("store")) {//if the user presses the store button we store the current number in the displayArea to SharedPrefs
            numberEditor.putString("userNum", String.valueOf(displayArea.getText()));
            numberEditor.apply();
        }
        else if(type.equals("recall")) {//if the user presses recall we put the stored number into displayArea
            storedNumber = numberSharedPrefs.getString("userNum", null);
            displayArea.setText(storedNumber);

        }

    }

}//end MainActivity.java