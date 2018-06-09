/*
 * AM2302 Basic
 * Temperature and Humidity Sensor
 * Adafruit 
 * https://www.adafruit.com/product/393
 * 
 * Cory Welch
 * corywelch.ca 
 * https://github.com/corywelch
 * 
 */

/* 
 *  AM2302
 *  Red    : VCC = 3.3V - 5V
 *  Black  : Ground
 *  Yellow : Data (need to experiment with different pullups)
 *  
 *  This sensor is basically a DHT22 in a larger case. 
 *  Some online says it already has internal pullup resistor still to be confirmed
 *  
 */

// Includes
#include "DHT.h" // DHT Library for Sensor

// Pins
#define AM2302_PIN 7 // Data Pin for AM2302

// Global Variables
#define EN_SERIAL true
#define BAUD 9600
#define READ_DELAY 3000 // Minimum is 2sec based on sensor spec sheet
#define AM2302_TYPE DHT22

// Global Functions
void sprint(String text);
void sprintln(String text);
void readSensor();

// Objects
DHT AM2302(AM2302_PIN, AM2302_TYPE); // Object for AM2302

void setup() {
  // Initializing Serial
  if(EN_SERIAL)
  Serial.begin(BAUD);

  // Initializing Reading of the AM2302
  AM2302.begin();

  // Delay before first read
  delay(READ_DELAY);
  
  sprintln("Setup Complete");

}

void loop() {

  readSensor();

  delay(READ_DELAY);

}

void readSensor(){

  // Retrieve Values from Sensor
  float humidity = AM2302.readHumidity(); // % 0 - 99.9
  float temperature = AM2302.readTemperature(); // C -40 - 80

  // Read Check
  if( isnan(humidity) || isnan(temperature) ){
    sprintln("Failed to read properly");
  }

  // Compute Heat Index
  float heatIndex = AM2302.computeHeatIndex(temperature, humidity, false); // False needed cause default uses Fahrenheit

  // Print Results to Console
  String readings = "";
  readings += "Temperature " + String(temperature) + "*C ";
  readings += "Humidity " + String(humidity) + "% ";
  readings += "Heat Index " + String(heatIndex) + "*C ";

  sprintln(readings);
  
}

void sprint(String text){
  if(EN_SERIAL){
    Serial.print(text);
  } else {
    return;
  }
}

void sprintln(String text){
  if(EN_SERIAL){
    Serial.println(text);
  } else {
    return;
  }
}
