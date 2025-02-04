// Adapted from https://github.com/FortySevenEffects/arduino_midi_library/blob/master/examples/Basic_IO/Basic_IO.ino

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

// set motor pins...

void setup(){
  MIDI.begin(MIDI_CHANNEL_OMNI); //Listen to all incoming messages
}

void loop(){
  if (MIDI.read()){
    digitalWrite(LED_BUILTIN, HIGH);
    MIDI.sendNoteOn(42, 127, 1); // Send a Note (pitch 42, speed 127, channel 1)
    delay(1000); // 1 second
    MIDI.sendNoteOff(42, 0, 1);  // Stop the note
    digitalWrite(LED_BUILTIN, LOW);
  }
}
