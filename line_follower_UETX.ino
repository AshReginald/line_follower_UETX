#include <AFMotor.h>

AF_DCMotor motor1(1); // Khai báo động cơ 1
AF_DCMotor motor2(2); // Khai báo động cơ 2

int sensorPins[8] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Đầu dò line

// Các hằng số PID
float kp = 1.5;   // Hệ số tỷ lệ
float ki = 0.1;   // Hệ số tích phân
float kd = 0.05;  // Hệ số vi phân

float targetPosition = 3.5;  // Vị trí mong muốn (vị trí trung tâm)
float currentPosition = 0;   // Vị trí hiện tại

float prevError = 0;         // Sai số trước đó
float integral = 0;          // Tích phân

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
  
  // Tính giá trị sai số
  float error = leftWeight - rightWeight;
  
  // Tính thành phần PID
  integral += error;
  float derivative = error - prevError;
  
  // Tính động cơ điều khiển từ PID
  float motorControl = kp * error + ki * integral + kd * derivative;
  
  // Giới hạn giá trị động cơ điều khiển
  motorControl = constrain(motorControl, -255, 255);
  
  // Tính tốc độ motor từ động cơ điều khiển
  int motorSpeed1 = 255 - (int)motorControl;
  int motorSpeed2 = 255 + (int)motorControl;
  
  // Tăng tốc độ lên tối đa khi xe đi thẳng
  if (error == 0) {
    motorSpeed1 = 255;
    motorSpeed2 = 255;
  }
  
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
  
  // Lưu giá trị sai số trước đó
  prevError = error;
}
