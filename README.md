# Drum Arm
- [Drum Arm](#drum-arm)
  - [Project Scope](#project-scope)
    - [Mechanical](#mechanical)
    - [Software](#software)
  - [Directories](#directories)
    - [raw\_midi](#raw_midi)
    - [test\_code](#test_code)
    - [arm\_controller](#arm_controller)
  - [Setup](#setup)
    - [Dependencies](#dependencies)
    - [Wiring](#wiring)
  - [Code](#code)
    - [reader.py](#readerpy)

## Project Scope
### Mechanical
Printed compliant mechanisms to emulate human wrists for the robot arm

### Software
Software to emulate sequence of movements when playing the drums; Main loop will 
1. Read one line of csv
2. Process swivel boolean
3. Execute swivel
4. Process delay time
5. Execute delay
6. Hit drum
7. Change dexterity
8. Repeat

---

## Directories
### raw_midi
Contains raw midi files for testing

### test_code
Basic code from examples and templates to test motors and file reading

### arm_controller
Contains the main `arm_controller.ino` that controls all movement and file reading

## Setup
### Dependencies
`reader.py` requires the `mido` library, do `pip install -r requirements.txt` to install

### Wiring
Pins have been defined at the start `arm_controller.ino` <br>
Servo pins:
- Right swivel servo (rightTurnServo): 32
- Right hit servo (rightHitServo): 33
- Left swivel servo (leftTurnServo): 34
- Left hit servo (leftHitServo): 35 <br>
SD Pin:
- csPin (csPin): 5

---

## Code
### reader.py
Use `reader.py` to convert midi files into readable csv format, csv is formatted as such `<swivel boolean (0/1)>,<delay time (ms)>`.

Copy the output file `translated.csv` onto an SD card and plant the SD card into the reader on the circuit.

Start the circuit

Reader handles swivel logic as well, determining when to swivel and when not to swivel, if the next note is the same as the previous note, do not swivel, if next note is different, swivel

### arm_controller.ino
Reads from csv file on SD card, 
