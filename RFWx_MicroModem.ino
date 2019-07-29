#include <Wire.h>
#include <stdlib.h>
#include <SparkFunBME280.h>
#include "CountDown.h"

//CHANGE THIS VARIABLES:
//**** APRS COMMENT (max 43 chars)
const char APRS_CMNT[] = "RFWx MM datalogger TARTAMARE Rimigliano 18";

//**** APRS_PRJ, Telemetry Project Title (max 23 chars)
const char APRS_PRJ[] = "MicroModem RFWx";

const char callsign[] = "IW5EJM";
const char ssid[] = "4";
const char tlm_callsign[] = "IW5EJM";
const char longitude[] = "01031.54E";
const char latitude[] = "4301.86N";
const int altitude  = 1;

//APRS WX data tx interval
const int BeaconTime = 600;

//no edit below this line

// Global sensor object
  BME280 mySensor;
  
//countdown to trigger data transmission
CountDown CD(CountDown::SECONDS);

int counter=0;
char sentence[150];

#define Serial2MM Serial

typedef struct  {
  float temperatureC;
  float temperatureF;
  float pressure ;
  float humidity;
} WeatherStruct, *WeatherStructPtr;
WeatherStruct wx;    //declare la structure
WeatherStructPtr wx_ptr = &wx;


//read WX data 
void getBmeValues(){
    
    float pres;

    wx.temperatureC = mySensor.readTempC();

    //*** calc standardized barometric pressure
    if ((pres=mySensor.readFloatPressure()) > 0.1f)
      wx.pressure = pres * ( pow(1.0 -(0.0065 * altitude * -1 /(273.15+wx.temperatureC)), 5.255));
    else
      wx.pressure = 0.0f;
        
    wx.temperatureF = mySensor.readTempF();
    wx.humidity =  mySensor.readFloatHumidity();
}


//send APRS packet
void Send2APRSWx()
{

  sprintf(sentence, "!=%s/%s_.../...g...t%03dr...p...P...h%02db%05d",                     latitude, 
                                                                                          longitude, 
                                                                                          (int)(wx.temperatureF), 
                                                                                          (int)(wx.humidity), 
                                                                                          (int)(wx.pressure/10));
  
  Serial2MM.println(sentence);
 }
 

void Send2APRSPos()
{
  //send position packet
  sprintf(sentence, "!=%s/%s&%s",latitude,longitude,APRS_CMNT);                                                    
  Serial2MM.println(sentence);
//
//  //send telemetry data
//  delay(1);
//  sprintf(sentence2, "!=%s/%s&%s", station.latitude,station.longitude,sets.APRS_CMNT);
//  Serial2MM.println(sentence2);
  
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  //init BME280
  mySensor.settings.commInterface = I2C_MODE;
  mySensor.settings.I2CAddress = 0x76;
  mySensor.settings.runMode = 3; //Normal mode
  mySensor.settings.tStandby = 0;
  mySensor.settings.filter = 0;
  mySensor.settings.tempOverSample = 1;
  mySensor.settings.pressOverSample = 1;
  mySensor.settings.humidOverSample = 1;
  delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
  mySensor.begin();
  
  Serial2MM.begin(9600); while (!Serial2MM); //serial modem init
  
  delay(1000);
  sprintf(sentence, "c%s",callsign);                                                    
  Serial2MM.println(sentence);
  delay(350);
  sprintf(sentence, "sc%s",ssid);                                                    
  Serial2MM.println(sentence);
  delay(350);
  sprintf(sentence, "S");                                                    
  Serial2MM.println(sentence);
  delay(350);
  Serial2MM.flush();
  
  CD.stop();
  delay(1000);

}

void loop() {
  
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(50); 
  digitalWrite(LED_BUILTIN, LOW);
  
  if (CD.remaining() < 1) { 
    CD.stop();
    getBmeValues();
    digitalWrite(LED_BUILTIN, HIGH);
    Send2APRSWx();
    counter++;
    delay(4500);
    digitalWrite(LED_BUILTIN, LOW);
    if (counter >= 3) {
        counter=0;
        digitalWrite(LED_BUILTIN, HIGH);
        Send2APRSPos();
        delay(4500);
        digitalWrite(LED_BUILTIN, LOW);
         }
    CD.start(BeaconTime);
    }

  
  delay(1000);

}
