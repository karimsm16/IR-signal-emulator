#include <Arduino.h>
#include <WiFi.h>
#include<ArduinoOTA.h> 


// wifi credentials //
const char* networkname = "Infinix SMART 7 HD";  // enter your wifi name
const char* password = "pjTR@135"; // and here your wifi password //

void setup (){

Serial.begin(115200) ;
Serial.println ("Booting");

// connect to wifi //

WiFi.begin(networkname,password);
Serial.print ("connecting ...") ;

while(WiFi.status() !=WL_CONNECTED){

Serial.print("CONNECTION FAILED !!");
delay(500);

} 

Serial.println("connected ");
Serial.print("IP Address :");
Serial.println(WiFi.localIP());

// OTA Hostname
ArduinoOTA.setHostname("ESP32-OTA"); // this is the adress of your esp32 in the network 


// Start OTA service
    ArduinoOTA.begin();

    Serial.println("OTA Ready");
}

//  the Loop function 

void loop()
{
// This checks continuously for incoming OTA requests.
    ArduinoOTA.handle();
}

