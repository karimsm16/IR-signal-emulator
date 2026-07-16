// libraries

#include <Arduino.h>
#include <WiFi.h>
#include<ArduinoOTA.h> 
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
 
// the IR receiver pin 

const uint8_t IR_receiverPin = 15 ; 

// wifi credentials //
const char* networkname = "Infinix SMART 7 HD";  // enter your wifi name
const char* password = "pjTR@135"; // and here your wifi password //

// create an object to handle the IR signals 

IRrecv IRreceiver (IR_receiverPin) ;
 
// create a variable to store the decoded IR signals 

decode_results output ;

void setup (){

Serial.begin(115200) ;
delay(500);
Serial.println ("Booting");


// connect to wifi //


WiFi.begin(networkname,password);
Serial.print ("connecting ...") ;

while(WiFi.status() !=WL_CONNECTED){

Serial.print("Loading "); // the esp32 still trying to connect to the WIFI
delay(500);

} 

Serial.println("connected ");
Serial.print("IP Address :");
Serial.println(WiFi.localIP());

// OTA Hostname
ArduinoOTA.setHostname("ESP32-OTA"); // name used to identify your esp32 on the local  network 


// Start OTA service
    ArduinoOTA.begin();

    Serial.println("OTA Ready");

// start reception 
Serial.println ("start the IR reception ... "); 
  
//  enable the IR receiver 
IRreceiver.enableIRIn();
Serial.println("start listening....") ;
Serial.println(" waiting ...");



}

//  the Loop function 

void loop()
{
// This checks continuously for incoming OTA requests.
    ArduinoOTA.handle(); 

    // check if a complete IR signal has been received 
    
 if(IRreceiver.decode(&output))
 {
    Serial.println("----------------------");
    Serial.println ("signal received ");
    Serial.println("----------------------");


 
   // print protocol , adress, command and raw timings 

   Serial.println(resultToHumanReadableBasic(&output)) ;
  
   Serial.println("Raw Timing Data:");

   Serial.println(resultToTimingInfo(&output));

    // prepare the receiver for the next signal 

  IRreceiver.resume();

 }
}

