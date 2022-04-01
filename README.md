## Project MESA IoT

The following code files are for a project in collaboration between Singapore Management University, School of Computing and Information System, CS462 Internet of Things: Technology and Application module and Code for Singapore. 

Each folder contains codes for one board each and will require a secret.h file to compile successfully. Contents within secret.h are as follows: 

```
// Wifi and Thingspeak secrets
#define wifiname xxxxxxx  (Wifi name)
#define password xxxxxxx  (Wifi password)
#define channelNo xxxxxxx (ThingSpeak Channel ID)
#define writeKey "xxxxxxx" (ThingSpeak Write API Key)

// Telegram Secrets
#define host "xxxxxxx" (Telegram host)
#define endpoint "xxxxxxx" (Telegram endpoint for sending messages (includes bot secret key))
#define port 443 (Port for requests to be sent to)
#define fingerprint "xxxxxxx" (Telegram SSL cert)
```
