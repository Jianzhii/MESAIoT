#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "ThingSpeak.h"

#include "telegram.h"

char* firstMedicineBody = "{\"chat_id\": -1001318460964, \"text\": \"1st reminder: Elderly has not taken medication\"}";
char* secondMedicineBody = "{\"chat_id\": -1001318460964, \"text\": \"2nd reminder: Elderly has not taken medication\"}";
char* takenBody = "{\"chat_id\": -1001318460964, \"text\": \"Elderly has taken medication\"}";

const long utcOffsetInSeconds = 28800;

int touch = 0;
int led = 16;
float pressureVal = 0;

bool medicationTaken = false;
bool reminderSent = false;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
WiFiClient client;

void setup() {
  pinMode(touch, INPUT);
  pinMode(led, OUTPUT);

  Serial.begin(9600);

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

  pressureVal = analogRead(touch);
  Serial.println(pressureVal);
  
  if (pressureVal < 50) {
    medicationTaken = true;
    digitalWrite(led, LOW);
    int res ;
    do {
      Serial.println("Atttempting to update ThingSpeak...");
      res = ThingSpeak.writeField(channelNo, 3, 1, writeKey);
      delay(1000);
    } while (res!=200);
    Serial.println("Channel update successful.");
  }

  if (timeClient.getHours() == 20 && timeClient.getMinutes() == 00 && timeClient.getSeconds() == 00) {
    if (not medicationTaken){
      reminderSent = true;
      sendTele(firstMedicineBody);
      digitalWrite(led, HIGH);
    }
  }

  if (timeClient.getHours() == 22 && timeClient.getMinutes() == 00 && timeClient.getSeconds() == 00) {
    if (not medicationTaken) {      
      reminderSent = true;
      sendTele(secondMedicineBody);
    }
  }

  if (reminderSent and medicationTaken) {
    sendTele(takenBody);
    reminderSent = false;
  }

  if (timeClient.getHours() == 00 && timeClient.getMinutes() == 00 && timeClient.getSeconds() == 00) {
    reminderSent = false;
    medicationTaken = false;
  }

  delay(1000);  
}
