#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <DFRobot_B_LUX_V30B.h>

// ------------------ WiFi & MQTT Setup ------------------
const char* ssid = "RAINET_Plus";
const char* password = "h1pp0p0tamu5";
const char* mqtt_server = "mqtt-dashboard.com";
const int mqtt_port = 1883;
const char* mqtt_topic_pub = "esp32/lightSystem";
const char* mqtt_topic_sub = "esp32/manual/control";

WiFiClient espClient;
PubSubClient client(espClient);

// ------------------ Sensor & Pins ------------------
DFRobot_B_LUX_V30B myLux(15); // EN pin = 15

#define stepPin 32
#define dirPin 33
#define enablePin 34

#define topLimitSwitch 16
#define bottomLimitSwitch 23

#define manualUpBtn 27
#define manualDownBtn 14
#define modeToggleBtn 12

#define ledManual 19
#define ledAuto   18

// ------------------ System Parameters ------------------
const int stepDelay = 40;
const float thresholdLux = 200.0;
const float maxLux = 200000.0;

enum MotorState { IDLE, MOVING_UP, MOVING_DOWN };
MotorState currentState = IDLE;

enum Mode { MANUAL, AUTO };
Mode currentMode = AUTO;

bool lastToggleState = HIGH;
bool lastManualUpState = HIGH;
bool lastManualDownState = HIGH;

// ------------------ Functions ------------------
void moveStep(bool direction) {
  digitalWrite(dirPin, direction);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(stepDelay);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(stepDelay);
}

void updateModeLEDs() {
  digitalWrite(ledManual, currentMode == MANUAL ? HIGH : LOW);
  digitalWrite(ledAuto, currentMode == AUTO ? HIGH : LOW);
}

void setup_wifi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String command = "";
  for (int i = 0; i < length; i++) {
    command += (char)payload[i];
  }

  Serial.print("Received MQTT command: ");
  Serial.println(command);

  if (currentMode == MANUAL) {
    if (command == "UP" && digitalRead(topLimitSwitch) == HIGH) {
      currentState = MOVING_UP;
    } else if (command == "DOWN" && digitalRead(bottomLimitSwitch) == HIGH) {
      currentState = MOVING_DOWN;
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe(mqtt_topic_sub);
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// ------------------ Setup ------------------
void setup() {
  Serial.begin(9600);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);

  pinMode(topLimitSwitch, INPUT_PULLUP);
  pinMode(bottomLimitSwitch, INPUT_PULLUP);
  pinMode(manualUpBtn, INPUT_PULLUP);
  pinMode(manualDownBtn, INPUT_PULLUP);
  pinMode(modeToggleBtn, INPUT_PULLUP);
  pinMode(ledManual, OUTPUT);
  pinMode(ledAuto, OUTPUT);

  Wire.begin();
  myLux.begin();

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqtt_callback);

  Serial.println("System Initialized.");
}

// ------------------ Loop ------------------
void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  // Mode toggle button
  bool toggleState = digitalRead(modeToggleBtn);
  if (lastToggleState == HIGH && toggleState == LOW) {
    currentMode = (currentMode == AUTO) ? MANUAL : AUTO;
    Serial.println((currentMode == AUTO) ? "Switched to AUTO" : "Switched to MANUAL");
    currentState = IDLE;
    delay(300);
  }
  lastToggleState = toggleState;
  updateModeLEDs();

  bool topLimit = digitalRead(topLimitSwitch) == LOW;
  bool bottomLimit = digitalRead(bottomLimitSwitch) == LOW;

  float lux = myLux.lightStrengthLux();
  if (lux > maxLux) lux = 0;

  if (currentMode == AUTO) {
    if (lux > thresholdLux && !topLimit) {
      currentState = MOVING_UP;
    } else if (lux < thresholdLux && !bottomLimit) {
      currentState = MOVING_DOWN;
    } else {
      currentState = IDLE;
    }
  }

  if (currentMode == MANUAL) {
    bool manualUpState = digitalRead(manualUpBtn);
    if (lastManualUpState == HIGH && manualUpState == LOW && !topLimit) {
      currentState = MOVING_UP;
    }
    lastManualUpState = manualUpState;

    bool manualDownState = digitalRead(manualDownBtn);
    if (lastManualDownState == HIGH && manualDownState == LOW && !bottomLimit) {
      currentState = MOVING_DOWN;
    }
    lastManualDownState = manualDownState;
  }

  // Motor move logic
  while (currentState != IDLE) {
    if (currentState == MOVING_UP) {
      moveStep(LOW);
      if (digitalRead(topLimitSwitch) == LOW) currentState = IDLE;
    } else if (currentState == MOVING_DOWN) {
      moveStep(HIGH);
      if (digitalRead(bottomLimitSwitch) == LOW) currentState = IDLE;
    }
  }

  // Publish status to Node-RED
  String modeStr = (currentMode == AUTO) ? "AUTO" : "MANUAL";
  String motorStr = (currentState == MOVING_UP) ? "UP" :
                    (currentState == MOVING_DOWN) ? "DOWN" : "IDLE";

  String payload = "{\"lux\":" + String(lux, 1) +
                   ",\"mode\":\"" + modeStr + "\"" +
                   ",\"motor\":\"" + motorStr + "\"}";

  client.publish(mqtt_topic_pub, payload.c_str());
  Serial.println("MQTT -> " + payload);

  delay(3000);
}
