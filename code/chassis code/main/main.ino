#include "include.h"

void setup()
{
  pinMode(53,OUTPUT);
  InitSbus();
  InitMotor();
  Serial.begin(9600);
  t = millis() + 30;
}

void loop()
{
  TaskRun(); 
}
