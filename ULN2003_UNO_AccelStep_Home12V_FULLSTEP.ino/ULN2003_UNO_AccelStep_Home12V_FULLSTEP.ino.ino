/*
  Serial Monitor Control for Homing a stepper motor using AccelStepper

  dw MOTOR gets Hot if you leave it on for more than 3 minutes!  digitalWrite (stepper pins, LOW); to limit stops motor from overheating

  Original sketch Created by Yvan / https://Brainy-Bits.com

  Modified by dw for ULN2003 driver

  28BYJ-48 12V DC 4-Phase 5-Wire Geared Stepper Motor; 2048 FULL steps per revolution

*/

#include <AccelStepper.h>

// AccelStepper Setup
// dw you don't have to comment out the mode you don't want but it helps to know which mode you picked
#define FULLSTEP 4    //no load MaxSeed = 800; Acceleration = No Limit; 2048 FULLSTEP's per Revolution
//#define HALFSTEP 8      //no load MaxSpeed = 2000; Acceleration = No Limit; 4096 HALFSTEP's per Revolution


#define motorPin5  8     // Blue   - 28BYJ48 pin 1
#define motorPin6  9     // Pink   - 28BYJ48 pin 2
#define motorPin7  10    // Yellow - 28BYJ48 pin 3
#define motorPin8  11    // Orange - 28BYJ48 pin 4
// Red    - 28BYJ48 pin 5 (VCC)

// NOTE: The sequence 1-3-2-4 is required for proper sequencing of 28BYJ-48

AccelStepper stepperX(FULLSTEP, motorPin5, motorPin7, motorPin6, motorPin8);

// Define the Pins used
#define home_switch 12 // Pin 12 connected to Home Switch (NO MicroSwitch); 10K Resistor

// Stepper Travel Variables
long TravelX;  // Used to store the X value entered in the Serial Monitor
int move_finished = 1; // Used to check if move is completed
long initial_homing = -1; // Used to Home Stepper at startup


void setup() {
  Serial.begin(9600);  // Start the Serial monitor with speed of 9600 Bauds

  pinMode(home_switch, INPUT_PULLUP);

  delay(5);  //


  //  Set Max Speed and Acceleration of each Steppers at startup for homing
  stepperX.setMaxSpeed(100.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperX.setAcceleration(100.0);  // Set Acceleration of Stepper


  // Start Homing procedure of Stepper Motor at startup

  Serial.print("Stepper is Homing . . . . . . . . . . . ");

  while (digitalRead(home_switch)) {  // Make the Stepper move CW until the switch is activated
    stepperX.moveTo(initial_homing);  // Set the position to move to
    initial_homing++;  // dw Increase by 1 ++ for CW
    stepperX.run();  // Start moving the stepper
    delay(5);
  }

  stepperX.setCurrentPosition(0);  // Set the current position as zero for now
  stepperX.setMaxSpeed(100.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperX.setAcceleration(100.0);  // Set Acceleration of Stepper
  initial_homing = 1;

  while (!digitalRead(home_switch)) { // Make the Stepper move CCW until the switch is deactivated
    stepperX.moveTo(initial_homing);
    stepperX.run();
    initial_homing--;  //dw Increase by 1 -- for CCW
    delay(5);
  }
  // disable outputs to geared stepper (no need to consider hold position)
  digitalWrite (8, LOW);
  digitalWrite (9, LOW);
  digitalWrite (10, LOW);
  digitalWrite (11, LOW);

  stepperX.setCurrentPosition(0);
  Serial.println("Homing Completed");
  Serial.println("");
  stepperX.setMaxSpeed(1000.0);      // Set Max Speed of Stepper (Faster for regular movements)
  stepperX.setAcceleration(1000.0);  // Set Acceleration of Stepper

  // Print out Instructions on the Serial Monitor at Start
  Serial.println("Enter Travel distance: ");
}

void loop() {

  while (Serial.available() > 0)  { // Check if values are available in the Serial Buffer

    move_finished = 0; // Set variable for checking move of the Stepper

    TravelX = Serial.parseInt(); // Put numeric value from buffer in TravelX variable
    if (TravelX < 0 || TravelX > 2000) {  // Make sure the position entered is not beyond the HOME or MAX position
      Serial.println("");
      Serial.println("Please enter a value greater than zero and smaller or equal to 2000.....");
      Serial.println("");
    } else {
      Serial.print("Moving stepper into position: ");
      Serial.println(TravelX);

      stepperX.moveTo(-1 * TravelX); // Set new moveto position of Stepper  dw -1*TravelX for CCW
      while (stepperX.distanceToGo() != 0)   //dw added this line
      { //dw added this line
        stepperX.run();                      //dw added this line
      }                                      //dw added this line
      delay(1000);  // Wait 1 seconds before moving the Stepper
    }
  }

  if (TravelX >= 0 && TravelX <= 2000) {

    // Check if the Stepper has reached desired position
    if ((stepperX.distanceToGo() != 0)) {

      stepperX.run();  // Move Stepper into position

    }

    // If move is completed display message on Serial Monitor
    if ((move_finished == 0) && (stepperX.distanceToGo() == 0)) {
      Serial.println("COMPLETED!");
      Serial.println("");
      Serial.println("Enter Travel distance: ");
      stepperX.disableOutputs();    // dw added this line; stops motor from overheating when off
      move_finished = 1; // Reset move variable
    }
    
    // disable outputs to geared stepper (no need to consider hold position)
    digitalWrite (8, LOW);
    digitalWrite (9, LOW);
    digitalWrite (10, LOW);
    digitalWrite (11, LOW);

  }
}
