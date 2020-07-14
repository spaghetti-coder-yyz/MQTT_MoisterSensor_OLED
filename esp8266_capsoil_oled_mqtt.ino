// Documentation:
// ESP8266 with SSD1306 OLED
// https://randomnerdtutorials.com/esp8266-0-96-inch-oled-display-with-arduino-ide/
//
// ESP8266 / WEMOS with Capacitive Soil Moisture Sensor
// https://www.hackster.io/xxlukas84/capacitive-soil-moisture-sensor-v1-2-with-wemos-d1-lite-d63455


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define PIN_RESET 255
#define DC_JUMPER 0

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Set Moisture Variable
int i;
int moi = 0;  

// WIFI settigs and MQTT broker server
const char* ssid = "B&M-IoT";
const char* password = "6474620281";
const char* mqtt_server = "172.16.1.31";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
//float temp = 0;
//int inPin = 5;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient_temperature_sensor")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void setup()
{
  Serial.begin(115200);
  setup_wifi(); 
  client.setServer(mqtt_server, 1883);
  

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }
    delay(2000);
    display.clearDisplay();

}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
  
  //oled.clear(PAGE); 
  moi = analogRead(0); 
  Serial.println(moi);
  //oled.setFontType(0);
  //oled.setCursor(7, 0);
  //oled.print("Moisture: ");
  //oled.setFontType(2);
  //oled.setCursor(14, 15);
  //oled.print(moi);  // Print an integer  
  //oled.display(); 

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(5, 8);
  display.println(moi);
  display.display(); 

  // Change publish topic to be unique for this device
  client.publish("ha/_capsoil2", String(moi).c_str());
  
  delay(1000);

  
  
  
  }
}
