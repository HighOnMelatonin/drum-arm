#include <SPI.h>
#include <SD.h>
#include <ESP32Servo.h>

#define ledPin 2
#define csPin 5 // sd pin

// Servo Pins
#define rightTurnServo 32
#define rightHitServo 33
#define leftTurnServo 25
#define leftHitServo 26
#define hitSpeedDelay 2500
#define swivelSpeedDelay 4000

// Servo constants
#define upStart 90  // starting pos for updown
#define endDown 20 // ending pos for updown
#define leftStart 90  // starting pos for left hand
#define leftEnd 150  // rim pos for left hand
#define rightStart 90  // starting pos for right hand
#define rightAngle 30 // rim pos for right hand

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
uint64_t previous_note = 0;
String filename = "/translated.csv";
File csvFile;
bool leftCenter = true;  // true: at center, false: not at center
bool rightCenter = true; // true: at center, false: not at center

char buf[32];

void hitDrum(bool dexterity){
  if (dexterity) {
    // right hand hit
    for (rightHPos = upStart; rightHPos >= endDown; rightHPos -= 1) {
      servoRH.write(rightHPos);
      delayMicroseconds(hitSpeedDelay);
    }
    previous_note = millis();
    for (rightHPos = endDown; rightHPos <= upStart; rightHPos += 1) {
      servoRH.write(rightHPos);
      delayMicroseconds(hitSpeedDelay);
    }
  }
  else{
    // left hand hit
    for (leftHPos = upStart; leftHPos >= endDown; leftHPos -= 1) {
      servoLH.write(leftHPos);
      delayMicroseconds(hitSpeedDelay);
    }
    previous_note = millis();
    for (leftHPos = endDown; leftHPos <= upStart; leftHPos += 1) {
      servoLH.write(leftHPos);
      delayMicroseconds(hitSpeedDelay);
    }
  }
  return;
}

void rightSwivel() {
  Serial.println("Entered right swivel func");
  if (rightCenter) {
    // Right is at center position (move to side)
    for (rightSPos = rightStart; rightSPos >= rightAngle; rightSPos -= 1) {
      servoRT.write(rightSPos);
      delayMicroseconds(swivelSpeedDelay);
      Serial.println(rightSPos);
    }
  }
  else{
    // Right is at side position (return to center)
    for (rightSPos = rightAngle; rightSPos <= rightStart; rightSPos += 1) {
      servoRT.write(rightSPos);
      delayMicroseconds(swivelSpeedDelay);
    }
  }

  rightCenter = !rightCenter;
}

void leftSwivel() {
  Serial.println("Entered left swivel func");
  if (leftCenter) {
    // Left is at center position (move to side)
    for (leftSPos = leftStart; leftSPos <= leftEnd; leftSPos += 1) {
      servoLT.write(leftSPos);
      delayMicroseconds(swivelSpeedDelay);
    }
  }
  else{
    // Left is at side position (return to center)
    for (leftSPos = leftEnd; leftSPos >= leftStart; leftSPos -= 1) {
      servoLT.write(leftSPos);
      delayMicroseconds(swivelSpeedDelay);
    }
  }

  leftCenter = !leftCenter;
}

void setup(){
  pinMode(ledPin, OUTPUT);
  pinMode(csPin, OUTPUT); // the hardware SS pin must be kept as an output or the SD library functions will not work
  Serial.begin(9600);
  SD.begin(csPin);
  csvFile = SD.open(filename, FILE_READ);

  servoRT.attach(rightTurnServo, 500, 2500);
  servoRH.attach(rightHitServo, 500, 2500);
  servoLT.attach(leftTurnServo, 500, 2500);
  servoLH.attach(leftHitServo, 500, 2500);

  // test hit
  hitDrum(true);
  delay(500);
  rightSwivel();
  delay(500);

  hitDrum(true);
  delay(500);
  rightSwivel();
  
}

//char buffer2[32];

void loop(){
  /*
  Sequence of events:
  1. Read swivel direction
  2. Read delay time
  3. Delay time
  4. Hit drum
  */
  digitalWrite(ledPin, HIGH);
  Serial.println("loop body");

  if (csvFile.available()) {
    Serial.println("CSVFile is available");
    memset(&buf[0], 0, sizeof(buf));
    csvFile.readBytesUntil('\n', buf, 32);
    //delay(1);
      
      // int s;
      // int j = 0;
      // s = csvFile.read();

      // // This block determines swivel
      // while (s != ',') {
      //   // Check if turn
      //   if (s == '1') {
      //     if (dexterity) {
      //       Serial.println("turn right");
      //       // Right swivel
      //       rightSwivel();
      //     }
    //       else{
    //         Serial.println("turn left");
    //         // Left swivel
    //         leftSwivel();
    //     }
    //     s = csvFile.read();
    //   }

    //   // This block determins delay time
    //   while ((s = csvFile.read()) != '\n') {
    //     buf[j] = s;
    //     j++;
    //     Serial.write(s);
    //   }

    //   Serial.write('\n');
    // }

    // delaytime is string, timedelay is int
    if (buf[0] == '1') {
      if (dexterity) {
        Serial.println("turn right");
        // Right swivel
        rightSwivel();
      }
      else{
        Serial.println("turn left");
        // Left swivel
        leftSwivel();
      }
    }
    String delaytime = "";
    for (int j = 2; j < 32; j++) {
      if (buf[j] = '\0') {
        delaytime += (char) buf[j];
      }
    }
    int timedelay = delaytime.toInt();
    Serial.println(timedelay);
    // insert offset
    while((millis()-previous_note)< timedelay){
      delayMicroseconds(100);
    }

    // hit drum
    hitDrum(dexterity);

    dexterity = !dexterity;  // switch dexterity
    digitalWrite(ledPin, LOW);
  }else{
    csvFile.close();
    if(!leftCenter){
      leftSwivel();
    }
    if(!rightCenter){
      rightSwivel();
    }
    delay(1000);
    csvFile = SD.open(filename, FILE_READ);
  }
  
}
