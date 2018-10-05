
//Wiring:
//Adafruit 14-Segment Alphanumeric LED FeatherWing(Fwing)
//Fwing Pin Numbers Looking Down from above the Display:
//  x  x  x  x 24 23 22   21 20 19 18 17 16 15
//
//  1  2  3  4  5  6  7    8  9 10 11 12 13 14
//Arduino Pin 3.3 V   to Fwing Pin 2
//Arduino Pin GRD     to Fwing Pin 4
//Arduino Pin A4(SDA) to Fwing Pin 15; I2C(SDA)
//Arduino Pin A5(SCL) to Fwing Pin 16; I2C(SCL)

#include <toneAC.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

//Adafruit_7segment matrix = Adafruit_7segment();  Sort of works but G2 segment
//of the Alpha display does not light up and the numbers look funny.
//Clever trick: Use Arduino's String Constructor function ( String() )
//to convert the integer variable into a character string, which is then parsed
//https://forums.adafruit.com/viewtopic.php?f=25&t=78880#p399930
//See 4 uses in the code below!

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

int buttonPin = 13;     //pin number for routine number (Yellow Wires)
int onOffPin = 11;      //pin number for start button  (Blue Wires)


int g = 98;   //G2  97.9989
int a = 110;  //A2 110.0000
int b = 123;  //B2 123.4708
int c = 131;  //C3 130.8128
int d = 147;  //D3 146.8324
int e = 165;  //E3 164.8138
int f = 175;  //F3 174.6141
int g3 = 196; //G3 195.9977
int a3 = 220; //A3 220.0000


// variables will change:
int plaa = 0;         //Print Line Loop
int plab = 0;         //Print Line Loop
boolean onOffState = HIGH;
boolean LASTonOffState = HIGH;
int buttonPushCounter = 0;      // counter for number of button presses for routine
int buttonState = HIGH;         // current state of the button
int lastButtonState = HIGH;     // previous state of the button




void setup() {

  pinMode(buttonPin, INPUT_PULLUP);         // INPUT_PULLUP command uses Internal pull-up resitor
  pinMode(onOffPin, INPUT_PULLUP);          // INPUT_PULLUP command uses Internal pull-up resitor

  //Pin 10 Speaker REQUIRED PIN toneAC()Library!
  pinMode(2, OUTPUT);  //G2;g  LED Wire: Org/Blk
  pinMode(3, OUTPUT);  //A2;a  LED Wire: Red/Grn
  pinMode(4, OUTPUT);  //B2;b  LED Wire: Red/Blk
  pinMode(5, OUTPUT);  //C3;c  LED Wire: Blu/Blk
  pinMode(6, OUTPUT);  //D3;d  LED Wire: Red/Wht
  pinMode(7, OUTPUT);  //E3;e  LED Wire: Grn/Blk
  pinMode(8, OUTPUT);  //F3;f  LED Wire: Wht/Blk
  pinMode(A0, OUTPUT);  //G3;g3 LED Wire: Brn/Blk
  pinMode(12, OUTPUT); //A3;a3 LED Wire: Yel/Blk

  Serial.begin(9600);
  //matrix.begin(0x70);  // pass in the address
  alpha4.begin(0x70);  // pass in the address

}

// Code template for each of the cases 1 through 9:
void runPlayer(int freq, int Volume, int PIN, int STATE, int HOLD, int rest)
{
  delay (200);

  //toneAC (frequency, Volume);  volume range 0 to 10
  toneAC(freq, Volume);
  digitalWrite(PIN, STATE);
  delay (HOLD);

  toneAC(0);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(A0, LOW);
  digitalWrite(12, LOW);
  if (rest == 0) {
    delay(150);         // short rest between notes
  } else {
    delay(300);         // long rest between notes
  }
}

