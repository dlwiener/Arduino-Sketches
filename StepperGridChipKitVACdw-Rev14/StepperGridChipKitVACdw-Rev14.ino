#include <AccelStepper.h>

/* 
  Version dw-Rev13
    
  pb Modification:
  Leds ON at Start instead of OFF at Start
  dw Modification:
  Each case runs the same sequence of patterns with different with 4 different starting points!
  dw Modification:
  Run Vacuum during homing
  Extra homing routine was added after the 4 cases, don't know why this was necessary but some cycles would not go home???

  pb four button interface, to choose instant start of the following:
  high speed, high amplitude run (Corner1; case 4)     Corner1 *-------* Corner2
  high speed, low amplitude run (Corner2; case 2)
  low speed, high amplitude run (Corner3; case 3)      Corner3 *-------* Corner4
  low speed, low amplitude run (Corner4; case 1)
  
*/

//Chipkit WF32 EXT Power Jumper for Wall Wart PS
//Chipkit WF32 UART Power Jumper for USB Power and Programming from Computer

//dw Resistors for Sound Levels, 1K(Loud,pin 37),  2.2K(Soft, pin 36)
//??dw newPos Values for ChipKit all have opposite +/- signs compared to Arduino???

#define NOTE_A3  220
#define NOTE_A4  440
#define NOTE_A5  880

//Velmex Slider 10 Threads per Inch
//Bodine Electric Stepper Motor 23T2BEWO; 1.7 Volts, 4.7 Amps, 100 oz-in, 200 steps/rev, NEMA 23
//http://agardunor.tripod.com/bodine.htm; 0.88mH Inductance
//Stepper Supply MaxVolts ~= SQRT( Inductance in mH ) * 32; therefore 30 Volts for optimum speed
//Old Rule: Stepper Supply Voltage for optimum speed = 20 to 25 times the motor's voltage rating; therefore 34 to 42.5 Volts
//Solid State Relay: Magnecraft 70S2-04-B-06-S

//set up the accelStepper instance
//AccelStepper stepper(1, motorStepPin, motorDirPin)
//the "1" tells it we are using a driver (TB6600 is the driver)
//Define 1, pulse pin 8, direction pin 9

AccelStepper stepper1(1, 8, 9);

//Vemex Motor, TB6600-4 Set at 1/4 microsteps.
//AccelStepper Speed is measured in steps/second

//Left Button is Routine Number; Right Button is Start
// Routine Number, Swing Distance Half Cycle = 2*Amplitude:
// 1 Fast   0.75_inches  Measured Period 2.0 sec; Freq 0.50 Hz
// 2 Fast   1.25_inches
// 3 Fast   2.00_inches
// 4 Medium 0.75_inches  Measured Period 3.5 sec; Freq 0.29 Hz
// 5 Medium 1.25_inches
// 6 Medium 2.00_inches
// 7 Slow   0.75_inches  Measured Period 5.0 sec; Freq 0.20 Hz
// 8 Slow   1.25_inches
// M Slow   2.00_inches

//ChipKit wiring to TB6600-4.0:
//ChipKit pin 9 to DIR+(+5V)
//ChipKit Ground to DIR-(DIR)
//ChipKit pin 8 to PUL+(+5V)
//ChipKit Ground to PUL-(PUL)

int homeSwitch = 12;    //Limit switch connected to pin 12
int lowHighButton = 13; //was pin number for routine number, now is low speed, high amplitude run button Corner2
int highLowButton = 11; //was start button; now high speed low amplitude run button, Corner3
int conveyorPin = 40;   //pin number for 120 Volt power to conveyor motor
int speakerPin = 10;
int vacuumPin = 34;
int highHighButton = 31; // pin number for high speed-high amplitude run button Corner4
int lowLowButton = 32;   // pin number for instant run low speed-low amplitude button Corner1

// variables will change:
int plaa = 0;         //Print Line Loop
int plab = 0;                       //Print Line Loop
int hBval;

boolean VacuumEnable = LOW;         // indicator if vacuum run needed
boolean Corner1_Flag = HIGH;        // low speed, low amplitude run
boolean Corner2_Flag = HIGH;        // low speed, high amplitude run
boolean Corner3_Flag = HIGH;        // high speed, low amplitude run
boolean Corner4_Flag = HIGH;        // high speed, high amplitude run

