#include "EmonLib.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include "ThingSpeak.h" //download 

const char* ssid = "Galaxy A13324C";
const char* password = "12345678";

unsigned long myChannelNumber = 2452481;
const char * myWriteAPIKey = "98W7XALQLC0C0MNR";

int keyIndex = 0;       
WiFiClient  client;


const float vCalibration = 175;
// EnergyMonitor instances for each phase
EnergyMonitor emon1;
EnergyMonitor emon2;
EnergyMonitor emon3;


int volt = 0;
int cur = 0;
int watts1 = 0;
int watts2 = 0;
int watts3 = 0;

unsigned long lastMillis = millis();

void setup() {
  Serial.begin(9600);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
    WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak


  //voltage sensor pins and calibration
  emon1.voltage(34, vCalibration, 1.7); // Initialize the EnergyMonitor instance for phase 1
  emon2.voltage(35, vCalibration, 1.7); // Initialize the EnergyMonitor instance for phase 2
  emon3.voltage(32, vCalibration, 1.7); // Initialize the EnergyMonitor instance for phase 3


  //current sensor pins 
  emon1.current(13, 3.3);       // Current: input pin, calibration.
  emon2.current(14, 3.3);       // Current: input pin, calibration.
  emon3.current(33, 3.3);       // Current: input pin, calibration.

  // A small delay for the system to stabilize
  delay(1000);
}

void loop()
 {

     phase1();
    phase2();
  phase3();

    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);//update data
  if(x == 200){//responese ok mean 200
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

delay(20000); // Wait 20 seconds to update the channel again defalut
  }

  void phase1()
  {
    // Measure voltage and current for phase 1
  emon1.calcVI(20, 1000);
  float supplyVoltage1 = emon1.Vrms;
  float Irms1            = emon1.Irms;             // Extract Irms into Variable

  Serial.println("Phase 1 Irms = " + String(Irms1));
  ThingSpeak.setField(2, Irms1);

  watts1 = supplyVoltage1 * Irms1;

  ThingSpeak.setField(3, watts1);

   // Serial.println("Phase 1 Irms = ", (Irms1 ));
    // Serial.println(Irms1);
  
  // Check if voltage for phase 1 is above 100V
  if (supplyVoltage1 >= 100 ) 
  {

    Serial.println("Phase 1 Voltage: " + String(supplyVoltage1));
    ThingSpeak.setField(1, supplyVoltage1);


    //Serial.print("Phase 1 Voltage: ");
    //Serial.println(supplyVoltage1);
  } else {
    //Serial.println(volt);
    //Serial.println("Phase 1 =");
    Serial.println("Phase 1 Voltage: " + String(volt));
  }
  }
void phase2()
{
  // Measure voltage and current for phase 2
  emon2.calcVI(20, 1000);
  float supplyVoltage2 = emon2.Vrms;
  float Irms2            = emon2.Irms;             // Extract Irms into Variable

    Serial.println("Phase 2 Irms = " + String(Irms2));
      ThingSpeak.setField(5, Irms2);

    watts2 = supplyVoltage2 * Irms2;
    ThingSpeak.setField(6, watts2);

    //Serial.println("Phase 2 Irms");
     //Serial.println(Irms2);


    // Check if voltage for phase 2 is above 100V
  if (supplyVoltage2 >= 100) {
    Serial.println("Phase 2 Voltage: " + String(supplyVoltage2));
    ThingSpeak.setField(4, supplyVoltage2);
    //Serial.print("Phase 2 Voltage: ");
    //Serial.println(supplyVoltage2);
  } 
  else 
  {
     Serial.println("Phase 2 Voltage: " + String(volt));
    //Serial.println(volt);
    //Serial.println("Phase 2 = ");
  }

}
  
void phase3()
{
  // Measure voltage and current for phase 3
  emon3.calcVI(20, 1000);
  float supplyVoltage3 = emon3.Vrms;
  float Irms3            = emon3.Irms;             // Extract Irms into Variable

  Serial.println("Phase 3 Irms = " + String(Irms3));
  //ThingSpeak.setField(8, number);

    //Serial.println("Phase 3 Irms");
     //Serial.println(Irms3);

      watts3 = supplyVoltage3 * Irms3;
    ThingSpeak.setField(8, watts3);

    // Check if voltage for phase 3 is above 100V
  if (supplyVoltage3 >= 100) 
  {
    Serial.println("Phase 3 Voltage: " + String(supplyVoltage3));
    ThingSpeak.setField(7, number);
    //Serial.print("Phase 3 Voltage: ");
    //Serial.println(supplyVoltage3);
  }
   else 
  {

     Serial.println("Phase 3 Voltage: " + String(volt));
   //Serial.println(volt);
   // Serial.println("Phase 3 = ");
  }
delay(500); // Delay before taking the next reading
}
