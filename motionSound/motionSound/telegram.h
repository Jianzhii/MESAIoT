#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char* host = "api.telegram.org";
const char* endpoint = "/bot5291963985:AAGrHHVy5H7Y4me5uQmRHqZUcgoR6JAoff4/sendMessage";
const int port = 443;
const char* fingerprint = "F2 AD 29 9C 34 48 DD 8D F4 CF 52 32 F6 57 33 68 2E 81 C1 90";

void sendTele(char* body) {
  WiFiClientSecure httpsClient;

  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000); // 15 seconds timeout limit
  delay(1000);

  int retry = 0;
  Serial.println("try to connecct");
  while ((!httpsClient.connect(host, port)) && (retry < 30)) {
    delay(1000);
    Serial.println('...');
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
  Serial.println("request sent");

  // String line;
  while (httpsClient.available()) {
    // line = httpsClient.readStringUntil('\n');  //Read Line by Line
    // Serial.println(line); //Print response
    delay(1000);
  }
  Serial.println("Connection ended");
  // return true;
}