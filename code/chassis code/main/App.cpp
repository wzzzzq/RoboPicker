#include "include.h"

const int L = 1020, R = 1030;
int mafor, mago, maturn;
int mbfor, mbgo, mbturn;
int mcfor, mcgo, mcturn;
int mdfor, mdgo, mdturn;
int speed_max = 110;
int speed_min = -110;
double ref1, ref2, ref3, ref4;
unsigned long t, T;

void TaskRun(void)
{
  GetSignal();
  if (millis() > t)
  {
    Speed();
    t =  millis() + 30;
  }
  if (rcsig[1] == 0)
    fail_safe();
  else
  {
    if(rcsig[5]>1024) digitalWrite(53,1);
    else digitalWrite(53,0);
    if ((rcsig[1] > L) && (rcsig[1] < R) && (rcsig[2] > L) && (rcsig[2] < R) && (rcsig[4] > L) && (rcsig[4] < R))
      stop_all();
    else
    {
      if (!((rcsig[2] > L) && (rcsig[2] < R)))
      {
        mafor = map(rcsig[2], 340, 1708, speed_min, speed_max);
        mbfor = map(rcsig[2], 340, 1708, speed_min, speed_max);
        mcfor = map(rcsig[2], 340, 1708, speed_min, speed_max);
        mdfor = map(rcsig[2], 340, 1708, speed_min, speed_max);
      }
      else
      {
        mafor = 0;
        mbfor = 0;
        mcfor = 0;
        mdfor = 0;
      }
      if (!((rcsig[1] > L) && (rcsig[1] < R)))
      {
        mago = map(rcsig[1], 340, 1708, speed_min, speed_max);
        mbgo = map(rcsig[1], 340, 1708, speed_max, speed_min);
        mcgo = map(rcsig[1], 340, 1708, speed_min, speed_max);
        mdgo = map(rcsig[1], 340, 1708, speed_max, speed_min);
      }
      else
      {
        mago = 0;
        mbgo = 0;
        mcgo = 0;
        mdgo = 0;
      }
      if (!((rcsig[4] > L) && (rcsig[4] < R)))
      {
        maturn = map(rcsig[4], 340, 1708, speed_min, speed_max);
        mbturn = map(rcsig[4], 340, 1708, speed_min, speed_max);
        mcturn = map(rcsig[4], 340, 1708, speed_max, speed_min);
        mdturn = map(rcsig[4], 340, 1708, speed_max, speed_min);
      }
      else
      {
        maturn = 0;
        mbturn = 0;
        mcturn = 0;
        mdturn = 0;
      }
      ref1 = max(min(mafor + mago + maturn, speed_max), speed_min);
      ref2 = max(min(mbfor + mbgo + mbturn, speed_max), speed_min);
      ref3 = max(min(mcfor + mcgo + mcturn, speed_max), speed_min);
      ref4 = max(min(mdfor + mdgo + mdturn, speed_max), speed_min);
      setspeed();
      moving();
    }
  }
}