int OnTime;                         // Time for vacuum to be on
int i;                              //number of routine loops
int j;
int runID = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(homeSwitch, INPUT);        //Used 10K External pull-up resistor; INPUT_PULLUP command uses Internal pull-up resistor
  pinMode(lowHighButton, INPUT);     //Used 10K External pull-up resistor; INPUT_PULLUP command uses Internal pull-up resistor
  pinMode(highLowButton, INPUT);     //Used 10K External pull-up resistor; INPUT_PULLUP command uses Internal pull-up resistor
  pinMode(highHighButton, INPUT);    //Used 10K External pull-up resistor; INPUT_PULLUP command uses Internal pull-up resistor
  pinMode(lowLowButton, INPUT);
  pinMode(conveyorPin, OUTPUT);      //Output pin for 120 Volt power for conveyor motor
  pinMode(vacuumPin, OUTPUT);        //Output pin for 120 Volt power for conveyor motor
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(30, OUTPUT);
  digitalWrite(vacuumPin, LOW);
  digitalWrite(conveyorPin, LOW);
  clear_Leds_turn_off_sound();
  Serial.println("Motor Homing Next");
  motor_homing();
  turn_on_leds();
  Serial.println(" Finished setup routine.");
}


void motor_homing()
{ //this routine runs the motor to limit switch (Home)
  hBval = digitalRead(homeSwitch);
  stepper1.setMaxSpeed(10000);
  stepper1.setAcceleration(2000);
  while (hBval == 0)
  { // sets backwards slowly till it hits the switch and stops
    stepper1.moveTo(2000);
    stepper1.run();
    hBval = digitalRead(homeSwitch);
    Serial.print(hBval);
    Serial.println("  Home State, 0=Seeking Home, 1=Home");
    if (hBval == 1)
    { //set motor position to zero
      stepper1.setCurrentPosition(100);  //limit switch offset
    }
  }
}


void runMotor(int MaxSpeed, int Acceleration, int newPos, int j)
{
  delay(100);
  stepper1.setCurrentPosition(100);  //limit switch offset
  for ( int i = 0; i < j; i++ )      // int j is Number of cycles for each case
  {
    //set speed,acceleration and 2*amplitude of the pendulum
    stepper1.setMaxSpeed(MaxSpeed);
    stepper1.setAcceleration(Acceleration);
    stepper1.runToNewPosition(newPos);
    stepper1.stop();
    stepper1.runToNewPosition(0);
  }
}

/*  dw run vac during homing instead
void run_Vacuum(int OnTime)
{
  digitalWrite(vacuumPin, HIGH);   //start vacuum
  delay(OnTime);     //run vacuum for xx seconds, can change by changing OnTime parameter input
  digitalWrite(vacuumPin, LOW);    //stop vacuum
}
*/

