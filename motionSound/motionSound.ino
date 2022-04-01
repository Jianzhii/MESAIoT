// import libraries for WiFI and Http requests
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "ThingSpeak.h"

#include "telegram.h"

char* sleepBody = "{\"chat_id\": -1001318460964, \"text\": \"Its sleeping time..\"}";
char* motionBody = "{\"chat_id\": -1001318460964, \"text\": \"No motion detected for 5 minute.\"}";
char* soundBody = "{\"chat_id\": -1001318460964, \"text\": \"Loud sounds have been detected...\"}";

const long utcOffsetInSeconds = 28800;

int motionPin = 5;              
int soundPin = A0;
int motionCounter = 0;
int motionVal;
int soundVal;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
WiFiClient  client;

void setup() { 
  pinMode(motionPin, INPUT);
//  pinMode(soundPin, INPUT);
  Serial.begin(9600);        // initialize serial
  
  // connect to Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiname, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("connecting....");
  }
  Serial.println("Connected");
  ThingSpeak.begin(client); 
  timeClient.begin();
}

void loop() {
  timeClient.update();
  Serial.println("############## Motion Check ##############");
  if (timeClient.getHours() > 22 and timeClient.getHours() < 6) {
    Serial.println("it is morning they sleeping hello");
    sendTele(sleepBody);
    motionCounter += 5;
  } else {
    motionVal = digitalRead(motionPin);
    Serial.print("Motion Sensor Value: ");
    Serial.println(motionVal);
    int res; 
    do {
      Serial.println("Attempting to update ThingSpeak...");
      res = ThingSpeak.writeField(channelNo, 2, motionVal, writeKey);
      delay(1000);
      motionCounter += 1;
    } while (res != 200);
    Serial.println("Channel update successful.");
    
    if (motionVal == 0) {
      motionCounter += 1;
      if (motionCounter >= 300) {
        motionCounter = 0;
        Serial.println("No motion detected for 5 minute");
        sendTele(motionBody);
        motionCounter += 5;
      }
    } else {
      motionCounter = 0 ;
    }
  }

  Serial.println("############## Sound Check ##############");
  soundVal = analogRead(soundPin);

  Serial.print("Sound Sensor Value: ");
  Serial.println(soundVal);
  int res; 
  do {
    Serial.println("Attempting to update ThingSpeak...");
    res = ThingSpeak.writeField(channelNo, 1, soundVal, writeKey);
    delay(1000);
    motionCounter += 1;
  } while (res != 200);        
  Serial.println("Channel update successful.");
  
  if (soundVal > 50) {
    Serial.println("Loud sound detected");
    sendTele(soundBody);
    motionCounter += 5;
  } else {
    Serial.println("No sound detected");
  }
  
  delay(1000);
}
