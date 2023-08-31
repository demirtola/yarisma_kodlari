#define PWM_FREQ  5000                              // PWM frekansi
#define PWMA_CH    0                                     // PWM kanali (0-15)
#define PWMB_CH    1                                     // PWM kanali (0-15)
#define PWM_RES    8                                     // PWM cozunurlugu (1-16 bits)

#define MOTA_PIN   PWM1                                   // D2
#define MOTA_DIR1  D15                                    // Motor yon secme pin1
#define MOTA_DIR2  D14                                    // Motor yon secme pin2

#define MOTB_PIN   PWM0                                   // D3
#define MOTB_DIR1  D13                                    // Motor yon secme pin1
#define MOTB_DIR2  D12                                   // Motor yon secme pin2

void setup() {
  delay(2000);
  // put your setup code here, to run once:
  pinMode(MOTA_PIN, OUTPUT);                             // Motor PWM pini cikis olarak ayarlandi
  pinMode(MOTA_DIR1, OUTPUT);                            // Motor yon secme pin1 cikis olarak ayarlandi
  pinMode(MOTA_DIR2, OUTPUT);                            // Motor yon secme pin2 cikis olarak ayarlandi

  pinMode(MOTB_PIN, OUTPUT);                             // Motor PWM pini cikis olarak ayarlandi
  pinMode(MOTB_DIR1, OUTPUT);                            // Motor yon secme pin1 cikis olarak ayarlandi
  pinMode(MOTB_DIR2, OUTPUT);                            // Motor yon secme pin2 cikis olarak ayarlandi

  ledcSetup(PWMA_CH, PWM_FREQ, PWM_RES);                // PWM zamanlayicisi ayarlandi
  ledcAttachPin(MOTA_PIN, PWMA_CH);       // Motor PWM pini ile zamanlayici iliskilendirildi

  ledcSetup(PWMB_CH, PWM_FREQ, PWM_RES);                // PWM zamanlayicisi ayarlandi
  ledcAttachPin(MOTB_PIN, PWMB_CH);       // Motor PWM pini ile zamanlayici iliskilendirildi

}

void loop() {
  digitalWrite(MOTA_DIR1, HIGH);        // motor ileri yönde hareket ediyor
  digitalWrite(MOTA_DIR2, LOW);
  digitalWrite(MOTB_DIR1, HIGH);        // motor ileri yönde hareket ediyor
  digitalWrite(MOTB_DIR2, LOW);

  motor(220, 0);
  delay(2000);
  motor(-220, 0);
  delay(2000);
  motor(0, 220);
  delay(2000);
  motor(0, -220);
  delay(2000);
  motor(0, 0);
  delay(5000);
}
bool motor_init_bool = false;
void motor(int sol, int sag) {
  if (motor_init_bool == false) {
    pinMode(MOTA_PIN, OUTPUT);                             // Motor PWM pini cikis olarak ayarlandi
    pinMode(MOTA_DIR1, OUTPUT);                            // Motor yon secme pin1 cikis olarak ayarlandi
    pinMode(MOTA_DIR2, OUTPUT);                            // Motor yon secme pin2 cikis olarak ayarlandi

    pinMode(MOTB_PIN, OUTPUT);                             // Motor PWM pini cikis olarak ayarlandi
    pinMode(MOTB_DIR1, OUTPUT);                            // Motor yon secme pin1 cikis olarak ayarlandi
    pinMode(MOTB_DIR2, OUTPUT);                            // Motor yon secme pin2 cikis olarak ayarlandi

    ledcSetup(PWMA_CH, PWM_FREQ, PWM_RES);
    ledcAttachPin(MOTA_PIN, PWMA_CH);

    ledcSetup(PWMB_CH, PWM_FREQ, PWM_RES);
    ledcAttachPin(MOTB_PIN, PWMB_CH);
    motor_init_bool = true;
  }

  if (sol > 0) {
    digitalWrite(MOTB_DIR1, HIGH);        // motor ileri yönde hareket ediyor
    digitalWrite(MOTB_DIR2, LOW);
  }
  else {
    digitalWrite(MOTB_DIR1, LOW);        // motor ileri yönde hareket ediyor
    digitalWrite(MOTB_DIR2, HIGH);
  }
  ledcWrite(PWMB_CH, abs(sol));

  if (sag > 0) {
    digitalWrite(MOTA_DIR1, HIGH);        // motor ileri yönde hareket ediyor
    digitalWrite(MOTA_DIR2, LOW);
  }
  else {
    digitalWrite(MOTA_DIR1, LOW);        // motor ileri yönde hareket ediyor
    digitalWrite(MOTA_DIR2, HIGH);
  }
  ledcWrite(PWMA_CH, abs(sag));
}
