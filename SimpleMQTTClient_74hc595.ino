//https://github.com/plapointe6/EspMQTTClient/blob/master/examples/SimpleMQTTClient/SimpleMQTTClient.ino
//mosquitto_pub -h 94.228.120.144 -t esp32/leds -m 0   -- all leds off
//mosquitto_pub -h 94.228.120.144 -t esp32/leds -m 255   -- all 8 leds on
//mosquitto_pub -h 94.228.120.144 -t esp32/leds -m 1   -- led 1 on
//mosquitto_pub -h 94.228.120.144 -t esp32/leds -m 3   -- led 1 on and led 2 on
//mosquitto_sub -h 94.228.120.144 -t esp32/leds 


#include "EspMQTTClient.h"

int latchPin = 1;//3;
int clockPin = 2;
int dataPin = 0;
long randNumber;
int led_num = 0;

EspMQTTClient client(
"Bauman_Robotics",
"31415926",
"94.228.120.144", // MQTT Broker server ip
"MQTTUsername", // Can be omitted if not needed
"MQTTPassword", // Can be omitted if not needed
"TestClient", // Client name that uniquely identify your device
1883 // The MQTT port, default to 1883. this line can be omitted
);

void setup()
{
//set pins to output so you can control the shift register
pinMode(latchPin, OUTPUT);
pinMode(clockPin, OUTPUT);
pinMode(dataPin, OUTPUT);

// Optional functionalities of EspMQTTClient
//client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
client.enableLastWillMessage("TestClient/lastwill", "I am going offline"); // You can activate the retain flag by setting the third parameter to true
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
client.subscribe("esp32/leds", [](const String & payload) {
led_num = payload.toInt();
});
}

void loop()
{
client.loop();
// take the latchPin low so
// the LEDs don't change while you're sending in bits:
digitalWrite(latchPin, LOW);
// shift out the bits:
shiftOut(dataPin, clockPin, MSBFIRST, led_num);
//take the latch pin high so the LEDs will light up:
digitalWrite(latchPin, HIGH);
}