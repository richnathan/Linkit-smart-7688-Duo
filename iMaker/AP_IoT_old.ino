
#include <Bridge.h>
#include <HttpClient.h>

//For pH meter
#define Offset 1.5             //deviation compensate
#define VCC    3.3
#define K      5.83            //For 5V, K = 3.5

unsigned long int avgValue;     //Store the average value of the sensor feedback

int interval = 1;            //Uint = 10 seconds, wait how many 10 seconds to sense again
//int interval = 90;

//For WiFi to ThingSpeak
HttpClient c;

void setup() {
  Bridge.begin();

  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
}

void loop() {
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

  String req_string = "http://api.thingspeak.com/update?key=6P8LFI9ALCW0EGW0";
  req_string += "&field1=";
  req_string += String(t, 2);
  req_string += "&field2=";
  req_string += String(photo, DEC);
  req_string += "&field3=";
  req_string += String(waterhigh, DEC);
  req_string += "&field4=";
  req_string += String(pH, 2);

  c.get(req_string);

  for(int i=0; i<interval; i++)
    delay(10000);
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

