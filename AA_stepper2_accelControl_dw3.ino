/*
Runs ONE 28BYJ-48 stepper motor with AccelStepper Library.
Uses up a 9V Battery very quick!!!
Coded with pauses between CW and CCW.
ULN2003 Driver Board; 28BYJ-48 5V DC 4-Phase 5-Wire Stepper Motor
*/

#include <AccelStepper.h>

#define FULLSTEP 4
#define HALFSTEP 8


                        
#define motorPin5  8     // Blue   - 28BYJ48 pin 1
#define motorPin6  9     // Pink   - 28BYJ48 pin 2
#define motorPin7  10    // Yellow - 28BYJ48 pin 3
#define motorPin8  11    // Orange - 28BYJ48 pin 4
                        // Red    - 28BYJ48 pin 5 (VCC)

// NOTE: The sequence 1-3-2-4 is required for proper sequencing of 28BYJ-48

AccelStepper stepper2(HALFSTEP, motorPin5, motorPin7, motorPin6, motorPin8);
int i;
int j;  // int j is Number of cycles

void setup() {
// nothing to do inside setup but void setup() cannot be deleted!
}


void runMotor(int MaxSpeed, int Acceleration, int newPos, int j)


{
  delay(100);
  stepper2.setCurrentPosition(0);  
  for ( int i = 0; i < j; i++ )      // int j is Number of cycles 

  {
    //set speed,acceleration and 2*amplitude of the pendulum
    stepper2.setMaxSpeed(MaxSpeed);
    stepper2.setAcceleration(Acceleration);
    stepper2.runToNewPosition(newPos);
    stepper2.stop();
    delay(3000);
    stepper2.runToNewPosition(0);
    delay(3000);
  }

}

void loop()
{
  
 runMotor(1000, 50, -2048, 3);  // 1/2 revolution
 
}
 

 
