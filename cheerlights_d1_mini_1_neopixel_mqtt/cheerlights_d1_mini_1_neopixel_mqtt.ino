

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

// Update these with values suitable for your network.

const char* ssid = "Motel6";
const char* password = "";
const char* mqtt_server = "iot.eclipse.org";

String color;

String lastCommandString = "black";
int brightness = 150;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, D2, NEO_GRB + NEO_KHZ800);

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
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
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  color = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    color+=(char)payload[i];
  }
  Serial.println();
  Serial.println("Color: " + color);

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266ClientCheerlightGhost123")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("cheerlights");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
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
}

void setcolor() {
  if (color == "white") {colorWipe(strip.Color(255,255,255),50);
} else if (color == "warmwhite" || color == "oldlace" ) {colorWipe(strip.Color(253,245,230),50);
} else if (color == "black" || color == "off") {colorWipe(strip.Color(0,0,0),50);
} else if (color == "red") { colorWipe(strip.Color(255,0,0),50);
} else if (color == "green") { colorWipe(strip.Color(0, 255, 0), 50);
} else if (color == "blue") { colorWipe(strip.Color(0,0,255),50);
} else if (color == "cyan") {colorWipe(strip.Color(0,255,255),50);
} else if (color == "magenta") {colorWipe(strip.Color(255,0,255),50);
} else if (color == "yellow") {colorWipe(strip.Color(255,255,0),50);
} else if (color == "purple") {colorWipe(strip.Color(102,51,204),50);
} else if (color == "orange") {colorWipe(strip.Color(255,153,0),50);
} else if (color == "pink") { colorWipe(strip.Color(255, 192, 203),50);}
}

void colorWipe(uint32_t c, uint8_t wait) {
  
  //for(uint16_t i=0; i<strip.numPixels(); i++) {
strip.setPixelColor(0, c);
strip.show();

}


