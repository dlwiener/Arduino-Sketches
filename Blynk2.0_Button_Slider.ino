/*************************************************************

  You’ll need:
   - Blynk IoT app (download from App Store or Google Play)
   - ESP8266 board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/
#include <Servo.h>   //dw added to control servo with slider from V0
Servo servo;         //dw added to control servo with slider from V0 

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLeZCMGPgw"
#define BLYNK_DEVICE_NAME           "Button"
#define BLYNK_AUTH_TOKEN            "IJhzWrnsjPtq52Qvel3S3NrFaYmKfyB7"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "BlackPete-2.4G";
char pass[] = "rorydog54";

//dw added the following to control servo with slider from V0
BLYNK_WRITE(V0){
  servo.write(param.asInt());
}


void setup()
{
  // Debug console
  Serial.begin(115200);
  servo.attach(13);   //dw 13 means pin D13 of Blynk Board
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
