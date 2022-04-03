#include <FutabaSBUS.h>
FutabaSBUS sbus;
int rcsig[25];
void setup() {
  // put your setup code here, to run once:
   sbus.begin(Serial2);
   sbus.attachDataReceived(dataReceived);
   Serial.begin(9600);
}
void dataReceived(ChannelData channels) {
        // do something with the data
        for(int i=1;i<=16;i++)
        {
          rcsig[i]=channels.data[i-1];
        }
        rcsig[17]=channels.channels.channel17;
        rcsig[18]=channels.channels.channel18;
}
void loop() {
  // put your main code here, to run repeatedly:
 sbus.receive();
 Serial.println(rcsig[1]);
}
