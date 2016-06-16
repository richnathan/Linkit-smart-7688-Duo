// Code written to count an interrupt on Linkit Smart 7688 Duo
//I  could not call the external interrupt  with the standard attachInterrupt(pin, ISR, mode) .  Perhaps I have a setting off or the 7688 Duo clockspeed isnt 
//compatiable with the arduino library... I dont know.  So here is my work around...  Not elegant and slow... but effective.
// The flowmeter interrupt is counted by a python script (isr.py) running on the MPU, called from a cron job.  The ISR (interrupt service routine)
  // counts the clicks for X seconds and then writes the number to a flatfile.  A second python script
  // then opens the flatfile and send the value through the bridge.  This routine then reads the value from the bridge, converts
  // back to an integer for the arduino to use.
  // open source, use as you wish
  // Author Nathan Richardson
  // Date   5/25/16
/*

Place isr.py and isr4.py scripts in ./Iot/ directory .  Create /IoT/data/ directory chmod 777.
from OpenWRT dashboard > system > scheduled tasks tab, Call this script to run every 30 seconds with a cron job.  
* * * * * python /IoT/isr.py; sleep 30; python /IoT/isr.py;

 */



#include <Process.h>
#include <Bridge.h>

//Flow Sensor1
char flow1[5];
int flow1_int;

void setup() {
  
  Bridge.begin();
   
  Serial.begin(9600);
 delay(3000);
Serial.println("Setup Complete.......Begin Loop....");

}

void loop() {
  
  flowmeter();

 Serial.print(millis()); 
 Serial.print(",  Flow count: "); Serial.println(flow1_int);

 delay(500);
}

void flowmeter() {
  
 
  Process p;
  p.runShellCommand("python /IoT/isr4.py"); //Run the Python Code to generate values
    
  Bridge.get("90",flow1,5);  //get value from Bridge and store
  flow1_int = atoi(flow1);//convert ASCII to integer
 
//  Serial.println(flow1_int);  //print result to serial monitor to check

}
