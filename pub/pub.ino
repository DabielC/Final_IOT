#include <WiFiS3.h>
#include <MQTTClient.h>

const char WIFI_SSID[] = "✌️";          
const char WIFI_PASSWORD[] = "ruheeyaiyai";  

const char MQTT_BROKER_ADRRESS[] = "phycom.it.kmitl.ac.th";  
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "65070009";  
const char MQTT_USERNAME[] = "";  
const char MQTT_PASSWORD[] = "";  

const char PUBLISH_TOPIC[] = "65070009";    

const int PUBLISH_INTERVAL = 500;  // 500ms (Real-time updates)

WiFiClient network;
MQTTClient mqtt = MQTTClient(256);

unsigned long lastPublishTime = 0;
bool lastSwitchState = HIGH;  // Track last switch state

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);  // Switch
  pinMode(A0, INPUT);  // Potentiometer

  connectWiFi();
  connectToMQTT();
}

void loop() {
  mqtt.loop();

  if (millis() - lastPublishTime > PUBLISH_INTERVAL) {
    int switchState = digitalRead(2);
    
    if (switchState == LOW && lastSwitchState == HIGH) {
      mqtt.publish(PUBLISH_TOPIC, "ON");
      Serial.println("Switch Pressed: LED ON");
    } else if (switchState == HIGH && lastSwitchState == LOW) {
      mqtt.publish(PUBLISH_TOPIC, "OFF");
      Serial.println("Switch Released: LED OFF");
    }
    lastSwitchState = switchState;

    sendBrightness();
    lastPublishTime = millis();
  }
}

void sendBrightness() {
  int val = analogRead(A0);
  int brightness = map(val, 0, 1023, 0, 100);  // Scale to 0-100%
  
  String val_str = String(brightness);
  char messageBuffer[10];
  val_str.toCharArray(messageBuffer, 10);
  
  mqtt.publish(PUBLISH_TOPIC, messageBuffer);
  Serial.println("Potentiometer: " + String(brightness) + "%");
}

void connectWiFi() {
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.print("Connecting to WiFi: ");
    Serial.println(WIFI_SSID);
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(5000);
  }
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTT() {
  mqtt.begin(MQTT_BROKER_ADRRESS, MQTT_PORT, network);
  while (!mqtt.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nConnected to MQTT broker!");
}
