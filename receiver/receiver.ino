#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct robot_arm_pwm_values {
  byte _1;
  byte _2;
  byte _3;
  byte _4;
} robot_arm_pwm_values;

//message
typedef struct struct_message {
  byte y_direction; //0-255
  byte x_direction; //0-255 sağ dönme bilgisi
  byte buttons; //8 buton kapasitesi
  robot_arm_pwm_values RobotKol;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.print(len);
  Serial.print("y");
  Serial.println(myData.y_direction);
  Serial.print("x");
  Serial.println(myData.x_direction);
  Serial.print("butons:");
  Serial.println(myData.buttons, BIN);
  Serial.print("robot arm:");
  Serial.print(myData.RobotKol._1);
  Serial.print(" ");
  Serial.print(myData.RobotKol._2);
  Serial.print(" ");
  Serial.print(myData.RobotKol._3);
  Serial.print(" ");
  Serial.print(myData.RobotKol._4);
  Serial.print(" ");
  Serial.println();
}

void setup() {
  
  // Initialize Serial Monitor
  Serial.begin(115200);
  for (int i = 0; i < 10; i++) {
    Serial.println("alici basliyorum");
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

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

}
