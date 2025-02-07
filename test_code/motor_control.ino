// Move servo motor 180 degrees
// Adapted from https://github.com/madhephaestus/ESP32Servo/blob/master/examples/Sweep/Sweep.ino

#include <ESP32Servo.h>

#define servoPin 13 // define the pin for the servo motor
#define angle 180   // define the angle to move the servo motor
#define ledPin 2    // define the pin for the LED

Servo myservo;  // create servo object to control a servo
int pos = 0;

void setup() {
    myservo.attach(servoPin);  // attaches the servo on pin servoPin to the servo object

    pinMode(ledPin, OUTPUT);   // set the LED pin as an output
    pinMode(servoPin, OUTPUT); // set the servo pin as an output
}

void loop() {
    digitalWrite(ledPin, HIGH); // turn the LED on to indicate start

    for (pos = 0; pos <= angle; pos += 1) { // goes from 0 degrees to angle degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = angle; pos >= 0; pos -= 1) { // goes from angle degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
    }

    digitalWrite(ledPin, LOW); // turn the LED off to indicate end
}
