
#include <Bridge.h>
//#include "YunClient.h"
#include <HttpClient.h>
//YunClient client;
HttpClient c;

//One Wire Temp  source https://milesburton.com/Dallas_Temperature_Control_Library
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 3  // Be sure to attatch 4.7k ohm resistor from 5V to data

//For pH meter
#define Offset 1.5             //deviation compensate
#define VCC    3.3
#define K      5.83            //For 5V, K = 3.5


unsigned long int avgValue;     //Store the average value of the sensor feedback

//int interval = 1;            //Uint = 10 seconds, wait how many 10 seconds to sense again
//int interval = 90;



const long logInterval = 20000;
const int systemdelay = 3000;
long lastLogTime = 0;

//One Wire Temp
float tempC;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//ThingSpeak
//unsigned long myChannelNumber = 124496;
const char * myWriteAPIKey = "6P8LFI9ALCW0EGW0";

 
//Xively
//Xively api key for upload and download
// set up net client info:
 #define APIKEY        "Jr5YtuExT7JqovQPJpNhcFDivuISUjBuToMGcHwtBs3DJ4Qx"                  // replace your pachube api key here
 #define FEEDID        128680996                   // replace your feed ID
 //#define USERAGENT     "my-project"           // user agent is the project name
//const unsigned long postingInterval = 60000;  //delay between updates to xively.com
//unsigned long lastRequest = 0;      // when you last made a request
String dataString = "";

//  Flow Meter
char flow1[5];
float flow1_int;


  

void setup() {
  Serial.begin(9600);
  Bridge.begin();
  sensors.begin();
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
}

void loop() {
  flowmeter();
  char buff[40]; 
  
  int photo = analogRead(0);

  digitalWrite(14, HIGH);
  delay(4000);
  int waterhigh = analogRead(1);
  digitalWrite(14, LOW);
  delay(1000);
  
  float pH = convertTopH();
  double t = log(((10240000.0/analogRead(3)) - 10000)); 
  t = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * t * t ))* t ); 
  t -= 273.15;

if (millis() - lastLogTime > logInterval)
  {
    digitalWrite(13, LOW);
    lastLogTime = millis();
     Serial.println("  Logging to thingspeak, Please wait..... ");

 String url = "http://api.thingspeak.com/update?key=6P8LFI9ALCW0EGW0";
  url += "&field1=";
  url += String(t, 2);
  url += "&field2=";
  url += String(photo, DEC);
  url += "&field3=";
  url += String(waterhigh, DEC);
  url += "&field4=";
  url += String(pH, 2);
  url += "&field5=";
  url += String(tempC, 1);
  url += "&field7=";
  url += String(flow1_int, 2);
 
  c.get(url);

/* 
 const char* hostts = "api.thingspeak.com"; 

if (!client.connect(hostts, 80)) {
    return;
  }
  //Construct the post...
  String url = "/update?key=";
  url += "&field1=";
  url += String(t, 2);
  url += "&field2=";
  url += String(photo, DEC);
  url += "&field3=";
  url += String(waterhigh, DEC);
  url += "&field4=";
  url += String(pH, 2);
  url += "&field5=";
  url += String(tempC, 1);
  url += "&field7=";
  url += String(flow1_int, 2);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + hostts + "\r\n" + 
               "Connection: close\r\n\r\n");
               
               client.stop();
*/

// LED with flash when logging to thingspeak
   for (int j=1; j<=25; j=j+1) {     // Start our for loop
    digitalWrite(13,HIGH); //Turn red LED on
    delay(20);             //Leave on for redOnTime
    digitalWrite(13,LOW);  //Turn red LED off
    delay(100);  
     }            

dataString = "field1,";
  dataString += String(t, 2);
  dataString += "\nfield2,";
  dataString += String(photo, DEC);
  dataString += "\nfield3,";
  dataString += String(waterhigh, DEC);
  dataString += "\nfield4,";
  dataString += String(pH, 2);
  dataString += "\nfield5,";
  dataString += String(tempC, 1);
  dataString += "\nfield7,";
  dataString += String(flow1_int, 2);


     Serial.println("  Logged to Thingspeak.....  ");
// updateData();
  sendData();

  
  
  }
 
    
tempC = OneWireTemp();
 Serial.print(millis());
 Serial.print(",\t t: "); Serial.print(t,2); Serial.print(" ml/min");
 Serial.print(",\t Photo: "); Serial.print(photo,DEC); 
 Serial.print(",\t Water Level: "); Serial.print(waterhigh, DEC); 
 Serial.print(",\t pH: "); Serial.print(pH, 2); 
 Serial.print(",\t Water Temp C ");  Serial.print(tempC, 1);
 Serial.print(",\t  Flow count: "); Serial.print(flow1_int,2); Serial.println(" l/min");
 
 Serial.println(" ");
  delay(systemdelay);
}

float convertTopH()
{
  int buf[10];                //buffer for read analog
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  {
    buf[i]=analogRead(2);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        int temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*VCC/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue+Offset;                      //convert the millivolt into pH value
  return(phValue);
}


float OneWireTemp()
{
  
   sensors.requestTemperatures();
     float tempC = sensors.getTempCByIndex(0);  //sensors.getTempC();
  if (tempC == -127.00) {
    Serial.print(" OneWire Temp Sensor Error");
    
     } else {

return tempC;
     }
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
  Serial.print("URL Chaine ");
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
//Opens connection to MPU via YunBridge to get flowmeter counts
void flowmeter() {
  Process p;
  p.runShellCommand("python /IoT/isr4.py"); //Run the Python Code to generate values
  Bridge.get("90",flow1,5);  //get value from Bridge and store
  int fl2 = atoi(flow1); //convert ASCII to integer
  flow1_int = (fl2 * 4 / 400.0);   //apply flowrate calculation ml/min, isr.py logs for 15 seconds 60/15 = 4,  for 1/8" Q= 6.5 clcks/ml

}
