#ifndef _MOTOR_H_
#define _MOTOR_H_

void macount();
void mbcount();
void mccount();
void mdcount();
void moving();
void stop_all();
void fail_safe();
void GetM1Speed();
void GetM2Speed();
void GetM3Speed();
void GetM4Speed();
void Speed();
void setspeed();
void InitMotor();
void PrintSpeed();
void graph();
extern double M1PWMOUT, M2PWMOUT, M3PWMOUT, M4PWMOUT;//motor output
extern double in1, in2, in3, in4, M1S, M2S, M3S, M4S;//motor input

#endif
