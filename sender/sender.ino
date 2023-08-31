#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xD4, 0xD4, 0xDA, 0xFC, 0xD5, 0xE8};

// Structure example to send data
// Must match the receiver structure
typedef struct robot_arm_pwm_values {
  byte _1;
  byte _2;
  byte _3;
  byte _4;
} robot_arm_pwm_values;

//message
typedef struct struct_message {
  byte y_direction; //-127 +127 ileri geri bilgisi
  byte x_direction; //-127 sol, +127 sağ dönme bilgisi
  byte buttons; //8 buton kapasitesi
  robot_arm_pwm_values RobotKol;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  for (int i = 0; i < 10; i++) {
    Serial.println("verici basliyorum");
    delay(500);
  }
  delay(1000);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}
unsigned long send_timer = micros();
void loop() {
  send_timer = micros();
  // Set values to send
  myData.y_direction = 110;
  myData.x_direction = 220;
  myData.buttons = 0b00101101;
  myData.RobotKol._1 = 129;
  myData.RobotKol._2 = 60;
  myData.RobotKol._3 = 90;
  myData.RobotKol._4 = 180;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(20);

}
