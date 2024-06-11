#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


// Connect your yellow pin to Pin12 on Arduino
#define ONE_WIRE_BUS 4
#define tdsPin A0



const char* ssid = "yourNetworkName";
const char* password = "yourPassword";

double tdsVal;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);
 
void setup(void) {
  Serial.begin(9600);

  // initialize the Serial Monitor at a baud rate of 9600.
  
  WiFi.begin(ssid, password);
  //WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP Address is: "); Serial.println(WiFi.localIP());
  
  
 
  sensors.begin();
}

 
void loop(void){ 
  WiFiClient client;
  HTTPClient clientHTTP;


  if(WiFi.status() == WL_CONNECTED) {
  
  sensors.requestTemperatures();
  String newStr = String(sensors.getTempFByIndex(0));

  Serial.println(newStr);
  //String url = "https://dweet.io/dweet/for/projIOTTest2002?temp=" + newStr;
  String url = "https://dweet.io/dweet/for/projIOTTest2222?temp=100";
  
  clientHTTP.begin(client, url.c_str());
 
  tdsVal = analogRead(tdsPin);

  Serial.println(url);
  Serial.println("TDS VAL : " + String(tdsVal));
  
  delay(1000);
  }
  delay(1000);
  
}