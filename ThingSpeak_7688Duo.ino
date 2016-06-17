/*
  
   thingspeak example compatible with Linkit Smart 7688 Duo dev board
  ///////
  
 /*
  Yún HTTP Client

 This example for the Arduino Yún shows how create a basic
 HTTP client that connects to the internet and downloads
 content. In this case, you'll connect to the Arduino
 website and download a version of the logo as ASCII text.

 created by Tom igoe
 May 2013

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/HttpClient

 */
#include <ThingSpeak.h>
#include "YunClient.h"

YunClient client;

unsigned long myChannelNumber = 123456;
const char * myWriteAPIKey = "W___YOUR_API_KEY__F";

void setup() {
  // Bridge takes about two seconds to start up
  // it can be helpful to use the on-board LED
  // as an indicator for when it has initialized
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  Serial.begin(9600);
  
ThingSpeak.begin(client);

  while (!Serial); // wait for a serial connection
}

void loop() {
  // Initialize the client library
 

  // Make a HTTP request:

  float voltage = random(5)*(1/2.5)+1 ;

  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  ThingSpeak.writeField(myChannelNumber, 8, voltage, myWriteAPIKey);

Serial.println(voltage);

  delay(20000);
}

