#include "include.h"
#include <FutabaSBUS.h>
int rcsig[25];
FutabaSBUS sbus;
void dataReceived(ChannelData channels) 
{
  // do something with the data
  for (int i = 1; i <= 16; i++)
  {
    rcsig[i] = channels.data[i - 1];
  }
  rcsig[17] = channels.channels.channel17;
  rcsig[18] = channels.channels.channel18;
}
void InitSbus()
{
  sbus.begin(Serial2);
  sbus.attachDataReceived(dataReceived);
}
void GetSignal()
{
  sbus.receive();
}
void PrintSignal()
{
  for(int i=1;i<=16;i++)
  {
    Serial.println(rcsig[i]);
  }
  Serial.println("-----------");
}
