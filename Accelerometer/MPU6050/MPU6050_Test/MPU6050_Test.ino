/*
 * WeFish
 * 
 * Accelerometer Reading and Angle Calculation
 * 
 * 
 * Department of Systems Design Engineering
 * University of Waterloo
 * Class of 2018
 * 
 * SYDE 362 - Group 6
 * Cory Welch - 20520003
 * 
 * 
 * Pin Out
 * VCC = 3.3V
 * GNG = GND
 * SCL = A5
 * SDA = A4
 * XDA = Open
 * XCL = Open
 * ADO = GND
 * INT = D2
 * 
 */

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu;
//MPU6050 mpu(0x69); // <-- use for AD0 high

#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards

#define pi 3.14159265
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

//Interrupt detection routine
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}
 

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
       // Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    
    //Serial.begin(115200);
    Serial.begin(9600);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // wait for ready
    /*
    Serial.println(F("\nSend any character to begin: "));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again
    //*/

    // Initialize Connection to MPU6050
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXAccelOffset(-4809);
    mpu.setYAccelOffset(1478);
    mpu.setZAccelOffset(2096);
    mpu.setXGyroOffset(-37);
    mpu.setYGyroOffset(-32);
    mpu.setZGyroOffset(35);

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(INTERRUPT_PIN, dmpDataReady, RISING);
        //attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
}

void loop() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;
 
        
        //mpu.dmpGetQuaternion(&q, fifoBuffer);
        Quaternion* qt = &q;
        int16_t qI[4];
        qI[0] = ((fifoBuffer[0] << 8) | fifoBuffer[1]);
        qI[1] = ((fifoBuffer[4] << 8) | fifoBuffer[5]);
        qI[2] = ((fifoBuffer[8] << 8) | fifoBuffer[9]);
        qI[3] = ((fifoBuffer[12] << 8) | fifoBuffer[13]);
          
        qt -> w = (float)qI[0] / 16384.0f;
        qt -> x = (float)qI[1] / 16384.0f;
        qt -> y = (float)qI[2] / 16384.0f;
        qt -> z = (float)qI[3] / 16384.0f;
        /*/
        
        Serial.print(" w ");
        Serial.print(qt->w);
        Serial.print(" x ");
        Serial.print(qt->x);
        Serial.print(" y ");
        Serial.print(qt->y);
        Serial.print(" z ");
        Serial.print(qt->z);
        //*/
        VectorFloat* gravityt = &gravity;
          
        //mpu.dmpGetGravity(&gravity, &q);  
        gravityt -> x = 2 * (qt -> x*qt -> z - qt -> w*qt -> y);
        gravityt -> y = 2 * (qt -> w*qt -> x + qt -> y*qt -> z);
        gravityt -> z = qt -> w*qt -> w - qt -> x*qt -> x - qt -> y*qt -> y + qt -> z*qt -> z;
        //*/

        
        Serial.print(" gx: ");
        Serial.print(gravityt->x);
        Serial.print(" gy: ");
        Serial.print(gravityt->y);
        Serial.print(" gz: ");
        Serial.println(gravityt->z);
 
    }
}

