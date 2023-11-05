#include <QTRSensors.h>

QTRSensors qtr;

const  uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

int Sensor_ADC_0[8];

bool bit_Sensor[8];

byte bin_Sensor = 0b00000000;
//----------------------------------------

//----------------------------------------Variable PIN for the L298N motor driver
int Motor_R = 12; //--> D11->IN4 (To adjust the direction of the left motor rotation)
int Motor_R_PWM = 11; //--> D10->IN3 (To set the left motor PWM value)
int Motor_L_PWM = 10; //--> D9->IN2 (To set the right motor PWM value)
int Motor_L = 8; //--> D8->IN1 (To adjust the direction of the right motor rotation)
//----------------------------------------

/*int ENA = 11;   
int IN1 = 13;
int IN2 = 12;
int ENB = 10;   
int IN3 = 9;
int IN4 = 8; 3-4-B(8-9-10) la banh trai*/

int PWM_Speed = 100; //--> Variable for the PWM value

//----------------------------------------Configuration variables for KP, KI and KD values
int Kp = 10;
int Ki = 2;
int Kd = 100;
//----------------------------------------

int error, last_error; //--> Variable to hold error value of the line sensor

//========================================================================VOID SETUP
void setup() {
  // put your setup code here, to run once:
  //qtr.setTypeRC();
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){5, 6, A5, A4, A3, A2, A1, A0}, SensorCount);
  qtr.setEmitterPin(2);

  //----------------------------------------Motor PIN as output
  pinMode(Motor_L, OUTPUT);
  pinMode(Motor_L_PWM, OUTPUT);
  pinMode(Motor_R_PWM, OUTPUT);
  pinMode(Motor_R, OUTPUT);
  //----------------------------------------

  Serial.begin(9600);
}
//========================================================================

//========================================================================VOID LOOP
void loop() {
  // put your main code here, to run repeatedly:


  //#################################################### PID CONTROLLER ####################################################

    int PWM_Max_R = PWM_Speed;
    int PWM_Max_L = PWM_Speed;
    
    check_Error(); //--> Call the check_Error () subroutine to get an Error value.

    //........................................PID calculations
    int P = error;
    int I = I + error;
    int D = error - last_error;
    int PID = (Kp * P) + (Ki * I) + (Kd * D); 
    //........................................

    //........................................Applying the result of PID calculation to the driving motors.
    int PWM_R = PWM_Max_R - PID;
    int PWM_L = PWM_Max_L + PID;
    PWM_R = PWM_R - PID;
    PWM_L = PWM_L + PID; 

    last_error = error;

    if (PWM_R >= 0) {
      right_Forward();
    } else {
      right_Backward();
    }

    if (PWM_L >= 0) {
      left_Forward();
    } else {
      left_Backward();
    }

    if (PWM_R < 0) PWM_R = 0 - PWM_R;
    if (PWM_L < 0) PWM_L = 0 - PWM_L;
    if (PWM_R > 255) PWM_R = 255;
    if (PWM_L > 255) PWM_L = 255;

    if (digitalRead(Motor_R) == LOW) {
      PWM_R = abs(PWM_R);
    } else {
      PWM_R = 255 - PWM_R;
    }

    if (digitalRead(Motor_L) == LOW) {
      PWM_L = abs(PWM_L);
    } else {
      PWM_L = 255 - PWM_L;
    }

    analogWrite(Motor_R_PWM, PWM_R);
    analogWrite(Motor_L_PWM, PWM_L);
    read();
    //........................................
  //########################################################################################################################
}



//========================================================================void check_Error()
// Subroutines to get error values for several conditions
void check_Error() {
  check_Sensor(); //--> Calling the check_Sensor () subroutine

  //----------------------------------------Filling the error value
  if (bin_Sensor == 0b00000001) error = 13;
  if (bin_Sensor == 0b00000011) error = 11;
  if (bin_Sensor == 0b00000010) error = 9;
  if (bin_Sensor == 0b00000110) error = 7;
  if (bin_Sensor == 0b00000100) error = 5;
  if (bin_Sensor == 0b00001100) error = 3;
  if (bin_Sensor == 0b00001000) error = 1;
  if (bin_Sensor == 0b00011000) error = 0;
  if (bin_Sensor == 0b00010000) error = -1;
  if (bin_Sensor == 0b00110000) error = -3;
  if (bin_Sensor == 0b00100000) error = -5;
  if (bin_Sensor == 0b01100000) error = -7;
  if (bin_Sensor == 0b01000000) error = -9;
  if (bin_Sensor == 0b11000000) error = -11;
  if (bin_Sensor == 0b10000000) error = -13;
  //----------------------------------------
}
//========================================================================

//========================================================================void right_Forward()
// Subroutine to make the right motor move forward
void right_Forward() {
  digitalWrite(Motor_R, LOW);
}
//========================================================================

//========================================================================void right_Backward()
// Subroutine to make the right motor move backward
void right_Backward() {
  digitalWrite(Motor_R, HIGH);
}
//========================================================================

//========================================================================void left_Forward()
// Subroutine to make the left motor move forward
void left_Forward() {
  digitalWrite(Motor_L, LOW);
}
//========================================================================

//========================================================================void left_Backward()
// Subroutine to make the left motor move backward
void left_Backward() {
  digitalWrite(Motor_L, HIGH);
}
//========================================================================
void go_straight() {
  digitalWrite(Motor_R, HIGH);
  digitalWrite(Motor_L, HIGH);
}

//========================================================================void check_Sensor()
// Subroutines to check sensors based on the calibration result value (ResultRef)
void check_Sensor() {
  read_Sensor(); //--> 

  //----------------------------------------Specifies a bit_Sensor array for HIGH or LOW
  for (byte i=0; i<=7; i++) {
    if (Sensor_ADC_0[i] > 950) {
      bit_Sensor[i] = HIGH;
    } else {
      bit_Sensor[i] = LOW;
    }
  }
  //----------------------------------------

  //----------------------------------------inserting the bit_Sensor array into the bin_Sensor binary
  for (byte i=0; i<=7; i++) {
    bitWrite(bin_Sensor, i, bit_Sensor[7-i]);
  }
  //----------------------------------------
}

//========================================================================void read_Sensor()
// Subroutines to read sensor values in the form of ADC values
void read_Sensor() {
  qtr.read(sensorValues);
  Sensor_ADC_0[0] = analogRead(5); 
  Sensor_ADC_0[1] = analogRead(6); 
  Sensor_ADC_0[2] = analogRead(A5); 
  Sensor_ADC_0[3] = analogRead(A4); 
  Sensor_ADC_0[4] = analogRead(A3); 
  Sensor_ADC_0[5] = analogRead(A2); 
  Sensor_ADC_0[6] = analogRead(A1); 
  Sensor_ADC_0[7] = analogRead(A0); 
}
//========================================================================
void read(){
  for (uint8_t i = 0; i < 8; i++)
  {
    Serial.print(Sensor_ADC_0[i]);
    Serial.print('\t');
  }
  Serial.print(bin_Sensor);
  Serial.print('\t');
  Serial.println();


  delay(250);
}