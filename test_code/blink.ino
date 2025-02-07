// blink onboard LED on and off every second

#define ledPin = 2

void setup() {
    // Set up the LED pin as an output:
    pinMode(ledPin, OUTPUT);
}

void loop() {
    // Turn the LED on:
    digitalWrite(ledPin, HIGH);
    // Wait for 1 second:
    delay(1000);
    // Turn the LED off:
    digitalWrite(ledPin, LOW);
    // Wait for 1 second:
    delay(1000);
}
