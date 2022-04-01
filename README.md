## Project MESA IoT

The following code files are for a project in collaboration between Singapore Management University, School of Computing and Information System, CS462 Internet of Things: Technology and Application module and Code for Singapore. 

Each folder contains codes for one board each and will require a secret.h file to compile successfully. Contents within secret.h are as follows: 

```
// Wifi and Thingspeak secrets
#define wifiname xxxxxxx _(Wifi name)_
#define password xxxxxxx  _(Wifi password)_ 
#define channelNo xxxxxxx _(ThingSpeak Channel ID)_
#define writeKey "xxxxxxx" _(ThingSpeak Write API Key)_

// Telegram Secrets
#define host "xxxxxxx" _(Telegram host)_
#define endpoint "xxxxxxx" _(Telegram endpoint for sending messages (includes bot secret key))_
#define port 443 _(Port for requests to be sent to)_
#define fingerprint "xxxxxxx" _(Telegram SSL cert)_
```
