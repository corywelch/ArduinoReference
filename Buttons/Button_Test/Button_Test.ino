


  
  // General
  #define BAUD 9600
  #define  DEBOUNCE_TIME 2000

  #define SAMPLING_RATE 1 // how many per second
  int SAMPLE_DELAY = 1000/SAMPLING_RATE; 

  // Button / Switch Pins
  #define RECORD_BTN 2
  
  // LED Pins
  #define RED_LED 8
  #define GREEN_LED 7

  volatile long lastSerialTime = 0;

  // Record Status
  volatile bool recording = false;
  volatile bool recordBtnStatus = false;
  volatile bool recordBtnPreviousStatus = false;
  volatile int recordLastPressed = 0;
  volatile bool recordingStateChange = false;

void setup() {

  Serial.begin(BAUD);
  
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RECORD_BTN,INPUT);

  recording = false;
  recordingStateChange = true;

  Serial.println("Setup Complete");
}

void loop() {
  //Serial.println(digitalRead(RECORD_BTN));
  recordButtonPressed();

  if(recording && recordingStateChange){
    //First Run of Recording  
    digitalWrite(RED_LED,LOW);
    digitalWrite(GREEN_LED,HIGH);
    Serial.println("Recording Started");
    
    recordingStateChange = false;
  } else if(!recording && recordingStateChange){
    // Just finished Recording
    digitalWrite(RED_LED,HIGH);
    digitalWrite(GREEN_LED,LOW);
    Serial.println("Recording Finished");
    
    recordingStateChange = false;
  } else if(recording){
    // Recording
    if(millis() - SAMPLE_DELAY > lastSerialTime){
      lastSerialTime = millis();
      Serial.println(lastSerialTime);
    }
  } else {
     // Not Recording no Error
  }

}

void recordButtonPressed(){
  recordBtnStatus = digitalRead(RECORD_BTN);
  if(recordBtnStatus && !recordBtnPreviousStatus && millis() - recordLastPressed > DEBOUNCE_TIME){
    //Serial.println("Button Pressed");
    recording = !recording;
    recordingStateChange = true;
    recordLastPressed = millis();
  }
  recordBtnPreviousStatus = recordBtnStatus;
}
