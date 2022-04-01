#include <NTPClient.h>
#include <WiFiUdp.h>
#include "ThingSpeak.h"

#include "telegram.h"

char* body = "{\"chat_id\": -1001318460964, \"text\": \"Potential gas leak or fire!\"}";

int gasPin = A0;
float gasValue;

WiFiClient  client;
void setup(){
  Serial.begin(9600); // sets the serial port to 9600
  Serial.println("Gas sensor warming up!");

  // connect to Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiname, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("connecting....");
  }
  Serial.println("Connected");
  ThingSpeak.begin(client); 
  delay(20000); // allow the MQ-2 to warm up
}

void loop(){
  gasValue = analogRead(gasPin); // read analog input pin 0
  
  Serial.print("Sensor Value: ");
  Serial.println(gasValue);
  
  if(gasValue > 50)
  {
    Serial.println("Smoke detected!");
    sendTele(body);
  }
  
  int res; 
  do {
    Serial.println("Attempting to update ThingSpeak...");
    res = ThingSpeak.writeField(channelNo, 4, gasValue, writeKey);
    delay(1000);
  } while (res != 200);
  Serial.println("Channel update successful.");
  
  Serial.println("");
  delay(2000); // wait 2s for next reading
}
