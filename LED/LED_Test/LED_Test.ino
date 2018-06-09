/*
 * FYDP
 * 
 * Hardware LED Test
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
 */

  // General
  #define BAUD 9600
  
  // LED Pins
  #define RED_LED 8
  #define GREEN_LED 7
  #define BLUE_LED 9

  


void setup() {
  
  // SETUP DIGITAL OUTPUTS
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  digitalWrite(RED_LED,LOW);
  digitalWrite(GREEN_LED,LOW);
  digitalWrite(BLUE_LED,LOW);
  
}

void loop() {

  delay(2000);
  digitalWrite(BLUE_LED,LOW);
  digitalWrite(GREEN_LED,LOW);
  digitalWrite(RED_LED,HIGH);
  
  delay(2000);
  digitalWrite(BLUE_LED,LOW);
  digitalWrite(GREEN_LED,HIGH);
  digitalWrite(RED_LED,LOW);
  
  delay(2000);
  digitalWrite(BLUE_LED,HIGH);
  digitalWrite(GREEN_LED,LOW);
  digitalWrite(RED_LED,LOW);

  delay(2000);
  digitalWrite(BLUE_LED,HIGH);
  digitalWrite(GREEN_LED,LOW);
  digitalWrite(RED_LED,HIGH);

  delay(2000);
  digitalWrite(BLUE_LED,LOW);
  digitalWrite(GREEN_LED,HIGH);
  digitalWrite(RED_LED,HIGH);

  delay(2000);
  digitalWrite(BLUE_LED,HIGH);
  digitalWrite(GREEN_LED,HIGH);
  digitalWrite(RED_LED,LOW);

  delay(2000);
  digitalWrite(BLUE_LED,LOW);
  digitalWrite(GREEN_LED,LOW);
  digitalWrite(RED_LED,LOW);

}
