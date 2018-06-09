/*
 * FYDP
 * 
 * FSR Test Code
 * 
 * Department of Systems Design Engineering
 * University of Waterloo
 * Class of 2018
 * 
 * SYDE 462 - Group 14
 * Cory Welch - 20520003
 * Sophia Castellarin - 20506069
 * Raunaq Suri - 20519728
 * Dhananjay (Danny) Gosain - 20524438
 * 
 * Technical Details
 * 
 * FSR Should be conncted to 3.3V on one side
 * Other side should be connected to both a 10k resistor to ground, and the analog pin
 * It creates a voltage divider with the FSR and the 10k
 * 
 * 
 */

//
// GLOBAL CONSTANTS
//

  #define BAUD 9600
  
  // FSR
  #define FSR 0 //Analog Pin
  
  // Data Variables
  int fsrReading = 0;
  

void setup() {

  Serial.begin(BAUD);
  Serial.println("FSR Testing");

}

void loop() {

  Serial.println(analogRead(FSR));
  
}

