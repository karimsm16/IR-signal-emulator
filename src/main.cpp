// libraries

#include <Arduino.h>
#include <WiFi.h>
#include<ArduinoOTA.h> 
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>
 
// the IR receiver pin 

const uint8_t IR_receiverPin = 15 ; 

// IR LED pin
const uint8_t IR_LED_Pin     = 4;

// Push buttons
const uint8_t Save_Button   = 18;
const uint8_t Send_Button   = 19;
const uint8_t Delete_Button = 21;
const int led = 2 ;

// wifi credentials //
const char* networkname = " your  WIFI network name";  // enter your wifi name
const char* password = "network password"; // and here your wifi password //

// create  objects to handle the IR signals 

IRrecv IRreceiver (IR_receiverPin) ; // create an object qfor receiving  
IRsend IRtransmitter(IR_LED_Pin); //  create an object for transmitting 
 
// create a variable to store the decoded IR signals 

decode_results output ;
decode_results savedSignal ;
decode_results lastSignal;


bool signalSaved = false ;

void setup (){


pinMode(Save_Button, INPUT_PULLUP);
pinMode(Send_Button, INPUT_PULLUP);
pinMode(Delete_Button, INPUT_PULLUP);
pinMode (led, OUTPUT);

Serial.begin(115200) ;
delay(500);
Serial.println ("Booting");


// connect to wifi //


WiFi.begin(networkname,password);
Serial.print ("connecting ...") ;

if(WiFi.status() !=WL_CONNECTED) 
{

Serial.print("Loading "); // the esp32 still trying to connect to the WIFI
delay(500);

} 

else {

Serial.println("connected ");
Serial.print("IP Address :");
Serial.println(WiFi.localIP());
     }  
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
 
// 

IRtransmitter.begin();


}

//  the Loop function 

void loop()
{
// This checks continuously for incoming OTA requests.
    ArduinoOTA.handle(); 

    // check if a complete IR signal has been received 
    
 if(IRreceiver.decode(&output))
 {
    if (output.decode_type == UNKNOWN or output.rawlen < 12) 
    {
       IRreceiver.resume();
         return;

    } 
    lastSignal = output  ;
    Serial.println("----------------------");
    Serial.println ("signal received ");
    Serial.println("----------------------");

    digitalWrite (led,HIGH);
    delay(1000);
    digitalWrite(led,LOW) ;

 
   // print protocol , adress, command and raw timings 

   Serial.println(resultToHumanReadableBasic(&output)) ;
  
   Serial.println("Raw Timing Data:");

   Serial.println(resultToTimingInfo(&output));

    // prepare the receiver for the next signal 

  IRreceiver.resume();

 } 

  if (digitalRead(Save_Button) == LOW)
 {
    savedSignal = lastSignal;
    signalSaved = true;

    Serial.println("Signal saved.");
    Serial.print("Saved output protocol: ");
    Serial.println(typeToString(output.decode_type));
    digitalWrite (led,HIGH);
    delay(500);
    digitalWrite (led,LOW); 
    delay(500);

    delay(300);
 }

   if (digitalRead(Delete_Button) == LOW)
  {
    signalSaved = false;

    Serial.println("Signal deleted.");

    delay(300);
  }
  if (digitalRead(Send_Button) == LOW)
{
    if (!signalSaved)
    {
        Serial.println("No signal saved.");
        delay(300);
         digitalWrite (led,HIGH);
    delay(1000);
    digitalWrite (led,LOW); 
    delay(500);
        return;
    }

       Serial.print("Saved protocol: ");
       Serial.println(typeToString(savedSignal.decode_type));

    switch (savedSignal.decode_type)
    {
        case NEC:
            IRtransmitter.sendNEC(savedSignal.value, savedSignal.bits);
            break;

        case SONY:
            IRtransmitter.sendSony(savedSignal.value, savedSignal.bits);
            break;

        case SAMSUNG:
            IRtransmitter.sendSAMSUNG(savedSignal.value, savedSignal.bits);
            break;

        case LG:
            IRtransmitter.sendLG(savedSignal.value, savedSignal.bits);
            break;

        case PANASONIC:
            IRtransmitter.sendPanasonic(0, savedSignal.value);
            break;

        default:
            Serial.println("This protocol is not supported yet.");
            delay(300);
            return;
    }

    Serial.println("Signal transmitted.");
    delay(300);
}
}

