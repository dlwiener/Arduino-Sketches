#include <Stepper.h>

// Number of steps per revolution of INTERNAL motor in 4-step mode
#define STEPS_PER_MOTOR_REVOLUTION 32   

// Number of steps per revolution of OUTPUT SHAFT (= gear reduction; 2048 steps)
#define STEPS_PER_OUTPUT_REVOLUTION 32 * 64

// Declare 'small_stepper' variable
Stepper small_stepper(STEPS_PER_MOTOR_REVOLUTION, 8, 10, 9, 11);

// Declare 'Steps2Take' variable
int  Steps2Take;

void setup()
{
// Nothing  (Stepper Library sets pins as outputs)
}

void loop()
{
  small_stepper.setSpeed(5);   // SLOWLY Show the 4 step sequence 
  Steps2Take  =  20;  // Rotate CW
  small_stepper.step(Steps2Take);
  delay(2000);

  Steps2Take  =  STEPS_PER_OUTPUT_REVOLUTION / 2;  // Rotate CW 1/2 turn
  small_stepper.setSpeed(500);   
  small_stepper.step(Steps2Take);
  delay(1000);
  
  Steps2Take  =  - STEPS_PER_OUTPUT_REVOLUTION / 2;  // Rotate CCW 1/2 turn  
  small_stepper.setSpeed(1000);  // 2000 a good max speed??
  small_stepper.step(Steps2Take);
  delay(2000);
}
