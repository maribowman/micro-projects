#define MOTION_PIN D4

void setup() {
  Serial.begin(115200);
  pinMode(MOTION_PIN, INPUT);
}

void loop() {
  if (digitalRead(MOTION_PIN) == LOW) {
    Serial.println("off");
    delay(1500);
  } else {
    Serial.println("on");
    delay(3000);
  }
  
}
