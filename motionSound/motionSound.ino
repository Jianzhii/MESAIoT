#include "arduino_secrets.h"
#include <WiFiUdp.h>

#include "thingProperties.h"
#include "telegram.h"

// group chat: -1001318460964

const long utcOffsetInSeconds = 28800;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int motionPin = 5;              // the pin that the sensor is atteched to
int soundPin = 16;
int motionCounter = 0;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  pinMode(motionPin, INPUT);
  pinMode(soundPin, INPUT);
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  timeClient.begin();

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  
  delay(10000);
}

void loop() {
  ArduinoCloud.update();
  timeClient.update();

  Serial.println("############## Motion Check ##############");

  motionVal = digitalRead(motionPin);
  Serial.print("Motion Sensor Value: ");
  Serial.println(motionVal);

  if (timeClient.getHours() > 2 or timeClient.getHours() < 3) {
    Serial.println("it is morning they sleeping hello");
    char* body = "{\"chat_id\": -741531015, \"text\": \"sleeping larh dey.\"}";
    sendTele(body);
  } else {
    if (motionVal == 0) {
      motionCounter += 5;
      if (motionCounter == 60) {
        motionCounter = 0;
        Serial.println("No motion detected for 1 minute") ;

        // bool sent = false;
        // while (not sent) {
        char* body = "{\"chat_id\": -741531015, \"text\": \"No motion detected for 1 minute.\"}";
        sendTele(body);
        //   Serial.println("sending message");
        // }
      }
    } else {
      motionCounter = 0 ;
    }
  }

  Serial.println("############## Sound Check ##############");
  soundVal = digitalRead(soundPin);

  Serial.print("Sound Sensor Value: ");
  Serial.println(soundVal);

  if (soundVal == HIGH) {
    Serial.println("Loud sound detected");
    // bool sent = false;
    // while (not sent) {
    char* body = "{\"chat_id\": -741531015, \"text\": \"Loud sounds have been detected...\"}";
    sendTele(body);
    //   Serial.println("sending message");
    // }
  } else {
    Serial.println("No sound detected");
  }

  delay(2000); // wait 2s for next reading
}



/*
  Since Val is READ_WRITE variable, onValChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onValChange()  {
  // Add your code here to act upon Val change
}