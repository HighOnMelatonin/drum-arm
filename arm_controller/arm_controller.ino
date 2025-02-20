#include <SPI.h>
#include <SD.h>
#include <ESP32Servo.h>

#define ledPin 2
#define csPin 5 // sd pin

// Servo Pins
#define rightTurnServo 32
#define rightHitServo 33
#define leftTurnServo 34
#define leftHitServo 35

// Servo constants
#define upStart 90  // starting pos for updown
#define endDown 180 // ending pos for updown
#define leftStart 90  // starting pos for left hand
#define leftEnd 30  // rim pos for left hand
#define rightStart 0  // starting pos for right hand
#define rightAngle 60 // rim pos for right hand

// Offsets for movement
#define swivelOffset 0  // offset time for swivel
#define hitOffset 0 // offset time for hit

Servo servoRT;
Servo servoRH;
Servo servoLT;
Servo servoLH;

int rightSPos = 0; // Initialise current position of right swivel motor
int leftSPos = 0;  // Initialise current position of left swivel motor
int rightHPos = 90; // Initialise current position of right hand motor
int leftHPos = 90; // Initialise current position of left hand motor

byte inByte;
bool sdInitSuccess = false; //card init status
bool dexterity = true;   // true: right, false: left
long long timedelay;
File csvFile;

void setup(){
  pinMode(ledPin, OUTPUT);
  pinMode(csPin, OUTPUT); // the hardware SS pin must be kept as an output or the SD library functions will not work
  Serial.begin(9600);
  SD.begin(csPin);
  csvFile = SD.open("/translated.csv", FILE_READ);

  servoRT.attach(rightTurnServo, 500, 2500);
  servoRH.attach(rightHitServo, 500, 2500);
  servoLT.attach(leftTurnServo, 500, 2500);
  servoLH.attach(leftHitServo, 500, 2500);
}


void loop(){
  /*
  Sequence of events:
  1. Read swivel direction
  2. Read delay time
  3. Delay time
  4. Hit drum
  */
  digitalWrite(ledPin, HIGH);

  for (int i = 0; i < 10; i++){

    int buf[32];
    if (csvFile.available()) {
      Serial.println("CSVFile is available");
      int s;
      int j = 0;
      s = csvFile.read();

      // This block determines swivel
      while (s != ',') {
        if (s == '0') {
          if (dexterity) {
            Serial.println("turn right");
            // Right swivel
            if (rightSPos == rightStart) {
              // Right is at start position (move to end)
              for (rightSPos = rightStart; rightSPos <= rightAngle; rightSPos += 1) {
                servoRT.write(rightSPos);
              }
            }
            else{
              // Right is at end position (return to start)
              for (rightSPos = rightAngle; rightSPos >= rightStart; rightSPos -= 1) {
                servoRT.write(rightSPos);
              }
            }
          }
          else{
            Serial.println("turn left");
            // Left swivel
            if (leftSPos == leftStart) {
              // Left is at start position (move to end)
              for (leftSPos = leftStart; leftSPos >= leftEnd; leftSPos -= 1) {
                servoLT.write(leftSPos);
              }
            }
            else{
              // Left is at end position (return to start)
              for (leftSPos = leftEnd; leftSPos <= leftStart; leftSPos += 1) {
                servoLT.write(leftSPos);
              }
            }
          }
        }
      }

      // This block determins delay time
      while ((s = csvFile.read()) != '\n') {
        buf[j] = s;
        j++;
        Serial.write(s);
      }

      Serial.write('\n');
    }

    // delaytime is string, timedelay is int
    String delaytime = "";
    for (int j = 0; j < 32; j++) {
      if (buf[j] = '\0') {
        delaytime += (char) buf[j];
      }
    }
    int timedelay = delaytime.toInt();
  }

  // insert offset
  delay(timedelay);

    // hit drum
    if (dexterity) {
      // right hand hit
      for (rightHPos = upStart; rightHPos <= endDown; rightHPos += 1) {
        servoRH.write(rightHPos);
      }
      for (rightHPos = endDown; rightHPos >= upStart; rightHPos -= 1) {
        servoRH.write(rightHPos);
      }
    }
    else{
      // left hand hit
      for (leftHPos = upStart; leftHPos <= endDown; leftHPos += 1) {
        servoLH.write(leftHPos);
      }
      for (leftHPos = endDown; leftHPos >= upStart; leftHPos -= 1) {
        servoLH.write(leftHPos);
      }
    }

  dexterity = !dexterity;  // switch dexterity
  digitalWrite(ledPin, LOW);
}
