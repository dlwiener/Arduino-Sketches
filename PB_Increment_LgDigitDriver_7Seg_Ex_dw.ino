// dw Cycle Digits on Display
// dw Version  SparkFun Large Digit Driver (TPIC6C596 IC) https://www.sparkfun.com/products/13279
// Post #12 https://forum.arduino.cc/t/9-digit-numbers-and-words-on-7-segment-using-large-digit-driver-from-sparkfun/646721/11
// For use with Sparkfun LARGE DIGIT DRIVER BOARDS (may work with other shift registers but that's down to you!)
// Prepared by Dave Wilkie based on a method learnt by watching a video by Kristian Blåsol entitled: Using a 74HC595 to connect an Arduino to a 7-segment LED display - Anything Arduino episode 9
// https://www.youtube.com/watch?v=cHA-fwiA4Qo
// This sketch was written to work with Sparkfun large digit driver boards using the wiring method outlined in the Sparkfun Large Digit Driver Board hookup guide.
// I could not get my head round the method used in the Sparkfun example so hopefully this might be easier to understand for some people.

// Here is a lookup table to show how to display each individual digit:
// digit    corresponding decimal number
//                      0   // dw added
//   1                 96
//   2                 93
//   3                117
//   4                102
//   5                 55
//   6                 63
//   7                 97
//   8                127
//   9                103
//   0                123
//   DP               128  //dw added   https://learn.sunfounder.com/lesson-20-74hc595-2/
// Using the method used in this sketch/program Each segment has a numerical value as shown below in this diagram:

//                            1 (top segment value=1
//                          -----
//            Top left    2|     |64 top Right
//      Middle Horizontal   --4--
//    Bottom Left         8|     |32 Bottom Right segment
//     Bottom horizontal    --16-
//
// So referring to the above diagram  if we wanted to display a number 8 we would add together 1+2+4+64+8+16+32 = 127
// so this bit of code:
// shiftOut(segmentData, segmentClock, MSBFIRST, 127);
// will display a number 8
// to display a number 1 we want the right 2 segments with a combined value of 64+32=96
// therefore:
// shiftOut(segmentData, segmentClock, MSBFIRST, 96);
// Would display a number 1
// This is a simple sketch to count from 0 to 9, if using a single 7 segment unit the numbers will count up on the single digit, if additional digits are connected as per the spark fun guide then the numbers will scroll to the right as they count up
// This is due to the fact that as the latch pin is raised and lowered each large digit driver board will shunt the configuration that is on it out of the right handside to be stored by the next digit and so on down the line.
// If you have 9 digits (as I have) in order to display a nine digit number one would write the latch pin low ShiftOut 9 digits and then write the latch pin high again.
// Think of the data being written as a flow of water flowing left to right and the latch Pin acting as a dam to stop the flow at whatever point the data is at. Once data flows out of the furthest right digit it is lost/deleted/evaporates/goes over the waterfall!

byte segmentLatch = 5;  // Pin 5 goes to LAT on the IN side of the first Spark fun large digit driver board
byte segmentClock = 6;  // Pin 6 goes to CLK on the IN side of the first large digit driver board
byte segmentData = 7;   // Pin 7 goes to SER (as in Serial Data) on the IN side of the first large digit driver board
//int charArray[10] = {123, 96, 93, 117, 102, 55, 63, 97, 127, 103}; // These values are the codes required to display the digits 0 to 9 See lookup table in the commenst above.

// dw added the following char values
char myCharOFF = 0;
char myChar0 = 123;
char myChar1 = 96;
char myChar2 = 93;
char myChar3 = 117;
char myChar4 = 102;
char myChar5 = 55;
char myChar6 = 63;
char myChar7 = 97;
char myChar8 = 127;
char myChar9 = 103;
char myCharDP = 128;

const int btn = 11;
int selector = 0;
boolean isPressed = false;


void setup() {

  pinMode(btn, INPUT_PULLUP);  //dw no resistor required thanks to internal pull-up
  // dw Note:there is no internal pull-down so you would need 10K ohm when coding for pull-down
  pinMode(segmentLatch, OUTPUT);
  pinMode(segmentData, OUTPUT);
  pinMode(segmentClock, OUTPUT);

// The following turns 0 on when Arduino boots up.
  digitalWrite(segmentLatch, LOW);
  shiftOut(segmentData, segmentClock, MSBFIRST, myChar0);
  digitalWrite(segmentLatch, HIGH);
  delay(200);
  }


