// import libraries for WiFI and Http requests
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char* wifiname = "asdf"; // change to your house wifi thing
const char* password = "sdfasdf";
const char* host= "api.telegram.org";
const char* endpoint = "/bot5291963985:AAGrHHVy5H7Y4me5uQmRHqZUcgoR6JAoff4/sendMessage";
const int port = 443;
const char* fingerprint = "F2 AD 29 9C 34 48 DD 8D F4 CF 52 32 F6 57 33 68 2E 81 C1 90";
const char* body = "{ \"chat_id\": -1001318460964, \"text\": \"Motion detected\" }";

int sensor = 5;              // the pin that the sensor is atteched to
int val = 0;                 // variable to store the sensor status (value)
int var = 0;

void setup() {
  pinMode(sensor, INPUT);    // initialize sensor as an input
  Serial.begin(9600);        // initialize serial

  // connect to Wifi
  WiFi.begin(wifiname, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("connecting....");
  }
  Serial.println("Connected");
  
  while (var  < 60) {
    val = digitalRead(sensor);
    Serial.println(val) ;
    if (val == 0) {
      var++ ;
    } else {
      var = 0 ;

      // test send msg
      WiFiClientSecure httpsClient;
      
      httpsClient.setFingerprint(fingerprint);
      httpsClient.setTimeout(15000); // 15 seconds timeout limit
      delay(1000);

      int retry=0;
      while ((!httpsClient.connect(host, port)) && (retry < 30)) {
        delay(100);
        Serial.println('...');
        retry++;
      }
      if (retry == 30) {
        Serial.println("Connection failed");
      }

      httpsClient.print(String("POST ") + endpoint + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Content-Type: application/json"+ "\r\n" +
                   "Content-Length:" + strlen(body) + "\r\n\r\n" +
                   "body:" + body + "\r\n" +
                   "Connection: close\r\n\r\n");      
      Serial.println("request sent");
  
      String line;
      while(httpsClient.available()){        
        line = httpsClient.readStringUntil('\n');  //Read Line by Line
        Serial.println(line); //Print response
      }
      Serial.println("Connection ended");
    }
    
    delay(1000); // goes into while loop after 1 second
  }

  Serial.println("No motion detected for 1 minute") ;
}

void loop(){


  
}
