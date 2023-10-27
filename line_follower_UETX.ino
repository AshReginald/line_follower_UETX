
#include <QTRSensors.h>

QTRSensors qtr;
// Khai báo chân cho module L298N cho động cơ 1
int ENA1 = 10;   // Chân enable động cơ 1
int IN1 = 8;
int IN2 = 9;

// Khai báo chân cho module L298N cho động cơ 2
int ENA2 = 11;   // Chân enable động cơ 2
int IN3 = 12;
int IN4 = 13;

float output=0;
float   P = 0, I = 0, D=0;
float preErr = 0;
int left_pwm, right_pwm;
int Err;

#define LEFT_SPEED 120
#define RIGHT_SPEED 160

// Các hằng số PID
float kp = 1.5;   // Hệ số tỷ lệ
float ki = 0.1;   // Hệ số tích phân
float kd = 0.05;  // Hệ số vi phân

float targetPosition = 3.5;  // Vị trí mong muốn (vị trí trung tâm)
float currentPosition = 0;   // Vị trí hiện tại

float prevError = 0;         // Sai số trước đó
float integral = 0;          // Tích phân

// Khai báo chân cho encoder động cơ 1
int encoderPinA1 = 2;  // Kết nối với dây C1 của encoder động cơ 1
int encoderPinB1 = 3;  // Kết nối với dây C2 của encoder động cơ 1

// Khai báo chân cho encoder động cơ 2
int encoderPinA2 = 4;  // Kết nối với dây C1 của encoder động cơ 2
int encoderPinB2 = 7;  // Kết nối với dây C2 của encoder động cơ 2

volatile long encoderCount1 = 0;  // Số xung từ encoder động cơ 1
volatile long encoderCount2 = 0;  // Số xung từ encoder động cơ 2


const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

void setup()
{
  // configure the sensors
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){5, 6, A5, A4, A3, A2, A1, A0}, SensorCount);
  qtr.setEmitterPin(2);
  

  Serial.begin(9600);
  pinMode(ENA1, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(encoderPinA1, INPUT);
  pinMode(encoderPinB1, INPUT);
  pinMode(encoderPinA2, INPUT);
  pinMode(encoderPinB2, INPUT);

  attachInterrupt(digitalPinToInterrupt(encoderPinA1), updateEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinA2), updateEncoder2, CHANGE);
}
void FORWARD(){
  analogWrite(IN1,200);
  analogWrite(IN2,0);
  analogWrite(IN3,0);
  analogWrite(IN4,200);
  analogWrite(ENA1, 200);
  analogWrite(ENA2, 200);
}
void LEFT(){
  analogWrite(IN1,0);
  analogWrite(IN2,0);
  analogWrite(IN3,200);
  analogWrite(IN4,0);
}
void RIGHT (){
  analogWrite(IN1,200);
  analogWrite(IN2,0);
  analogWrite(IN3,0);
  analogWrite(IN4,0);
}
void STOP(){
  analogWrite(IN1,0);
  analogWrite(IN2,0);
  analogWrite(IN3,0);
  analogWrite(IN4,0);
}
void loop()
{
  int pin[9];
  // read raw sensor values
  qtr.read(sensorValues);

  // print the sensor values as numbers from 0 to 1023, where 0 means maximum
  // reflectance and 1023 means minimum reflectance
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]);
    pin[i] = sensorValues[i];
    Serial.print('\t');
  }
  Serial.println();

  delay(250);
  if(pin[0] > 900 && pin[1] > 900 && pin[2] > 900 && pin[3] > 900 && pin[4] > 900 && pin[5] > 900 && pin[6] > 900 && pin[7] > 900) {
     FORWARD();
  }
  else if(pin[0] < 900 && pin[1] < 900) {
     RIGHT();
  }
  else if(pin[6] < 900 && pin[7] < 900) {
     LEFT();
  }
  analogWrite(ENA1, 200);
  analogWrite(ENA2, 200);
  // Đọc giá trị encoder cho động cơ 1 và động cơ 2
  long count1 = encoderCount1;
  long count2 = encoderCount2;


  // Thực hiện xử lý dữ liệu encoder ở đây

  delay(100);  // Đọc giá trị encoder mỗi 100ms
}

void updateEncoder1() {
  if (digitalRead(encoderPinB1) == LOW) {
    encoderCount1++;
  } else {
    encoderCount1--;
  }
}

void updateEncoder2() {
  if (digitalRead(encoderPinB2) == LOW) {
    encoderCount2++;
  } else {
    encoderCount2--;
  }
}
