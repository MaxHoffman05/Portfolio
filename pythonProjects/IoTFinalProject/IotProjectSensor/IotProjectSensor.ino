/*
Iot Final Project

Author - Max Hoffman

Finished on : 12/5/2023

*/
#include <OneWire.h>//temp libraries
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>//libraries to connect to the Wifi
#include <ESP8266HTTPClient.h>



#define ONE_WIRE_BUS 4//pin for temp sensor
#define tdsPin A0//pin for tds sensor
//wifi info
const char* ssid = "yourNetworkName";
const char* password = "yourNetworkPassword";
double tdsVal;

// Based off some example code for the temperature sensor
// it took me around a week to figure out how to read in 
// the data from the temp sensor and this was the only 2 libraries that worked to read in the data
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
 
void setup(void) {
  Serial.begin(9600);

 
  //code from in class to connect to wifi
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP Address is: "); Serial.println(WiFi.localIP());
  
  sensors.begin();//for temp sensor
}
 
void loop(void){ 
  WiFiClient client;
  HTTPClient clientHTTP;

  //same code from in class to make sure we are connected
  if(WiFi.status() == WL_CONNECTED) {
  
    //reads temperature and tds from both sensors
    //this is where I found the example code for the temp sensor that worked: https://randomnerdtutorials.com/guide-for-ds18b20-temperature-sensor-with-arduino/
    sensors.requestTemperatures();
    String waterTemp = String(sensors.getTempFByIndex(0));
    tdsVal = analogRead(tdsPin);
    tdsVal = tdsVal + 79;//calibrates the tds sensor 

    //this took me 5 hours to do, so it goes without saying I imagined this to take 5 minutes and it took 100x that.
    //we are sending the TDS value and temp value from each of our sensors to be dweeted out, these will be read by
    //the other piece of code from the other arduino with a screen to be displayed there and also do other cool things 
    String url = "http://dweet.io/dweet/for/projIOTHof23";
    clientHTTP.begin(client, url.c_str());
    clientHTTP.addHeader("Content-Type", "application/x-www-form-urlencoded");
    clientHTTP.POST("WaterTemp=" + waterTemp + "&TDS=" + String(tdsVal));//I forgot where but I saw if you add an & that lets you post multiple values
    clientHTTP.end();
  
    Serial.println("TDS VAL : " + String(tdsVal));
    Serial.println("Temp : " + waterTemp);
    delay(1000);
  }
  delay(3000);
  
}