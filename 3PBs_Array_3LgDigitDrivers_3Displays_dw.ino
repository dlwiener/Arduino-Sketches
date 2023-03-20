// dw PB Array Increment Three Digits to Display  000 to 999
// dw % Remainder Operation; Up Button; RESET Button; DOWN Button
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

// The method used in this sketch is based on the numerical value of each segment listed in the diagram below:

//                            1 (top segment value=1
//                          -----
//            Top left    2|     |64 top Right
//      Middle Horizontal   --4--
//    Bottom Left         8|     |32 Bottom Right segment
//     Bottom horizontal    --16-
//
// So referring to the above diagram  if we wanted to display a number 8 we would add together 1+2+4+64+8+16+32 = 127
// so the following bit of code:
// shiftOut(segmentData, segmentClock, MSBFIRST, 127);
// will display a number 8
// to display a number 1 we want the right 2 segments with a combined value of 64+32=96
// therefore:
// shiftOut(segmentData, segmentClock, MSBFIRST, 96);
// Would display a number 1
// The original sketch counted from 0 to 9 on one display.  For a single 7 segment display the numbers count up on the single digit, if additional digits are connected as per the SparkFun guide then the numbers will scroll to the right as they count up
// This is due to the fact that as the latch pin is raised and lowered each large digit driver board will shunt the configuration that is on it out of the right handside to be stored by the next digit and so on down the line.
// If you have 9 digits (as I have) in order to display a nine digit number one would write the latch pin low ShiftOut 9 digits and then write the latch pin high again.
// Think of the data being written as a flow of water flowing left to right and the latch Pin acting as a dam to stop the flow at whatever point the data is at. Once data flows out of the furthest right digit it is lost/deleted/evaporates/goes over the waterfall!

// ones digit
byte segmentLatch = 5;  // Pin 5 goes to LAT on the IN side of the first Spark fun large digit driver board
byte segmentClock = 6;  // Pin 6 goes to CLK on the IN side of the first large digit driver board
byte segmentData = 7;   // Pin 7 goes to SER (as in Serial Data) on the IN side of the first large digit driver board

// tens digit
byte segmentLatch2 = 2;
byte segmentClock2 = 3;
byte segmentData2 = 4;

// hundreds digit
byte segmentLatch3 = 8;
byte segmentClock3 = 9;
byte segmentData3 = 10;


const int btnUP = 13;  // increase increment button
boolean isPressed = false;

const int btnRESET = 12;  // reset button
boolean isPressed2 = false;

const int btnDOWN = 11;  // decrease increment button
boolean isPressed3 = false;



// n is an array of 13 integers
int driver[11] = { 123, 96, 93, 117, 102, 55, 63, 97, 127, 103, 123 };
int driver2[11] = { 123, 96, 93, 117, 102, 55, 63, 97, 127, 103, 123 };
int driver3[11] = { 123, 96, 93, 117, 102, 55, 63, 97, 127, 103, 123 };


int i = 0;
int j = 0;
int k = 0;
int m = 0;  // three display digits
// int n = 0;  four display digits

void setup() {

  pinMode(btnUP, INPUT_PULLUP);  //dw no resistor required thanks to internal pull-up
  // dw Note:there is no internal pull-down so you would need 10K ohm when coding for pull-down
  pinMode(btnRESET, INPUT_PULLUP);
  pinMode(btnDOWN, INPUT_PULLUP);

  // ones digit
  pinMode(segmentLatch, OUTPUT);
  pinMode(segmentData, OUTPUT);
  pinMode(segmentClock, OUTPUT);

  // tens digit
  pinMode(segmentLatch2, OUTPUT);
  pinMode(segmentData2, OUTPUT);
  pinMode(segmentClock2, OUTPUT);

  // hundreds digit
  pinMode(segmentLatch3, OUTPUT);
  pinMode(segmentData3, OUTPUT);
  pinMode(segmentClock3, OUTPUT);


  // The following turns 00 on when Arduino boots up.
  // ones digit
  digitalWrite(segmentLatch, LOW);
  shiftOut(segmentData, segmentClock, MSBFIRST, 123);
  digitalWrite(segmentLatch, HIGH);
  delay(200);

  // tens digit
  digitalWrite(segmentLatch2, LOW);
  shiftOut(segmentData2, segmentClock2, MSBFIRST, 123);
  digitalWrite(segmentLatch2, HIGH);
  delay(200);

  // hundreds digit
  digitalWrite(segmentLatch3, LOW);
  shiftOut(segmentData3, segmentClock3, MSBFIRST, 123);
  digitalWrite(segmentLatch3, HIGH);
  delay(200);
}


