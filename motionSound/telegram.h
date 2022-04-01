#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include "secret.h"

void sendTele(char* body) {
  WiFiClientSecure httpsClient;
  int retry = 0;

  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000); // 15 seconds timeout limit
  delay(1000);

  retry = 0;
  while ((!httpsClient.connect(host, port)) && (retry < 30)) {
    delay(1000);
    retry++;
  }
  if (retry == 30) {
    Serial.println("Connection failed");
  }
  
  httpsClient.print(String("POST ") + endpoint + " HTTP/1.1\r\n" +
                    "Host: " + host + "\r\n" +
                    "Content-Type: application/json" + "\r\n" +
                    "Content-Length:" + strlen(body) + "\r\n\r\n" +
                    body + "\r\n" +
                    "Connection: close\r\n\r\n");
  Serial.println("Telegram message sent");
  // String line;
  while (httpsClient.available()) {
    // line = httpsClient.readStringUntil('\n');  //Read Line by Line
    // Serial.println(line); //Print response
    delay(1000);
  }
  Serial.println("Connection ended");
    delay(5000);
}