void loop() {

  if (plaa == 0) {
    Serial.println();
    Serial.println("Rimba Player");
    //Serial.println();
    plaa = 1;
  }


  if (buttonPushCounter >= 0 && buttonPushCounter <= 10) {
    //To overcome Adafruit_7segment matrix problem  where the G2 segment of Alpha display
    //does not light up and the numbers look funny...
    //Clever trick: Use Arduino's String Constructor function ( String() )
    //to convert the integer variable into a character string, which is then parsed
    //https://forums.adafruit.com/viewtopic.php?f=25&t=78880#p399930
    String buttonPushCounterDisp = String (buttonPushCounter);
    alpha4.writeDigitAscii(3, buttonPushCounterDisp[0]);
    alpha4.writeDisplay();
    //matrix.writeDigitNum(3, buttonPushCounter);
    //matrix.writeDisplay();
  }
  //plaa "if" prints line only once
  if (plaa == 0) {
    Serial.println();
    Serial.println("Rimba Player");
    //Serial.println();
    plaa = 1;
  }

  {
    buttonState = digitalRead(buttonPin);
    delay (100);  //dw was 400

    // compare the buttonState to its previous state
    if (buttonState != lastButtonState) {
      // if the state has changed, increment the counter
      if (buttonState != HIGH) {
        // if the current state is HIGH then the button
        // went from off to on:
        buttonPushCounter++;
        //Serial.println("on");
        Serial.print("Number of Routine button presses:  ");
        Serial.println(buttonPushCounter);
        if (buttonPushCounter == 1) {
          Serial.println("Mary Had a little Lamb");
        }
        if (buttonPushCounter == 2) {
          Serial.println("Twinkle, Twinkle");
        }
        if (buttonPushCounter == 3) {
          Serial.println("This Old Man");
        }
        if (buttonPushCounter == 4) {
          Serial.println("Jingle Bells");
        }
        if (buttonPushCounter == 5) {
          Serial.println("Erie Canal");
        }
        if (buttonPushCounter == 6) {
          Serial.println("We Wish You a Merry Christmas");
        }
        if (buttonPushCounter == 7) {
          Serial.println("O Christmas Tree");
        }
        if (buttonPushCounter == 8) {
          Serial.println("Fre-re Jacques");
        }
        if (buttonPushCounter == 9) {
          Serial.println("Ode to Joy");
        }
        if (buttonPushCounter >= 10) {
          Serial.println("Press Start to Reset Routine Number Counter < 10");
          buttonPushCounter = 0;
          if (buttonPushCounter >= 0 && buttonPushCounter <= 10) {
            String buttonPushCounterDisp = String (buttonPushCounter);
            alpha4.writeDigitAscii(3, buttonPushCounterDisp[0]);
            alpha4.writeDisplay();
            //matrix.writeDigitNum(3, buttonPushCounter);
            //matrix.writeDisplay();
          }


        }

        if (buttonPushCounter >= 0 && buttonPushCounter <= 10) {
          String buttonPushCounterDisp = String (buttonPushCounter);
          alpha4.writeDigitAscii(3, buttonPushCounterDisp[0]);
          alpha4.writeDisplay();
          //matrix.writeDigitNum(3, buttonPushCounter);
          //matrix.writeDisplay();
        }

      } else {
        // if the current state is LOW then the button
        // went from on to off:
        Serial.println("off");
      }
      // Delay a little bit to avoid bouncing
      delay(100);  //dw was 250
    }

    //plab "if" prints line only once
    if (plab == 0) {
      Serial.println();
      Serial.println("Press Left button for Routine Number (Yellow Wires)");
      Serial.println("Press right button to start routine after Routine Number is entered (Blue Wires)");
      plab = 1;
    }
    onOffState = digitalRead(onOffPin);
    if (onOffState == HIGH && LASTonOffState == LOW)
    {
      Serial.println("pressed");
      Serial.println(onOffState);
      delay(20);
      //toggle the onOffState state
      if (onOffState == HIGH) {
        digitalWrite(onOffState, LOW);

      } else {
        digitalWrite(onOffState, HIGH);

      }

    }
    if (onOffState != HIGH) {
      switch (buttonPushCounter) {
        //see above template: void runPlayer(int freq, int Volume, int PIN, int STATE, int HOLD, int rest)
        case 1:

          { char *message = "  MARY HAD A LITTLE LAMB----E3----IS---FIRST---E3     ";

            alpha4.writeDigitRaw(3, 0x0);
            alpha4.writeDigitRaw(0, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(0, 0x0);
            alpha4.writeDigitRaw(1, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(1, 0x0);
            alpha4.writeDigitRaw(2, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(2, 0x0);
            alpha4.writeDigitRaw(3, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);


            // send a message!
            for (uint8_t i = 0; i < strlen(message) - 4; i++) {
              alpha4.writeDigitAscii(0, message[i]);
              alpha4.writeDigitAscii(1, message[i + 1]);
              alpha4.writeDigitAscii(2, message[i + 2]);
              alpha4.writeDigitAscii(3, message[i + 3]);
              alpha4.writeDisplay();

              delay(200);
            }
          }

          //Mary Had a Little Lamb
          //delay(3000);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          delay (200);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);

          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          delay (200);
          break;

        case 2:

          { char *message = "  TWINKLE TWINKLE----C3----IS---FIRST---C3     ";

            alpha4.writeDigitRaw(3, 0x0);
            alpha4.writeDigitRaw(0, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(0, 0x0);
            alpha4.writeDigitRaw(1, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(1, 0x0);
            alpha4.writeDigitRaw(2, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(2, 0x0);
            alpha4.writeDigitRaw(3, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);


            // send a message!
            for (uint8_t i = 0; i < strlen(message) - 4; i++) {
              alpha4.writeDigitAscii(0, message[i]);
              alpha4.writeDigitAscii(1, message[i + 1]);
              alpha4.writeDigitAscii(2, message[i + 2]);
              alpha4.writeDigitAscii(3, message[i + 3]);
              alpha4.writeDisplay();

              delay(200);
            }
          }


          //Twinkle Twinkle
          //delay(3000);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          delay(300);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          delay(300);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          delay(300);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          delay(300);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          delay(300);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          delay (200);
          break;

        case 3:

          { char *message = "  THIS OLD MAN----G3----IS---FIRST---G3     ";

            alpha4.writeDigitRaw(3, 0x0);
            alpha4.writeDigitRaw(0, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(0, 0x0);
            alpha4.writeDigitRaw(1, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(1, 0x0);
            alpha4.writeDigitRaw(2, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(2, 0x0);
            alpha4.writeDigitRaw(3, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);


            // send a message!
            for (uint8_t i = 0; i < strlen(message) - 4; i++) {
              alpha4.writeDigitAscii(0, message[i]);
              alpha4.writeDigitAscii(1, message[i + 1]);
              alpha4.writeDigitAscii(2, message[i + 2]);
              alpha4.writeDigitAscii(3, message[i + 3]);
              alpha4.writeDisplay();

              delay(200);
            }
          }

          //This Old Man
          //delay(3000);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 1200, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 1200, 1);
          delay (200);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 0);
          runPlayer(f, 1, 8, HIGH, 300, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 0);
          runPlayer(c, 1, 5, HIGH, 300, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 0);
          runPlayer(d, 1, 6, HIGH, 600, 0);
          runPlayer(e, 1, 7, HIGH, 600, 0);
          runPlayer(f, 1, 8, HIGH, 300, 1);
          runPlayer(g3, 1, A0, HIGH, 1200, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 1200, 1);
          delay (400);
          break;

        case 4:

          { char *message = "  JINGLE BELLS----E3----IS---FIRST---E3     ";

            alpha4.writeDigitRaw(3, 0x0);
            alpha4.writeDigitRaw(0, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(0, 0x0);
            alpha4.writeDigitRaw(1, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(1, 0x0);
            alpha4.writeDigitRaw(2, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(2, 0x0);
            alpha4.writeDigitRaw(3, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);


            // send a message!
            for (uint8_t i = 0; i < strlen(message) - 4; i++) {
              alpha4.writeDigitAscii(0, message[i]);
              alpha4.writeDigitAscii(1, message[i + 1]);
              alpha4.writeDigitAscii(2, message[i + 2]);
              alpha4.writeDigitAscii(3, message[i + 3]);
              alpha4.writeDisplay();

              delay(200);
            }
          }

          //Jingle Bells
          //delay(3000);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 1200, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 1200, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          delay(300);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 1200, 1);
          runPlayer(g3, 1, A0, HIGH, 1200, 1);
          delay(300);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 1200, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 1200, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          delay(300);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 1200, 1);
          runPlayer(c, 1, 5, HIGH, 1200, 1);
          delay (400);
          break;

        case 5:

          { char *message = "  ERIE CANAL----A3---IS---FIRST---A3     ";

            alpha4.writeDigitRaw(3, 0x0);
            alpha4.writeDigitRaw(0, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(0, 0x0);
            alpha4.writeDigitRaw(1, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(1, 0x0);
            alpha4.writeDigitRaw(2, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(2, 0x0);
            alpha4.writeDigitRaw(3, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);


            // send a message!
            for (uint8_t i = 0; i < strlen(message) - 4; i++) {
              alpha4.writeDigitAscii(0, message[i]);
              alpha4.writeDigitAscii(1, message[i + 1]);
              alpha4.writeDigitAscii(2, message[i + 2]);
              alpha4.writeDigitAscii(3, message[i + 3]);
              alpha4.writeDisplay();

              delay(200);
            }
          }

          //Erie Canal
          //delay(3000);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 1200, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 0);
          runPlayer(e, 1, 7, HIGH, 300, 1);

          runPlayer(f, 1, 8, HIGH, 600, 0);
          runPlayer(d, 1, 6, HIGH, 300, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(a, 1, 3, HIGH, 600, 0);
          runPlayer(a, 1, 3, HIGH, 300, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 0);
          runPlayer(f, 1, 8, HIGH, 600, 0);
          runPlayer(f, 1, 8, HIGH, 600, 0);
          runPlayer(f, 1, 8, HIGH, 300, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 1200, 1);
          delay(300);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 0);
          runPlayer(e, 1, 7, HIGH, 600, 0);
          runPlayer(f, 1, 8, HIGH, 600, 0);
          runPlayer(d, 1, 6, HIGH, 300, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);

          delay (400);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 1200, 1);

          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);

          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 0);
          runPlayer(g3, 1, A0, HIGH, 300, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);

          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 200, 0);
          runPlayer(e, 1, 7, HIGH, 200, 0);
          runPlayer(f, 1, 8, HIGH, 200, 0);
          runPlayer(d, 1, 6, HIGH, 200, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 1200, 1);
          delay (400);
          break;

        case 6:

          { char *message = "  WE WISH YOU A MERRY CHRISTMAS----G2----IS---FIRST---G2     ";

            alpha4.writeDigitRaw(3, 0x0);
            alpha4.writeDigitRaw(0, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(0, 0x0);
            alpha4.writeDigitRaw(1, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(1, 0x0);
            alpha4.writeDigitRaw(2, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(2, 0x0);
            alpha4.writeDigitRaw(3, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);


            // send a message!
            for (uint8_t i = 0; i < strlen(message) - 4; i++) {
              alpha4.writeDigitAscii(0, message[i]);
              alpha4.writeDigitAscii(1, message[i + 1]);
              alpha4.writeDigitAscii(2, message[i + 2]);
              alpha4.writeDigitAscii(3, message[i + 3]);
              alpha4.writeDisplay();

              delay(200);
            }
          }
          //We Wish You a Merry Christmas
          //delay(3000);
          runPlayer(g, 1, 2, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 0);
          runPlayer(d, 1, 6, HIGH, 600, 0);
          runPlayer(c, 1, 5, HIGH, 600, 0);
          runPlayer(b, 1, 4, HIGH, 300, 1);
          runPlayer(a, 1, 3, HIGH, 600, 1);
          runPlayer(a, 1, 3, HIGH, 600, 1);
          delay (200);
          runPlayer(a, 1, 3, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 0);
          runPlayer(e, 1, 7, HIGH, 600, 0);
          runPlayer(d, 1, 6, HIGH, 600, 0);
          runPlayer(c, 1, 5, HIGH, 300, 1);
          runPlayer(b, 1, 4, HIGH, 600, 1);
          runPlayer(g, 1, 2, HIGH, 600, 1);
          delay (200);
          runPlayer(g, 1, 2, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 0);
          runPlayer(f, 1, 8, HIGH, 600, 0);
          runPlayer(e, 1, 7, HIGH, 600, 0);
          runPlayer(d, 1, 6, HIGH, 300, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(a, 1, 3, HIGH, 600, 1);
          delay (200);
          runPlayer(g, 1, 2, HIGH, 600, 0);
          runPlayer(g, 1, 2, HIGH, 300, 1);
          runPlayer(a, 1, 3, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(b, 1, 4, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 1200, 1);
          delay (300);
          runPlayer(g, 1, 2, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(b, 1, 4, HIGH, 1200, 1);
          runPlayer(b, 1, 4, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(b, 1, 4, HIGH, 600, 1);
          runPlayer(a, 1, 3, HIGH, 600, 1);
          runPlayer(g, 1, 2, HIGH, 1200, 1);
          runPlayer(g, 1, 2, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(g, 1, 2, HIGH, 600, 1);
          runPlayer(g, 1, 2, HIGH, 600, 0);
          runPlayer(g, 1, 2, HIGH, 300, 1);
          runPlayer(a, 1, 3, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(b, 1, 4, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 1200, 1);
          delay (400);
          break;

        case 7:

          { char *message = "  O CHRISTMAS TREE----G2----IS---FIRST---G2     ";

            alpha4.writeDigitRaw(3, 0x0);
            alpha4.writeDigitRaw(0, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(0, 0x0);
            alpha4.writeDigitRaw(1, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(1, 0x0);
            alpha4.writeDigitRaw(2, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(2, 0x0);
            alpha4.writeDigitRaw(3, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);


            // send a message!
            for (uint8_t i = 0; i < strlen(message) - 4; i++) {
              alpha4.writeDigitAscii(0, message[i]);
              alpha4.writeDigitAscii(1, message[i + 1]);
              alpha4.writeDigitAscii(2, message[i + 2]);
              alpha4.writeDigitAscii(3, message[i + 3]);
              alpha4.writeDisplay();

              delay(200);
            }
          }


          //O Christmas Tree
          //delay(3000);
          runPlayer(g, 1, 2, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 0);
          runPlayer(c, 1, 5, HIGH, 300, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 0);
          runPlayer(e, 1, 7, HIGH, 300, 1);
          //delay(300);
          runPlayer(e, 1, 7, HIGH, 600, 1);

          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 0);
          runPlayer(e, 1, 7, HIGH, 300, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(b, 1, 4, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          delay(300);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 0);
          runPlayer(e, 1, 7, HIGH, 300, 1);
          runPlayer(a3, 1, 12, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 0);
          runPlayer(f, 1, 8, HIGH, 300, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          //delay(300);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 0);
          runPlayer(d, 1, 6, HIGH, 300, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 0);
          runPlayer(e, 1, 7, HIGH, 300, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          //delay(300);
          runPlayer(g, 1, 2, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 0);
          runPlayer(c, 1, 5, HIGH, 300, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);

          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 0);
          runPlayer(e, 1, 7, HIGH, 300, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 0);
          runPlayer(e, 1, 7, HIGH, 300, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(b, 1, 4, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          delay (200);
          break;

        case 8:

          { char *message = "  FRE-RE JACQUES----C3----IS---FIRST---C3     ";

            alpha4.writeDigitRaw(3, 0x0);
            alpha4.writeDigitRaw(0, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(0, 0x0);
            alpha4.writeDigitRaw(1, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(1, 0x0);
            alpha4.writeDigitRaw(2, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(2, 0x0);
            alpha4.writeDigitRaw(3, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);


            // send a message!
            for (uint8_t i = 0; i < strlen(message) - 4; i++) {
              alpha4.writeDigitAscii(0, message[i]);
              alpha4.writeDigitAscii(1, message[i + 1]);
              alpha4.writeDigitAscii(2, message[i + 2]);
              alpha4.writeDigitAscii(3, message[i + 3]);
              alpha4.writeDisplay();

              delay(200);
            }
          }


          //Fre-er Jacques
          //delay(3000);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          delay (200);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          delay (200);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 1200, 1);
          delay (200);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 1200, 1);
          delay (200);
          runPlayer(g3, 1, A0, HIGH, 600, 0);
          runPlayer(a3, 1, 12, HIGH, 600, 0);
          runPlayer(g3, 1, A0, HIGH, 600, 0);
          delay (200);
          runPlayer(f, 1, 8, HIGH, 600, 0);
          runPlayer(e, 1, 7, HIGH, 300, 1);
          runPlayer(c, 1, 5, HIGH, 600, 0);
          delay (200);
          runPlayer(g3, 1, A0, HIGH, 600, 0);
          runPlayer(a3, 1, 12, HIGH, 600, 0);
          runPlayer(g3, 1, A0, HIGH, 600, 0);
          delay (200);
          runPlayer(f, 1, 8, HIGH, 600, 0);
          runPlayer(e, 1, 7, HIGH, 300, 1);
          runPlayer(c, 1, 5, HIGH, 600, 0);
          delay (200);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(g, 1, 2, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          delay (200);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(g, 1, 2, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 1200, 1);
          delay (400);
          break;

        case 9:

          { char *message = "  ODE TO JOY----E3----IS---FIRST---E3     ";

            alpha4.writeDigitRaw(3, 0x0);
            alpha4.writeDigitRaw(0, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(0, 0x0);
            alpha4.writeDigitRaw(1, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(1, 0x0);
            alpha4.writeDigitRaw(2, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);
            alpha4.writeDigitRaw(2, 0x0);
            alpha4.writeDigitRaw(3, 0xFFFF);
            alpha4.writeDisplay();
            delay(200);


            // send a message!
            for (uint8_t i = 0; i < strlen(message) - 4; i++) {
              alpha4.writeDigitAscii(0, message[i]);
              alpha4.writeDigitAscii(1, message[i + 1]);
              alpha4.writeDigitAscii(2, message[i + 2]);
              alpha4.writeDigitAscii(3, message[i + 3]);
              alpha4.writeDisplay();

              delay(200);
            }
          }
          // Ode to Joy
          //delay(3000);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          delay (200);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          delay (200);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 0);
          runPlayer(f, 1, 8, HIGH, 300, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 0);
          runPlayer(f, 1, 8, HIGH, 300, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(g, 1, 2, HIGH, 600, 1);
          delay (200);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(g3, 1, A0, HIGH, 600, 1);
          runPlayer(f, 1, 8, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(e, 1, 7, HIGH, 600, 1);
          runPlayer(d, 1, 6, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          runPlayer(c, 1, 5, HIGH, 600, 1);
          delay (200);
          break;

        default:
          {
            //For Counter Presses more than 9:
            //Pressing Start will reset Counter to 0
          }

      }
      buttonPushCounter = 0;
      plaa = 0;
      plab = 0;
      if (buttonPushCounter >= 0 && buttonPushCounter <= 10) {
        String buttonPushCounterDisp = String (buttonPushCounter);
        alpha4.writeDigitAscii(3, buttonPushCounterDisp[0]);
        alpha4.writeDisplay();
        //matrix.writeDigitNum(3, buttonPushCounter);
        //matrix.writeDisplay();
      }


    }
  }
}



