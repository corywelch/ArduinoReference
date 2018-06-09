//  Pins
//  Arduino 5V out TO BT VCC
//  Arduino GND to BT GND
//  Arduino D9 to BT RX through a voltage divider
//  Arduino D8 BT TX (no need voltage divider)

//#include <SoftwareSerial.h>
#include <data.pb.h>
#include <pb_encode.h>


//SoftwareSerial BTserial(8, 9); // RX | TX
pb_ostream_t pb_out;
const long baudRate = 57600; 
char c = ' ';
int32_t term_char[] = {999,999,999,999};
const int DATA_LENGTH = 4;

// Testing setup
int testLength = 50;
int32_t test_data[][DATA_LENGTH] = {{234,134,124,764},{363,518,111,134},{166,646,662,698}};

void setup() 
{
    Serial.begin(baudRate);
//    BTserial.begin(baudRate);
    pb_out = as_pb_ostream(Serial);
}

 
void loop()
{    
    if (Serial.available()){
        c = Serial.read(); 
        if (String(c).equals("d")) {
          for (int i=1; i<=testLength; i++){
            dumpData(test_data[i%3]);
            delay(10);
          }
          delay(2);
          Serial.println(millis());
        } else if (String(c).equals("m")) {
          Serial.println(millis()); 
        }
    }
}

void dumpData(int32_t* data) {
  GoYoData point = GoYoData_init_default;  
  point.fsr = data[0];
  point.x_accel = data[1];
  point.y_accel = data[2];
  point.z_accel = data[3];
  if (!pb_encode(&pb_out, GoYoData_fields, &point)) {
    Serial.println(PB_GET_ERROR(&pb_out));
  }
}

static bool pb_print_write(pb_ostream_t *stream, const pb_byte_t *buf, size_t count) {
    Print* p = reinterpret_cast<Print*>(stream->state);
    size_t written = p->write(buf, count);
    return written == count;
};

pb_ostream_s as_pb_ostream(Print& p) {
    return {pb_print_write, &p, SIZE_MAX, 0};
};


