#include <esp_now.h>
#include <WiFi.h>

#define YPIN A0
#define XPIN A1

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xD4, 0xD4, 0xDA, 0xFC, 0xD5, 0xE8};

double y = 0;
double x = 0;

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

  pinMode(D9, INPUT_PULLUP);
  pinMode(D10, INPUT_PULLUP);
  joystick_calibration();

  Serial.begin(115200);
  for (int i = 0; i < 10; i++) {
    Serial.println("verici basliyorum");
    delay(500);
  }
  delay(1000);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;

  }
}
void loop() {
  myData.y_direction -= myData.y_direction / 10;
  myData.x_direction -= myData.x_direction / 10;
  myData.y_direction += readJoyStick(YPIN, y) / 10;
  myData.x_direction += readJoyStick(XPIN, x) / 10;
  bitWrite(myData.buttons, 0, !digitalRead(D9));
  bitWrite(myData.buttons, 1, !digitalRead(D10));
  myData.RobotKol._1 = 0;
  myData.RobotKol._2 = 0;
  myData.RobotKol._3 = 0;
  myData.RobotKol._4 = 0;

  Serial.print(analogRead(YPIN));
  Serial.print("\t");
  Serial.print(myData.y_direction);
  Serial.print("\t");
  Serial.print(analogRead(XPIN));
  Serial.print("\t");
  Serial.print(myData.x_direction);
  Serial.print("\t");
  Serial.print(myData.buttons, BIN);
  Serial.println();


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

void joystick_calibration() {
  Serial.println("joystick calibration started");
  for (int i = 0; i < 100; i++) {
    y += analogRead(YPIN) / 100;
    x += analogRead(XPIN) / 100;
    delay(10);
  }
  Serial.print("calibration : ");
  Serial.print(y);
  Serial.print(" ");
  Serial.println(x);
  delay(2000);
}

byte readJoyStick(int pin, int cal) {
  int val = analogRead(pin);
  if (val > cal) {
    return map(val, cal, 8192, 128, 255);
  }
  else {
    return map(val, 0, cal, 0, 127);
  }
}
