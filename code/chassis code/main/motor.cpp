#include "include.h"
#include <PID_v1.h>

bool rev1, rev2, rev3, rev4;
double M1PWMOUT, M2PWMOUT, M3PWMOUT, M4PWMOUT;//motor output
double in1, in2, in3, in4, M1S, M2S, M3S, M4S;//motor input
double Kp = 7, Ki = 2, Kd = 0; //PID
PID M1PID(&M1S, &M1PWMOUT, &ref1, Kp, Ki, Kd, DIRECT); //PID
PID M2PID(&M2S, &M2PWMOUT, &ref2, Kp, Ki, Kd, DIRECT);
PID M3PID(&M3S, &M3PWMOUT, &ref3, Kp, Ki, Kd, DIRECT);
PID M4PID(&M4S, &M4PWMOUT, &ref4, Kp, Ki, Kd, DIRECT);
struct motor
{
  int in1;
  int in2;
  int pwm;
  int A_out;
  int B_out;
};
motor M1, M2, M3, M4;
void macount()
{
  rev1 = digitalRead(M1.B_out);
  in1++;
}
void mbcount()
{
  rev2 = digitalRead(M2.B_out);
  in2++;
}
void mccount()
{
  rev3 = !digitalRead(M3.B_out);
  in3++;
}
void mdcount()
{
  rev4 = !digitalRead(M4.B_out);
  in4++;
}
void moving()
{
  if (M1PWMOUT > 0)
  {
    analogWrite(M1.pwm, M1PWMOUT);
    digitalWrite(M1.in1, 1);
    digitalWrite(M1.in2, 0);
  }
  else
  {
    analogWrite(M1.pwm, abs(M1PWMOUT));
    digitalWrite(M1.in1, 0);
    digitalWrite(M1.in2, 1);
  }
  if (M2PWMOUT > 0)
  {
    analogWrite(M2.pwm, M2PWMOUT);
    digitalWrite(M2.in1, 1);
    digitalWrite(M2.in2, 0);
  }
  else
  {
    analogWrite(M2.pwm, abs(M2PWMOUT));
    digitalWrite(M2.in1, 0);
    digitalWrite(M2.in2, 1);
  }
  if (M3PWMOUT > 0)
  {
    analogWrite(M3.pwm, M3PWMOUT);
    digitalWrite(M3.in1, 1);
    digitalWrite(M3.in2, 0);
  }
  else
  {
    analogWrite(M3.pwm, abs(M3PWMOUT));
    digitalWrite(M3.in1, 0);
    digitalWrite(M3.in2, 1);
  }
  if (M4PWMOUT > 0)
  {
    analogWrite(M4.pwm, M4PWMOUT);
    digitalWrite(M4.in1, 1);
    digitalWrite(M4.in2, 0);
  }
  else
  {
    analogWrite(M4.pwm, abs(M4PWMOUT));
    digitalWrite(M4.in1, 0);
    digitalWrite(M4.in2, 1);
  }
  //Serial.println("moved");
}
void stop_all()
{
  digitalWrite(M1.in1, 1);
  digitalWrite(M2.in1, 1);
  digitalWrite(M3.in1, 1);
  digitalWrite(M4.in1, 1);
  digitalWrite(M1.in2, 1);
  digitalWrite(M2.in2, 1);
  digitalWrite(M3.in2, 1);
  digitalWrite(M4.in2, 1);
  M1PWMOUT = 0;
  M2PWMOUT = 0;
  M3PWMOUT = 0;
  M4PWMOUT = 0;
  ref1 = 0;
  ref2 = 0;
  ref3 = 0;
  ref4 = 0;
}
void fail_safe()
{
  stop_all();
}
void GetM1Speed()
{
  if (rev1)
    M1S = in1;
  else
    M1S = -in1;
  in1 = 0;
}
void GetM2Speed()
{
  if (rev2)
    M2S = in2;
  else
    M2S = -in2;
  in2 = 0;
}
void GetM3Speed()
{
  if (rev3)
    M3S = in3;
  else
    M3S = -in3;
  in3 = 0;
}
void GetM4Speed()
{
  if (rev4)
    M4S = in4;
  else
    M4S = -in4;
  in4 = 0;
}
void Speed()
{
  GetM1Speed();
  GetM2Speed();
  GetM3Speed();
  GetM4Speed();
}
void setspeed()
{
  M1PID.Compute();
  M2PID.Compute();
  M3PID.Compute();
  M4PID.Compute();
}
void PrintSpeed()
{
  Serial.println(M1S);
  Serial.println(M2S);
  Serial.println(M3S);
  Serial.println(M4S);
  Serial.println("==========");
}
void graph()
{
  Serial.print("reference:");
  Serial.print(ref1);
  Serial.print(",");
  Serial.print("speed:");
  Serial.println(M1S);
}
void InitMotor()
{
  M1.A_out = 18;
  M2.A_out = 19;
  M3.A_out = 20;
  M4.A_out = 21;
  M1.B_out = 39;
  M2.B_out = 36;
  M3.B_out = 34;
  M4.B_out = 32;
  M1.in1 = 40;
  M1.in2 = 42;
  M2.in1 = 44;
  M2.in2 = 46;
  M3.in1 = 48;
  M3.in2 = 50;
  M4.in1 = 51;
  M4.in2 = 52;
  M1.pwm = 2;
  M2.pwm = 3;
  M3.pwm = 4;
  M4.pwm = 5;
  pinMode(M1.pwm, OUTPUT);
  pinMode(M2.pwm, OUTPUT);
  pinMode(M3.pwm, OUTPUT);
  pinMode(M4.pwm, OUTPUT);
  pinMode(M1.in1, OUTPUT);
  pinMode(M1.in2, OUTPUT);
  pinMode(M2.in1, OUTPUT);
  pinMode(M2.in2, OUTPUT);
  pinMode(M3.in1, OUTPUT);
  pinMode(M3.in2, OUTPUT);
  pinMode(M4.in1, OUTPUT);
  pinMode(M4.in2, OUTPUT);
  pinMode(M1.B_out, INPUT);
  pinMode(M2.B_out, INPUT);
  pinMode(M3.B_out, INPUT);
  pinMode(M4.B_out, INPUT);
  pinMode(M1.A_out, INPUT);
  pinMode(M2.A_out, INPUT);
  pinMode(M3.A_out, INPUT);
  pinMode(M4.A_out, INPUT);
  /************/
  attachInterrupt(digitalPinToInterrupt(M1.A_out), macount, FALLING);
  attachInterrupt(digitalPinToInterrupt(M2.A_out), mbcount, FALLING);
  attachInterrupt(digitalPinToInterrupt(M3.A_out), mccount, FALLING);
  attachInterrupt(digitalPinToInterrupt(M4.A_out), mdcount, FALLING);
  M1PID.SetMode(AUTOMATIC);
  M1PID.SetSampleTime(30);
  M2PID.SetMode(AUTOMATIC);
  M2PID.SetSampleTime(30);
  M3PID.SetMode(AUTOMATIC);
  M3PID.SetSampleTime(30);
  M4PID.SetMode(AUTOMATIC);
  M4PID.SetSampleTime(30);
  M1PID.SetOutputLimits(-255, 255);
  M2PID.SetOutputLimits(-255, 255);
  M3PID.SetOutputLimits(-255, 255);
  M4PID.SetOutputLimits(-255, 255);
}
