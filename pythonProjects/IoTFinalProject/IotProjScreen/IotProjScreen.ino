#define BLYNK_TEMPLATE_ID "TMPL2VoaZyRQH"
#define BLYNK_TEMPLATE_NAME "Send Temp"
#define BLYNK_AUTH_TOKEN "kfacFP3iRYNk0lka4Dydw7322j-Ttsmt"//for blynk
/*
Iot Final Project
Author - Max Hoffman
Finished on : 12/5/2023
*/
//code for esp8266 with screen, reads in data from dweet and sends blynk alerts if necessary
//these links helped me understand how to use Blynk to send notificaitons to my phone
//I also followed the youtube video on how to make code interact with blynk
//Lots of incorrect info on the internet now that there is blynk 2.0 opposed to 1.0, that took a little bit to figure out
//https://github.com/Tech-Trends-Shameer/Esp-8266-Projects/blob/main/Theft-Alert-Notification-Using-Blynk-IOT/theft-alert-notification-using-blynk-iot-and-esp8266.ino
//https://www.youtube.com/watch?v=129WpAtF99s
//https://community.blynk.cc/t/ota-and-blynk-begin/45562


//I also used code from the amazon site where I bought the esp8266 with screen for the u8g2 library which is for the screen
//I had originally planned to display a graph on the screen but I could not figure out the library and wanted to get something working for the project
//In the future Id like to display the temp as a graph
//https://www.amazon.com/Development-Wireless-Micropython-Programming-Soldered/dp/B0BVM3H46B/ref=sr_1_8?crid=260P7E6FXOJFE&keywords=arduino+with+screen+nodemcu&qid=1701830139&sprefix=arduino+with+screen+nodemcu%2Caps%2C219&sr=8-8
#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>//screen library
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <BlynkSimpleEsp8266.h>
const char* auth = "kfacFP3iRYNk0lka4Dydw7322j-Ttsmt";

//wifi name and password
const char* ssid = "yourNetworkName";
const char* password = "yourNetworkPassword";


//based off of the code I got the arduino from on amazon for the screen
U8G2_SSD1306_128X64_NONAME_F_SW_I2C
u8g2(U8G2_R0, 14, 12, U8X8_PIN_NONE);

BlynkTimer timer;

void readData() {  //our main code is put in readData() since we are using Blynk for notifications which requires loop() to be clear
  //code that prints out the dweeted vals to the screen, also ine
  WiFiClient client;
  HTTPClient clientHTTP;


  String getDweet = "http://dweet.io/get/latest/dweet/for/projIOTHof23";
  clientHTTP.begin(client, getDweet.c_str());
  clientHTTP.addHeader("Content-Type", "application/x-www-form-urlencoded");
  clientHTTP.GET();
  String testVal = clientHTTP.getString();
  //Serial.println(testVal);
  String tds1;

  JSONVar jsonVer = JSON.parse(testVal);  //get our json string and then assign variables for each read in value in the json string

  //ok this is a really bad way of doing it but I could not figure out how to grab the variables from the json string even with the notes and example that we did in class
  //so for now this is how I grab the TDS variable and temp variable, I will change this later to make the code way more efficient
  for (int i = 0; i < testVal.length(); i++) {
    if (testVal[i] == 'T') {
      if (testVal[i + 1] == 'D') {
        if (testVal[i + 2] == 'S') {

          //get last bit of string
          for (int j = i + 5; j < testVal.length(); j++) {
            tds1.concat(testVal[j]);
          }
          break;
        }
      }
    }
  }
  String tds;
  for (int k = 0; k < tds1.length(); k++) {
    if (tds1[k] != '}') {
      tds.concat(tds1[k]);
    } else {
      break;
    }
  }
  clientHTTP.end();
  String tempJSON;
  for (int i = 0; i < testVal.length(); i++) {
    if (testVal[i] == 'W') {
      if (testVal[i + 1] == 'a') {
        if (testVal[i + 2] == 't') {
          if (testVal[i + 3] == 'e') {
            if (testVal[i + 4] == 'r') {
              if (testVal[i + 5] == 'T') {
                if (testVal[i + 6] == 'e') {
                  if (testVal[i + 7] == 'm') {
                    if (testVal[i + 8] == 'p') {
                      for (int j = i + 9; j < testVal.length(); j++) {
                        tempJSON.concat(testVal[j]);
                      }
                      break;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  //Serial.println(tds);
  //Serial.println(tempJSON);
  String temp;
  //get the las bit of the temp json string
  for (int i = 2; i < 7; i++) {
    if (tempJSON[i] != ',') {
      temp.concat(tempJSON[i]);
    } else {
      break;
    }
  }

  //Serial.println(temp);


  String tdsString = "TDS : " + tds;
  String tempString = "Temp : " + temp;


  //draws the info on the screen
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_7x14B_tr);
  u8g2.drawStr(1, 10, "Fish Tank Stats");
  u8g2.drawStr(1, 30, tdsString.c_str());

  u8g2.drawStr(1, 45, tempString.c_str());
  u8g2.sendBuffer();


  //now we can do stuff with the info
  double tdsVal = tds.toDouble();
  double tempVal = temp.toDouble();
  Serial.println(tdsVal);
  Serial.println(tempVal);

  double TDSDisplay = 450.00;
  //Blynk Notifications
  if (tdsVal > TDSDisplay) {//for the aquarium I using this on the average is around 300, you just dont want this to build up 
  //and really it will only build up if you are not doing water changes and you are just topping off the aquarium as water evaporates.
    //tds is too high, do a water change!
    Blynk.logEvent("tdshigh", String(TDSDisplay) + " The TDS in the tank is: " + tds);

  }
  if (tempVal > 80.00) {
    //send appropriate alert if temp is too high
    Blynk.logEvent("temphigh", "Temperature : " + temp);
  }
  if (tempVal < 60.00) {
    //temp is too low!
    Blynk.logEvent("templow", "Temperature : " + temp);
  }

  delay(2000);
}
void setup() {
  timer.setInterval(1000L, readData);//this means run the timer method every second, learned from the youtube video
  Serial.begin(9600);
  //similar code to the other file except now we are taking in the info instead of sending it out
  WiFi.begin(ssid, password);
  //WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());

  Blynk.begin(auth, ssid, password);  //set up blynk

  u8g2.begin();
}


void loop() {
  Blynk.run();
  timer.run();  //found out from this forum post to make sure loop() is clean https://docs.blynk.io/en/legacy-platform/legacy-articles/keep-your-void-loop-clean
}
