#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

const int POT_PIN = 13;
const int LED_BUILTIN = 2;
uint8_t receiverAddress[] = {0xEC, 0xC9, 0xFF, 0xE2, 0x8D, 0x18}; // MAC address of ESP32 on the car

// Struct to send data
typedef struct 
{
  int angle;
} 
ServoData;

ServoData outgoingData;

void blink (int led_PIN, int time_length)
{
  digitalWrite(led_PIN, HIGH);
  delay(time_length);
  digitalWrite(led_PIN, LOW);
  delay(time_length);
}

void setup() 
{
  Serial.begin(115200);
  delay(100); // Wait for terminal to init

  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESP-NOW");
    for (int i = 0; i > 10; i++) // Blink 10 times if init ESP-NOW
    {
      blink(LED_BUILTIN, 500);
    }
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) 
  {
    Serial.println("Failed to add peer");
    for (int i = 0; i > 10; i++) // Blink 10 times if cannot connect
    {
      blink(LED_BUILTIN, 500);
    }
    return;
  }
    blink(LED_BUILTIN, 1000); // Blink 1 sec once if successfully connected
}

void loop() 
{
  int potValue = analogRead(POT_PIN);
  int angle = map(potValue, 0, 4095, 0, 180);

  outgoingData.angle = angle;
  esp_now_send(receiverAddress, (uint8_t *)&outgoingData, sizeof(outgoingData));

  Serial.printf("Sent angle: %i\n", angle);
  delay(100);
}