void loop() {

  if (digitalRead(btn) == LOW && isPressed == false)  //button is pressed AND this is the first digitalRead() that the button is pressed
  {
    isPressed = true;     //set to true, so this code will not run again until button released
    doSwitchStatement();  // a call to a separate function that performs the switch statement and subsequent evoked code

    selector++;  // this is done after the doSwitchStatement(), so case 0 will be executed on the first button press
    if (selector > 12) {
      selector = 0;
    }
    // selector = (selector+1) % 4;  // does the same, without if-statement
  } else if (digitalRead(btn) == HIGH) {
    isPressed = false;  //button is released, variable reset
  }
}

void doSwitchStatement() {
  switch (selector) {
    case 0:
      digitalWrite(segmentLatch, LOW);
      shiftOut(segmentData, segmentClock, MSBFIRST, myChar1);
      digitalWrite(segmentLatch, HIGH);
      delay(200);
      break;
    case 1:
      digitalWrite(segmentLatch, LOW);
      shiftOut(segmentData, segmentClock, MSBFIRST, myChar2);
      digitalWrite(segmentLatch, HIGH);
      delay(200);
      break;
    case 2:
      digitalWrite(segmentLatch, LOW);
      shiftOut(segmentData, segmentClock, MSBFIRST, myChar3);
      digitalWrite(segmentLatch, HIGH);
      delay(200);
      break;
    case 3:
      digitalWrite(segmentLatch, LOW);
      shiftOut(segmentData, segmentClock, MSBFIRST, myChar4);
      digitalWrite(segmentLatch, HIGH);
      delay(200);
      break;
    case 4:
      digitalWrite(segmentLatch, LOW);
      shiftOut(segmentData, segmentClock, MSBFIRST, myChar5);
      digitalWrite(segmentLatch, HIGH);
      delay(200);
      break;
    case 5:
      digitalWrite(segmentLatch, LOW);
      shiftOut(segmentData, segmentClock, MSBFIRST, myChar6);
      digitalWrite(segmentLatch, HIGH);
      delay(200);
      break;
    case 6:
      digitalWrite(segmentLatch, LOW);
      shiftOut(segmentData, segmentClock, MSBFIRST, myChar7);
      digitalWrite(segmentLatch, HIGH);
      delay(200);
      break;
    case 7:
      digitalWrite(segmentLatch, LOW);
      shiftOut(segmentData, segmentClock, MSBFIRST, myChar8);
      digitalWrite(segmentLatch, HIGH);
      delay(200);
      break;
    case 8:
      digitalWrite(segmentLatch, LOW);
      shiftOut(segmentData, segmentClock, MSBFIRST, myChar9);
      digitalWrite(segmentLatch, HIGH);
      delay(200);
      break;
    case 9:
      digitalWrite(segmentLatch, LOW);
      shiftOut(segmentData, segmentClock, MSBFIRST, myChar0);
      digitalWrite(segmentLatch, HIGH);
      delay(200);
      break;
    case 10:
      digitalWrite(segmentLatch, LOW);
      shiftOut(segmentData, segmentClock, MSBFIRST, myCharDP);
      digitalWrite(segmentLatch, HIGH);
      delay(200);
      break;
    case 11:
      digitalWrite(segmentLatch, LOW);
      shiftOut(segmentData, segmentClock, MSBFIRST, myCharOFF);
      digitalWrite(segmentLatch, HIGH);
      delay(200);
      break;
    case 12:
      digitalWrite(segmentLatch, LOW);
      shiftOut(segmentData, segmentClock, MSBFIRST, myChar0);
      digitalWrite(segmentLatch, HIGH);
      delay(200);
      break;

    default:
      digitalWrite(segmentLatch, LOW);
      shiftOut(segmentData, segmentClock, MSBFIRST, myChar0);
      digitalWrite(segmentLatch, HIGH);
      delay(200);
  }
}
