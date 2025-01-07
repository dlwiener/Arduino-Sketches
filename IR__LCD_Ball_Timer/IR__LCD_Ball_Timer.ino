//Adafruit IR Break Beam Sensors 3mm LEDs (2 pairs)
//dw Time between gates in Seconds and Speed in mi/hr
// If d = 25cm then max speed = 56 mi/hr
// If d = 50cm then max speed = 112 mi/hr
// 50cm is about 20 inches
// LCD 1602 with I2C Module
// LCD GRD to Uno Grd Green;  short tube version Black
// LCD VCC to Uno 5V Red; short tube version Red
// LCD SDA to Uno Pin A4 Yellow; short tube version Orange
// LCD SCL to Uno Pin A5 Blue; short tube version Brown



#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);


#define ENDPIN 4    //dw white;  short tube version Blue
#define STARTPIN 7  //dw Yellow; short tube version White
int endState = 0;
int startState = 0;
int timePassed = 0;
int timerStarted = false;

void setup() {

  // initialize the LCD,
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();

  //initialize the end pin as an input:
  pinMode(ENDPIN, INPUT);
  digitalWrite(ENDPIN, HIGH); // turn on the pullup
  // initialize the start pin as an input:
  pinMode(STARTPIN, INPUT);
  digitalWrite(STARTPIN, HIGH); // turn on the pullup
}

void loop() {
  // read the state of the pushbutton value:
  endState = digitalRead(ENDPIN);
  startState = digitalRead(STARTPIN);

  if (!endState && timerStarted) {

    float actualSeconds = timePassed;

    lcd.clear();
    lcd.print(timePassed);
    lcd.print(" = msec");
    lcd.setCursor (0, 1); // go to start of 2nd line
    // dw note: 1 cm/sec = .0223694 mi/hr
    // short tube version 24.5 cm between gates     
    lcd.print((24.5 * .0223694) / (actualSeconds / 1000));   
    lcd.print(" = mi/hr");
    //lcd.print(millis() / 1000);
    delay(500);

    timerStarted = false;
  }

  if (!startState && !timerStarted) {
    //Serial.println("START!");
    timePassed = 0;
    timerStarted = true;
  }

  if (timerStarted) {
    timePassed += 1;
    delay(1);
  }
}
