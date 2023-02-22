#define MR1 D8
#define MR2 D7
#define ML1 D10
#define ML2 D9
#define lE D0
#define lT D1
#define rE D2
#define rT D3
#define fE D4
#define fT D5

#include <PID_v1.h>
double Setpoint, Input, Output;
double kp = 4.4;
double ki = 0.05;
double kd = 0.82;
PID pid(&Input, &Output, &Setpoint, kp, ki, kd, DIRECT);


int Vl = 100;
int Vr = 100;

int r,l,f;
bool r_av,l_av,f_av;

void setup() {
  // put your setup code here, to run once:
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(lT, OUTPUT);
  pinMode(lE, INPUT);
  pinMode(rT, OUTPUT);
  pinMode(rE, INPUT);
  pinMode(fT, OUTPUT);
  pinMode(fE, INPUT);

  Setpoint = 5;
  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(-70,155);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
r = distance(rE,rT);
r_av = Available(r);
delay(5);
l = distance(lE,lT) - 2 ;
l_av = Available(l);
delay(5);
f = distance(fE,fT)+3;
f_av = Available(f);
delay(5);
Serial.print(r);
Serial.print(" xx ");
Serial.println(l);
if (r_av == LOW){
  kp = 4.6;
  ki = 0.05;
  kd = 0.85;
  Setpoint = 6;
  Input = r;
  pid.Compute();
  forward(Vr+Output,Vl-Output);
  if (f_av == LOW){
  Setpoint = 10;
  Input = r;
  pid.Compute();
  left(Vr,Vl-Output);
  if (l_av == LOW){
    Setpoint = 12;
    Input = r;
    pid.Compute();
    left(Vr+Output,Vl-Output);
    }
    }
  }
else if (l_av == LOW){
  kp = 4.9;
  ki = 0.05;
  kd = 0.82;
  Setpoint = 8;
  Input = l;
  pid.Compute();
  forward(Vr-Output,Vl+Output);
  if (f_av == LOW){
  Setpoint = 12;
  Input = l;
  pid.Compute();
  right(Vr-Output,Vl);
  }
}
else if (f_av == LOW){
  Setpoint = 10;
  Input = r;
  pid.Compute();
  left(Vr,Vl-Output);
}
else{
  forward(Vr+Output,Vl-Output);
  }
delay(50);
}

void forward(int r,int l) {
  analogWrite(MR1, r);
  analogWrite(MR2, 0);
  analogWrite(ML1, l);
  analogWrite(ML2, 0);
}

void right(int r,int l) {
  analogWrite(MR1, 0);
  analogWrite(MR2, r);
  analogWrite(ML1, l);
  analogWrite(ML2, 0);
}

void left(int r,int l) {
  analogWrite(MR1, r);
  analogWrite(MR2, 0);
  analogWrite(ML1, 0);
  analogWrite(ML2, l);
}

void backward(int r,int l) {
  analogWrite(MR1, 0);
  analogWrite(MR2, r);
  analogWrite(ML1, 0);
  analogWrite(ML2, l);
}

void Stop() {
  analogWrite(MR1, 0);
  analogWrite(MR2, 0);
  analogWrite(ML1, 0);
  analogWrite(ML2, 0);
}

int distance(int echoPin, int trigPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
long duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  int distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  return distance;
  Serial.print(distance);
  Serial.println(" cm");
  }

bool Available(int distance){
  int avl;
  if (distance > 18){avl = 1;}
  else {avl = 0;}
  return avl;
  }
