#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

Servo myServo;
const int SERVO_PIN = 33;

// Struct to receive data
typedef struct {
  int angle;
} ServoData;

ServoData incomingData;

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingDataBytes, int len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));
  Serial.print("Received angle: ");
  Serial.println(incomingData.angle);
  myServo.write(incomingData.angle);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  myServo.attach(SERVO_PIN);
}

void loop() {
  // Nothing here, everything happens via callback
}
