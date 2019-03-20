/*
  Motor Gets Hot if run too long (>2 minutes)!

  "disableOutputs()" kills all coil current preventing motor from getting hot.
  "stop()" stops motor but does not kill coil current, motor gets hot.
  Runs ONE 28BYJ-48 stepper motor with AccelStepper Library.
  Uses up a 9V Battery very quick!!!
  Coded with pauses between CW and CCW.
  ULN2003 Driver Board; 28BYJ-48 5V DC 4-Phase 5-Wire Stepper Motor
  Four Switches for Speeds: Low, Medium, High, Ultra
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

int lowButton = 2;     //UNO Pin Number
int mediumButton = 3;  //UNO Pin Number
int highButton = 4;    //UNO Pin Number
int ultraButton = 5;   //UNO Pin Number


boolean low_Flag = HIGH;
boolean medium_Flag = HIGH;
boolean high_Flag = HIGH;
boolean ultra_Flag = HIGH;

int i;
int j;  // int j is Number of cycles
int runID = 0;


void setup()
{
  Serial.begin(9600);
  pinMode(lowButton, INPUT);     //Used 10K External pull-up resistor; INPUT_PULLUP command uses Internal pull-up resistor
  pinMode(mediumButton, INPUT);  //Used 10K External pull-up resistor; INPUT_PULLUP command uses Internal pull-up resistor
  pinMode(highButton, INPUT);    //Used 10K External pull-up resistor; INPUT_PULLUP command uses Internal pull-up resistor
  pinMode(ultraButton, INPUT);   //Used 10K External pull-up resistor; INPUT_PULLUP command uses Internal pull-up resistor
  Serial.println(" Finished setup routine.");
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
    stepper2.disableOutputs();    // stops motor from overheating when off
    delay(1500);           // pause after CW
    stepper2.runToNewPosition(0);
    stepper2.disableOutputs();  // stops motor from overheating when off
    delay(1500);     //pause after CCW
  }

}


void runAuto(int runID)
{
  switch (runID)
  {
    case 1:      // low speed

      runMotor(250, 25, -2048, 2);  //runMotor(int MaxSpeed, int Acceleration, int newPos, int j)
      // 2048 HALFSTEP = 1/2 revolution

      delay(10);
      stepper2.disableOutputs();   // stops motor from overheating when off
      runID = 0; // reset run ID for next run choice
      low_Flag = HIGH;
      break;

    case 2:    // medium speed

      runMotor(1000, 100, -2048, 2);  //runMotor(int MaxSpeed, int Acceleration, int newPos, int j)
      // 2048 HALFSTEP = 1/2 revolution

      delay(10);
      stepper2.disableOutputs();   // stops motor from overheating when off
      runID = 0; // reset run ID for next run choice
      medium_Flag = HIGH;
      break;

    case 3:   //  high speed

      runMotor(2000, 200, -2048, 2);  //runMotor(int MaxSpeed, int Acceleration, int newPos, int j)
      // 2048 HALFSTEP = 1/2 revolution

      delay(10);
      stepper2.disableOutputs();   // stops motor from overheating when off
      runID = 0; // reset run ID for next run choice
      high_Flag = HIGH;
      break;

    case 4:   // ultra speed

      runMotor(3000, 300, -2048, 2);  //runMotor(int MaxSpeed, int Acceleration, int newPos, int j)
      // 2048 HALFSTEP = 1/2 revolution

      delay(10);
      stepper2.disableOutputs();   // stops motor from overheating when off
      runID = 0; // reset run ID for next run choice
      ultra_Flag = HIGH;
      break;

    default:
      {
      }
  }
  runID = 0; // reset run ID for next run choice
  Serial.println("Finished Auto run");
}


void loop()
{
  runID = 0;
  low_Flag = digitalRead(lowButton);    // initially HIGH until button pushed
  medium_Flag = digitalRead(mediumButton);   //              "
  high_Flag = digitalRead(highButton);   //              "
  ultra_Flag = digitalRead(ultraButton);  //              "
  if (low_Flag == LOW && medium_Flag == HIGH && high_Flag == HIGH && ultra_Flag == HIGH)
  {
    runID = 1;
    Serial.println(" Low Speed.");
  }
  if (low_Flag == HIGH && medium_Flag == LOW && high_Flag == HIGH && ultra_Flag == HIGH)
  {
    runID = 2;
    Serial.println(" Medium Speed.");
  }
  if (low_Flag == HIGH && medium_Flag == HIGH && high_Flag == LOW && ultra_Flag == HIGH)
  {
    runID = 3;
    Serial.println(" High Speed.");
  }
  if (low_Flag == HIGH && medium_Flag == HIGH && high_Flag == HIGH && ultra_Flag == LOW)
  {
    runID = 4;
    Serial.println("Ultra Speed.");
  }
  Serial.print("runID = ");
  Serial.println(runID);
  if (runID != 0)
  {
    Serial.println("Auto run started.");
    runAuto(runID);
  }
}