void loop() {

  if (digitalRead(btnRESET) == LOW && isPressed2 == false)  //button is pressed AND this is the first digitalRead() that the button is pressed
  {
    isPressed2 = true;  //set to true, so this code will not run again until button released

    // ones digit
    digitalWrite(segmentLatch, LOW);
    shiftOut(segmentData, segmentClock, MSBFIRST, 123);
    digitalWrite(segmentLatch, HIGH);
    delay(200);

    // tens digit
    digitalWrite(segmentLatch2, LOW);
    shiftOut(segmentData2, segmentClock2, MSBFIRST, 123);
    digitalWrite(segmentLatch2, HIGH);
    delay(200);

    // tens digit
    digitalWrite(segmentLatch3, LOW);
    shiftOut(segmentData3, segmentClock3, MSBFIRST, 123);
    digitalWrite(segmentLatch3, HIGH);
    delay(200);

    i = 0;
    j = 0;
    k = 0;
    m = 0;

  } else if (digitalRead(btnRESET) == HIGH) {
    isPressed2 = false;  //button is released, variable reset
  }


  if (digitalRead(btnUP) == LOW && isPressed == false)  //button is pressed AND this is the first digitalRead() that the button is pressed
  {
    isPressed = true;  //set to true, so this code will not run again until button released

    k = k + 1;
    i = k % 10;  // gets ones value; % is a Remainder Operation; Ex. 98 / 10 = 9 with Remainder of integer 8
    j = (k / 10) % 10;  // gets tens value; Ex. (76 / 10) / 10 = 0 with Remainder of integer 7
    m = (k / 100) % 10;  // gets hundreds value
    // int n = (k/1000) % 10;  // gets thousands value


    if (k > 999) {  //  limits k to 99 for two digit display; should be 999 for three digit display
      i = 0;
      j = 0;
      k = 0;
      m = 0;
    }


    do7SegStatement();  // a call to a separate function that performs the statement and subsequent evoked code


  } else if (digitalRead(btnUP) == HIGH) {
    isPressed = false;  //button is released, variable reset
  }


  if (digitalRead(btnDOWN) == LOW && isPressed == false)  //button is pressed AND this is the first digitalRead() that the button is pressed
  {
    isPressed = true;  //set to true, so this code will not run again until button released


    k = k - 1;
    i = k % 10;          // gets ones value; % is a Remainder Operation; Ex. 98 / 10 = 9 with Remainder of integer 8
    j = (k / 10) % 10;   // gets tens value; Ex. (76 / 10) / 10 = 0 with Remainder of integer 7
    m = (k / 100) % 10;  // gets hundreds value
    // int n = (k/1000) % 10;  // gets thousands value


    // the following cycles back to count down from 999
    if (k < 0) {  //  limits k to 00 for two digit display; should be 000 for three digit display
      i = 9;
      j = 9;
      k = 999;
      m = 9;


      /*
    // Instead of above the following cycles back to count down from 999
    if (k < 0) {  //  limits k to 00 for two digit display; 000 for three digit display
      i = 0;
      j = 0;
      k = 0;
      m = 0;
*/
    }



    do7SegStatement();  // a call to a separate function that performs the statement and subsequent evoked code


  } else if (digitalRead(btnDOWN) == HIGH) {
    isPressed = false;  //button is released, variable reset
  }
}

void do7SegStatement() {

  // ones digit
  digitalWrite(segmentLatch, LOW);
  shiftOut(segmentData, segmentClock, MSBFIRST, driver[i]);
  digitalWrite(segmentLatch, HIGH);
  delay(200);
  //delay(100);  // wait between button presses

  // tens digit
  digitalWrite(segmentLatch2, LOW);
  shiftOut(segmentData2, segmentClock2, MSBFIRST, driver2[j]);
  digitalWrite(segmentLatch2, HIGH);
  delay(200);
  //delay(100);  // wait between button presses

  // hundreds digit
  digitalWrite(segmentLatch3, LOW);
  shiftOut(segmentData3, segmentClock3, MSBFIRST, driver3[m]);
  digitalWrite(segmentLatch3, HIGH);
  delay(200);
  //delay(100);  // wait between button presses
}
