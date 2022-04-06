// import libraries for WiFI and Http requests
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <espnow.h>
#include "ThingSpeak.h"
#include "telegram.h"

typedef struct message {
   bool sendMsg;
} message;

message myMessage;
void onSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail\n" + String(sendStatus));
  }
}

char* motionBody = "{\"chat_id\": -1001318460964, \"text\": \"No motion detected for 5 minute.\"}";
char* soundBody = "{\"chat_id\": -1001318460964, \"text\": \"Loud sounds have been detected.\"}";

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
  
  // Get Mac Add
  Serial.print("Mac Address: ");
  Serial.print(WiFi.macAddress());
  Serial.println("ESP-Now Sender");
  // Initializing the ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Problem during ESP-NOW init");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);  
  Serial.println("Registering send callback function");
  esp_now_register_send_cb(onSent);
  // Register the peer
  Serial.println("Registering a peer");
  int addPeer = esp_now_add_peer(broadcastAddress, 2, 3, NULL, 0);
  if (addPeer == 0) {
    Serial.println("Successfully registered peer.");
  } else {
    Serial.println("Failed to register peer.");
  }

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
//    Serial.println("it is night time.");
//    sendTele(sleepBody);
//    motionCounter += 5;
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
        myMessage.sendMsg = true;
        esp_now_send(broadcastAddress, (uint8_t *) &myMessage, sizeof(myMessage));
        delay(5000);
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
    myMessage.sendMsg = true;
    esp_now_send(broadcastAddress, (uint8_t *) &myMessage, sizeof(myMessage));
    delay(5000);
    motionCounter += 5;
  } else {
    Serial.println("No sound detected");
  }
  
  delay(1000);
}
