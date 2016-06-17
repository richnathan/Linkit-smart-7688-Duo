/*
Adapted from
https://github.com/dragino/modules/blob/master/examples/Bridge/XivelyClient/XivelyClient.ino

compatible with Linkit Smart 7668 Duo Development board

/*
  Xively sensor client with Strings 
 
 This sketch connects an analog sensor to Xively,
 using an Arduino Yún. 
 
 created 15 March 2010
 updated 27 May 2013
 by Tom Igoe
   
 http://arduino.cc/en/Tutorial/YunXivelyClient
 
 */

*/


#include <Process.h>


// set up net client info:
 #define APIKEY        "______YOUR____API_______KEY______"                  // replace your xively api key within quotes
 #define FEEDID        123456789                   // replace your feed ID
 #define USERAGENT     "my-project"           // user agent is the project name
const unsigned long postingInterval = 60000;  //delay between updates to xively.com
unsigned long lastRequest = 0;      // when you last made a request
String dataString = "";

void setup() {
  // start serial port:
  Bridge.begin();
  Serial.begin(9600);

  while(!Serial);    // wait for Network Serial to open
  Serial.println("Xively client");

 
}

void loop() {
  // get a timestamp so you can calculate reading and sending intervals:
  long now = millis();

  // if the sending interval has passed since your
  // last connection, then connect again and send data:
  if (now - lastRequest >= postingInterval) {
    updateData();
    sendData();
    lastRequest = now;
  }
}

void updateData() {
  // convert the readings to a String to send it:
  dataString = "field1,";
  dataString += random(10) + 20;
  // add pressure:
  dataString += "\nfield2,";
  dataString += random(5) + 100;
}

// this method makes a HTTP connection to the server:
void sendData() {
  // form the string for the API header parameter:
  String apiString = "X-ApiKey: ";
  apiString += APIKEY;
  Serial.print("API Key String ");
  Serial.println(apiString);
  
  // form the string for the URL parameter:
  String url = "https://api.xively.com/v2/feeds/";
  url += FEEDID;
  url += ".csv";
  Serial.print("URL Chain ");
  Serial.println(url);

// Send the HTTP PUT request
  Serial.println(dataString);
  Serial.println(apiString);
  Process xively;
  Serial.print("\n\nSending data... ");
  xively.begin("curl");
  xively.addParameter("-k");
  xively.addParameter("--request");
  xively.addParameter("PUT");
  xively.addParameter("--data");
  xively.addParameter(dataString);
  xively.addParameter("--header");
  xively.addParameter(apiString);
  xively.addParameter(url);
  xively.run();
  Serial.println("done!");


  // If there's incoming data from the net connection,
  // send it out the Serial:
  while (xively.available()>0) {
    char c = xively.read();
    Serial.write(c);
  }

}
