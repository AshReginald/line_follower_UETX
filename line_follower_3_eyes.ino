float output=0;
float   P = 0, I = 0, D=0;
float preErr = 0;
int left_pwm, right_pwm;
int Err;

#define LEFT_SPEED 120
#define RIGHT_SPEED 160
#define Kp 25
#define Ki 0.00001
#define Kd 0.25

#define RS A3
#define MS A2
#define LS A1
#define RF 5
#define RB 3
#define LF 9
#define LB 6
#define threshold 10
int input1;
int input2;
int input3;
float distance;
float duration;

#define trig 2
#define echo 4
void setup()
{
Serial.begin(9600);
pinMode(5,OUTPUT);
pinMode(3,OUTPUT);
pinMode(9,OUTPUT);
pinMode(6,OUTPUT);
pinMode(LS,INPUT);
pinMode(MS,INPUT);
pinMode(RS,INPUT);
pinMode(echo,INPUT);
pinMode(trig,OUTPUT);
Serial.begin(9600);

}
void FORWARD(){
  analogWrite(LF,200);
  analogWrite(LB,0);
  analogWrite(RF,200);
  analogWrite(RB,0);
}
void LEFT(){
  analogWrite(LF,0);
  analogWrite(LB,0);
  analogWrite(RF,200);
  analogWrite(RB,0);

} 
void STOP(){
   analogWrite(LF,0);
  analogWrite(LB,0);
  analogWrite(RF,0);
  analogWrite(RB,0);
}

void RIGHT(){
  analogWrite(LF,200);
  analogWrite(LB,0);
  analogWrite(RF,0);
  analogWrite(RB,0);

}

void lineFollowing(){  
input1=digitalRead(LS); 
input2=digitalRead(MS); // gan gia tri hong ngoai ben phai
input3=digitalRead(RS);
if(input1==1 && input2==0 && input3==0)    Err =-2;
    else if (input1==1 && input2==1 && input3==0)    Err =-1;
    else if (input1==0 && input2==1 && input3==0)    Err =0;
    else if (input1==0 && input2==1 && input3==1)    Err =1; 
    else if (input1==0 && input2==0 && input3==1)    Err =2;
    else if (input1==0 && input2==0 && input3==0){
        if(preErr>0) Err=3;
        else if(preErr<0) Err = -3;
        else Err = 0;
    }
 
    P = Err;  
    I = I+Err;
    D = Err-preErr;
    output = Kp*P+ Ki*I + Kd*D;
    left_pwm = LEFT_SPEED + output;
    right_pwm = RIGHT_SPEED - output;
    if (preErr!=Err) preErr = Err;
    analogWrite(LF,left_pwm);
    analogWrite(LB,0);
    analogWrite(RF,right_pwm);
    analogWrite(RB,0);
}
 void avoidObstacle(){
   analogWrite(LF,0);
    analogWrite(LB,0);
     analogWrite(RF,0);
      analogWrite(RB,0);
 }
void measureDistance(){
  
     digitalWrite(trig,LOW);   
    delayMicroseconds(2);
    digitalWrite(trig,HIGH);   
    delayMicroseconds(5);   
    digitalWrite(trig,LOW);   
    duration = pulseIn(echo,HIGH);  
    distance = duration/2*0.034;
    if(distance < threshold){
          // Nếu khoảng cách nhỏ hơn ngưỡng, thực hiện hành động tránh vật cản
    avoidObstacle();
  } else {
    // Nếu khoảng cách lớn hơn ngưỡng, thực hiện điều khiển dò line
    lineFollowing();
  }

    }
  
  void loop(){
    measureDistance();
  }