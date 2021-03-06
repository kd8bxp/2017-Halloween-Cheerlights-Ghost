/* Copyright (c) 2017 LeRoy Miller
 *  
 *  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>
 */

#include <ESP8266WiFi.h>
#include "src/PubSubClient/PubSubClient.h"
#include "src/Adafruit_NeoPixel/Adafruit_NeoPixel.h"

// Update these with values suitable for your network.

const char* ssid = "";
const char* password = "";
const char* mqtt_server = "mqtt.cheerlights.com";

String color;
int brightness = 150;
int p = 20; //p = neopixel delay

Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, D3, NEO_GRB + NEO_KHZ800);

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  //Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  strip.begin();
  strip.show();
  strip.setBrightness(brightness);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  //  Serial.print(".");
  }

  //Serial.println("");
  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");
  color = "";
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    color+=(char)payload[i];
  }
  //Serial.println();
  //Serial.println("Color: " + color);

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    uint32_t chipid=ESP.getChipId();
    char clientid[25];
    snprintf(clientid,25,"CheerlightGhost-%08X",chipid);
    if (client.connect(clientid)) {
            client.subscribe("cheerlights");
    } else {
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
setcolor();  
colorDisplay(strip.Color(0,0,0),p);

}

void setcolor() {
  if (color == "white") {colorDisplay(strip.Color(255,255,255),p);
} else if (color == "warmwhite" || color == "oldlace" ) {colorDisplay(strip.Color(253,245,230),p); //has a cyan color to me.
} else if (color == "black" || color == "off") {colorDisplay(strip.Color(0,0,0),p);
} else if (color == "red") { colorDisplay(strip.Color(255,0,0),p);
} else if (color == "green") { colorDisplay(strip.Color(0, 255, 0), p);
} else if (color == "blue") { colorDisplay(strip.Color(0,0,255),p);
} else if (color == "cyan") {colorDisplay(strip.Color(0,255,255),p);
} else if (color == "magenta") {colorDisplay(strip.Color(255,0,255),p);
} else if (color == "yellow") {colorDisplay(strip.Color(255,255,0),p); //looks green/yellow to me
} else if (color == "purple") {colorDisplay(strip.Color(102,51,204),p); //looks more blue to me (maybe 160, 32, 240)
} else if (color == "orange") {colorDisplay(strip.Color(255,153,0),p); //looks more yellow to me
} else if (color == "pink") { colorDisplay(strip.Color(255, 192, 203),p);}

}

void colorDisplay(uint32_t c, uint8_t wait) {

for (int i=0;i<strip.numPixels(); i++) { 
strip.setPixelColor(i, c);

strip.show();
delay(wait);
}
}
