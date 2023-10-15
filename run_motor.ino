// Khai báo chân cho module L298N cho động cơ 1
int ENA1 = 10;   // Chân enable động cơ 1
int IN1 = 8;
int IN2 = 9;

// Khai báo chân cho module L298N cho động cơ 2
int ENA2 = 11;   // Chân enable động cơ 2
int IN3 = 12;
int IN4 = 13;

// Khai báo chân cho encoder động cơ 1
int encoderPinA1 = 2;  // Kết nối với dây C1 của encoder động cơ 1
int encoderPinB1 = 3;  // Kết nối với dây C2 của encoder động cơ 1

// Khai báo chân cho encoder động cơ 2
int encoderPinA2 = 4;  // Kết nối với dây C1 của encoder động cơ 2
int encoderPinB2 = 7;  // Kết nối với dây C2 của encoder động cơ 2

volatile long encoderCount1 = 0;  // Số xung từ encoder động cơ 1
volatile long encoderCount2 = 0;  // Số xung từ encoder động cơ 2

void setup() {
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

void loop() {
   // Điều khiển động cơ 1
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA1, 200);  // Điều chỉnh tốc độ 0-255

  // Điều khiển động cơ 2
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA2, 200);  // Điều chỉnh tốc độ 0-255

  // Đọc giá trị encoder cho động cơ 1 và động cơ 2
  long count1 = encoderCount1;
  long count2 = encoderCount2;

  // Hiển thị giá trị encoder trên Serial Monitor
  Serial.print("Encoder 1: ");
  Serial.print(count1);
  Serial.print("\tEncoder 2: ");
  Serial.println(count2);

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
