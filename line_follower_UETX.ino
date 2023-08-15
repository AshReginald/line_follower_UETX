#include <AFMotor.h>

AF_DCMotor motor1(1); // Khai báo động cơ 1
AF_DCMotor motor2(2); // Khai báo động cơ 2

int sensorPins[8] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Đầu dò line

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 8; i++) {
    pinMode(sensorPins[i], INPUT);
  }
}

void loop() {
  int sensorValues[8];
  
  // Đọc giá trị từ các đầu dò
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = digitalRead(sensorPins[i]);
  }
  
  // Kiểm tra vị trí của dòng line
  int leftWeight = 0, rightWeight = 0;
  for (int i = 0; i < 8; i++) {
    if (sensorValues[i] == LOW) {
      leftWeight += (i + 1);
      rightWeight += (8 - i);
    }
  }
  
  // Tính giá trị điều khiển motor dựa trên trọng số của dòng line
  int motorSpeed1 = 255 - (leftWeight * 15);
  int motorSpeed2 = 255 - (rightWeight * 15);
  
  // Giới hạn tốc độ motor
  motorSpeed1 = constrain(motorSpeed1, 0, 255);
  motorSpeed2 = constrain(motorSpeed2, 0, 255);
  
  // Điều khiển động cơ
  motor1.setSpeed(motorSpeed1);
  motor2.setSpeed(motorSpeed2);
  
  // Đảo chiều động cơ dựa trên trọng số
  if (leftWeight > rightWeight) {
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
  } else if (rightWeight > leftWeight) {
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
  } else {
    motor1.run(FORWARD);
    motor2.run(FORWARD);
  }
}