void turn_on_leds()
{
  digitalWrite(7, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(28, HIGH);
  digitalWrite(30, HIGH);
}



void clear_Leds_turn_off_sound()
{
  digitalWrite(7, LOW);  // Turn off all indicator LEDs
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  digitalWrite(3, LOW);
  digitalWrite(2, LOW);
  digitalWrite(28, LOW);
  digitalWrite(29, LOW);
  digitalWrite(30, LOW);
  noTone(10);            // Turn off all tones
  noTone(36);
  noTone(37);
}

void runAuto(int runID)
{
  switch (runID)
  {
    case 1:  //Sequence starts with Corner 4 button
      // low speed-low amplitude run
      digitalWrite(conveyorPin, HIGH);
      digitalWrite(7, HIGH); // turn on low speed-low amplitude led indicator
      digitalWrite(5, LOW);  // turn off the rest
      digitalWrite(28, LOW);
      digitalWrite(30, LOW);
      tone(36, NOTE_A3);
      runMotor(482, 482, -750, 1);  //463 was Period 5.2 sec, low speed low amplitude (dw changed 4 to 2 to 1)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      // dw high speed-high amplitude run
      digitalWrite(conveyorPin, HIGH);  // Turn on conveyor belt
      digitalWrite(30, HIGH); // turn on high speed/high amplitude led indicator
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(28, LOW);
      tone(37, NOTE_A5);
      runMotor(8163, 8163, -2000, 2); // high speed-high amplitude profile  (dw changed 11 to 6 to 2)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      // dw high speed-low amplitude run 
      digitalWrite(conveyorPin, HIGH);
      digitalWrite(5, HIGH);        // turn on low speed-high amplitude led indicator
      digitalWrite(7, LOW);
      digitalWrite(28, LOW);
      digitalWrite(30, LOW);
      tone(36, NOTE_A5);
      runMotor(3061, 3061, -750, 2);   // low speed, high amplitude run  (dw changed 9 to 5 to 2)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      // dw low speed-high amplitude run  
      digitalWrite(conveyorPin, HIGH);
      digitalWrite(28, HIGH); // turn on high speed-low amplitude led indicator
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(30, LOW);
      tone(37, NOTE_A3);
      runMotor(1284, 1284, -2000, 1);  // high speed low amplitude run  (dw changed 4 to 2 to 1)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      digitalWrite(vacuumPin, HIGH);   //start vacuum
      motor_homing();  // homes while vacuum is on
      delay(1000);  //extra delay for vacuum if needed
      digitalWrite(vacuumPin, LOW);  //stop vacuum
      runID = 0; // reset run ID for next run choice
      Corner1_Flag = HIGH;
      turn_on_leds();
      break;

    case 2:  //Sequence starts with Corner 2 button
      // dw high speed-low amplitude run
      digitalWrite(conveyorPin, HIGH);
      digitalWrite(5, HIGH);        // turn on low speed-high amplitude led indicator
      digitalWrite(7, LOW);
      digitalWrite(28, LOW);
      digitalWrite(30, LOW);
      tone(36, NOTE_A5);
      runMotor(3061, 3061, -750, 2);   // low speed, high amplitude run  (dw changed 9 to 5 to 2)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      // dw low speed-high amplitude run  
      digitalWrite(conveyorPin, HIGH);
      digitalWrite(28, HIGH); // turn on high speed-low amplitude led indicator
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(30, LOW);
      tone(37, NOTE_A3);
      runMotor(1284, 1284, -2000, 1);  // high speed low amplitude run  (dw changed 4 to 2 to 1)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      // dw low speed-low amplitude run
      digitalWrite(conveyorPin, HIGH);
      digitalWrite(7, HIGH); // turn on low speed-low amplitude led indicator
      digitalWrite(5, LOW);  // turn off the rest
      digitalWrite(28, LOW);
      digitalWrite(30, LOW);
      tone(36, NOTE_A3);
      runMotor(482, 482, -750, 1);  //463 was Period 5.2 sec, low speed low amplitude (dw changed 4 to 2 to 1)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      // dw high speed-high amplitude run
      digitalWrite(conveyorPin, HIGH);  // Turn on conveyor belt
      digitalWrite(30, HIGH); // turn on high speed/high amplitude led indicator
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(28, LOW);
      tone(37, NOTE_A5);
      runMotor(8163, 8163, -2000, 2); // high speed-high amplitude profile  (dw changed 11 to 6 to 2)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      digitalWrite(vacuumPin, HIGH);   //start vacuum
      motor_homing();  // homes while vacuum is on
      delay(1000);  //extra delay for vacuum if needed
      digitalWrite(vacuumPin, LOW);  //stop vacuum
      runID = 0; // reset run ID for next run choice
      Corner2_Flag = HIGH;
      turn_on_leds();
      break;

    case 3:  //Sequence starts with Corner 3 button
      // low speed-high amplitude run
      digitalWrite(conveyorPin, HIGH);
      digitalWrite(28, HIGH); // turn on high speed-low amplitude led indicator
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(30, LOW);
      tone(37, NOTE_A3);
      runMotor(1284, 1284, -2000, 1);  // high speed low amplitude run  (dw changed 4 to 2 to 1)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      // dw low speed-low amplitude run
      digitalWrite(conveyorPin, HIGH);
      digitalWrite(7, HIGH); // turn on low speed-low amplitude led indicator
      digitalWrite(5, LOW);  // turn off the rest
      digitalWrite(28, LOW);
      digitalWrite(30, LOW);
      tone(36, NOTE_A3);
      runMotor(482, 482, -750, 1);  //463 was Period 5.2 sec, low speed low amplitude (dw changed 4 to 2 to 1)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      // dw high speed-high amplitude run
      digitalWrite(conveyorPin, HIGH);  // Turn on conveyor belt
      digitalWrite(30, HIGH); // turn on high speed/high amplitude led indicator
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(28, LOW);
      tone(37, NOTE_A5);
      runMotor(8163, 8163, -2000, 2); // high speed-high amplitude profile  (dw changed 11 to 6 to 2)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      // dw high speed-low amplitude run 
      digitalWrite(conveyorPin, HIGH);
      digitalWrite(5, HIGH);        // turn on low speed-high amplitude led indicator
      digitalWrite(7, LOW);
      digitalWrite(28, LOW);
      digitalWrite(30, LOW);
      tone(36, NOTE_A5);
      runMotor(3061, 3061, -750, 2);   // low speed, high amplitude run  (dw changed 9 to 5 to 2)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      digitalWrite(vacuumPin, HIGH);   //start vacuum
      motor_homing();  // homes while vacuum is on
      delay(1000);  //extra delay for vacuum if needed
      digitalWrite(vacuumPin, LOW);  //stop vacuum
      runID = 0; // reset run ID for next run choice
      Corner3_Flag = HIGH;
      turn_on_leds();
      break;

    case 4:  //Sequence starts with Corner 1 button
      // dw high speed-high amplitude run
      digitalWrite(conveyorPin, HIGH);  // Turn on conveyor belt
      digitalWrite(30, HIGH); // turn on high speed/high amplitude led indicator
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(28, LOW);
      tone(37, NOTE_A5);
      runMotor(8163, 8163, -2000, 2); // high speed-high amplitude profile  (dw changed 11 to 6 to 2)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      // dw high speed-low amplitude run 

      digitalWrite(conveyorPin, HIGH);
      digitalWrite(5, HIGH);        // turn on low speed-high amplitude led indicator
      digitalWrite(7, LOW);
      digitalWrite(28, LOW);
      digitalWrite(30, LOW);
      tone(36, NOTE_A5);
      runMotor(3061, 3061, -750, 2);   // low speed, high amplitude run  (dw changed 9 to 5 to 2)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      // dw low speed-high amplitude run  

      digitalWrite(conveyorPin, HIGH);
      digitalWrite(28, HIGH); // turn on high speed-low amplitude led indicator
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(30, LOW);
      tone(37, NOTE_A3);
      runMotor(1284, 1284, -2000, 1);  // high speed low amplitude run  (dw changed 4 to 2 to 1)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      // dw low speed-low amplitude run
      digitalWrite(conveyorPin, HIGH);
      digitalWrite(7, HIGH); // turn on low speed-low amplitude led indicator
      digitalWrite(5, LOW);  // turn off the rest
      digitalWrite(28, LOW);
      digitalWrite(30, LOW);
      tone(36, NOTE_A3);
      runMotor(482, 482, -750, 1);  //463 was Period 5.2 sec, low speed low amplitude (dw changed 4 to 2 to 1)
      digitalWrite(conveyorPin, LOW);
      clear_Leds_turn_off_sound();
      delay(10);

      digitalWrite(vacuumPin, HIGH);   //start vacuum
      motor_homing();  // homes while vacuum is on
      delay(1000);  //extra delay for vacuum if needed
      digitalWrite(vacuumPin, LOW);  //stop vacuum
      runID = 0; // reset run ID for next run choice
      Corner4_Flag = HIGH;
      turn_on_leds();
      break;

    default:
      {
      }
  }
  motor_homing();   //dw not sure why this extra homing routine was required but sometimes cycles would not go home???
  runID = 0; // reset run ID for next run choice
  Serial.println("Finished Auto run");
}

void loop()
{
  runID = 0;
  Corner1_Flag = digitalRead(lowLowButton);    // initially HIGH until button pushed
  Corner2_Flag = digitalRead(lowHighButton);   //              "
  Corner3_Flag = digitalRead(highLowButton);   //              "
  Corner4_Flag = digitalRead(highHighButton);  //              "
  if (Corner1_Flag == LOW && Corner2_Flag == HIGH && Corner3_Flag == HIGH && Corner4_Flag == HIGH)
  {
    runID = 1;
    Serial.println(" Low Low run1.");
  }
  if (Corner1_Flag == HIGH && Corner2_Flag == LOW && Corner3_Flag == HIGH && Corner4_Flag == HIGH)
  {
    runID = 2;
    Serial.println(" High Low run2.");
  }
  if (Corner1_Flag == HIGH && Corner2_Flag == HIGH && Corner3_Flag == LOW && Corner4_Flag == HIGH)
  {
    runID = 3;
    Serial.println(" Low High run3.");
  }
  if (Corner1_Flag == HIGH && Corner2_Flag == HIGH && Corner3_Flag == HIGH && Corner4_Flag == LOW)
  {
    runID = 4;
    Serial.println("High High run4.");
  }
  Serial.print("runID = ");
  Serial.println(runID);
  if (runID != 0)
  {
    Serial.println("Auto run started.");
    runAuto(runID);
  }
}
