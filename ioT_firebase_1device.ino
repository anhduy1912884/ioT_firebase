#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>       
#include <Firebase_ESP_Client.h>
#include <stdlib.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define API_KEY "AIzaSyB45NVruZrpGHPmAb6-jcZ7S10SwKYybDY"           //firebase information
#define DATABASE_URL "the-proliec-default-rtdb.firebaseio.com/" 

#define Ledpin 14       // define output pin

WiFiServer server(80);

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int S1;
bool signupOK = false;

void setup() {  
  pinMode( Ledpin, OUTPUT );
  Serial.begin(115200);
  //connect to wifi
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("Connected.");  
  server.begin();
 
//connect to firebase
config.api_key = API_KEY;
config.database_url = DATABASE_URL;

if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true; }
else { Serial.printf("%s\n", config.signer.signupError.message.c_str());}

config.token_status_callback = tokenStatusCallback; 

Firebase.begin(&config, &auth);
Firebase.reconnectWiFi(true);
}

void loop(){
if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 100 || sendDataPrevMillis == 0))   {
   sendDataPrevMillis = millis();  

if (Firebase.RTDB.getString(&fbdo, "S1"))      //read from firebase
    {
      if (fbdo.dataType() == "string") 
      {
        S1 = fbdo.stringData().toInt();    //convert string data to int data
       Serial.println(S1);        
      }
    }     
    
    digitalWrite( Ledpin , S1 );  //set output 
}
}
