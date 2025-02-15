#include <Servo.h>                // 声明调用Servo.h库
#include <PID_v1.h>
#define  SERVO_NUM  6             //舵机数量
Servo myservo[SERVO_NUM];         //创建六个舵机类
const  byte servo_pin[SERVO_NUM] = {10, A2, A3, A0, A1, 7}; //宏定义舵机控制引脚

int data[10], cnt, l, num;

int ServoPwmDuty[8] = {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500}; //PWM脉冲宽度
int ServoPwmDutySet[8] = {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500}; //PWM脉冲宽度
double ServoPwmDutyInc[8];   //为了速度控制，当PWM脉宽发生变化时，每2.5ms或20ms递增的PWM脉宽

double cx = 93, cy = 61, Kpx = 0.2, Kpy = 0.4,  xnow, ynow;
double angle[10] = {1500, 1500, 1800, 800, 1500, 1500, 500};

bool ServoPwmDutyHaveChange = 0;  //脉宽有变化标志位
bool found = 0;

int ServoTime = 5000;      //舵机从当前角度运动到指定角度的时间，也就是控制速度

unsigned long t, T;

void ServoSetPluseAndTime(int id, int p, int time)
{
  if (id >= 0 && id < 6 && p >= 500 && p <= 2500)
  {
    if (time < 20)
      time = 20;
    if (time > 30000)
      time = 30000;
    ServoPwmDutySet[id] = p;
    ServoTime = time;
    ServoPwmDutyHaveChange = 1;
  }
}

void ServoPwmDutyCompare(void)//脉宽变化比较及速度控制
{
  int i;

  static int ServoPwmDutyIncTimes; //需要递增的次数
  static bool ServoRunning = 0; //舵机正在以指定速度运动到指定的脉宽对应的位置
  if (ServoPwmDutyHaveChange) //停止运动并且脉宽发生变化时才进行计算      ServoRunning == 0 &&
  {
    ServoPwmDutyHaveChange = 0;
    ServoPwmDutyIncTimes = ServoTime / 20; //当每20ms调用一次ServoPwmDutyCompare()函数时用此句
    for (i = 0; i < 6; i++)
    {
      //if(ServoPwmDuty[i] != ServoPwmDutySet[i])
      {
        if (ServoPwmDutySet[i] > ServoPwmDuty[i])
        {
          ServoPwmDutyInc[i] = ServoPwmDutySet[i] - ServoPwmDuty[i];
          ServoPwmDutyInc[i] = -ServoPwmDutyInc[i];
        }
        else
        {
          ServoPwmDutyInc[i] = ServoPwmDuty[i] - ServoPwmDutySet[i];
        }
        ServoPwmDutyInc[i] /= ServoPwmDutyIncTimes;//每次递增的脉宽
      }
    }
    ServoRunning = 1;  //舵机开始动作
  }
  if (ServoRunning)
  {
    ServoPwmDutyIncTimes--;
    for (i = 0; i < 6; i++)
    {
      if (ServoPwmDutyIncTimes == 0)
      { //最后一次递增就直接将设定值赋给当前值
        ServoPwmDuty[i] = ServoPwmDutySet[i];
        ServoRunning = 0; //到达设定位置，舵机停止运动
      }
      else
      {
        ServoPwmDuty[i] = ServoPwmDutySet[i] + (signed short int)(ServoPwmDutyInc[i] * ServoPwmDutyIncTimes);
      }
      myservo[i].writeMicroseconds(ServoPwmDuty[i]);
    }
  }
}
void GetData()
{
  found = 0;
  if (Serial.available())
  {
    found = 1;
    char input = Serial.read();
    while (input != '[' && input != -1)   input = Serial.read();
    if (input != -1)
    {
      String s = Serial.readStringUntil(']'), numstr = "";
      l = s.length(), cnt = 0;
      for (int i = 0; i < l; i++)
      {
        if (isDigit(s[i]))
          numstr += s[i];
        if (s[i] == ',' || i == l - 1)
        {
          data[++cnt] = numstr.toInt();
          numstr = "";
        }
      }
    }
  }
  xnow = data[1];
  ynow = data[2];
}
void PrintData()
{
  for (int i = 1; i <= cnt; i++)
  {
    Serial.println(data[i]);
  }
  Serial.println("==========");
}

void compute_x(int ref, int real)
{
  double error = 0;
  error = ref - real;
  angle[1] += Kpx * error;
  angle[1] = min(2500, angle[1]);
  angle[1] = max(500, angle[1]);
}

void compute_y(int ref, int real)
{
  double error = 0;
  error = ref - real;
  angle[2] += Kpy * error;
  angle[2] = min(2500, angle[2]);
  angle[2] = max(500, angle[2]);
  if (angle[2] > 1500) angle[4] = angle[2] - 1000;
  //else angle[3] = angle[2];
}

void aim()
{
  if (abs(xnow - cx) > 2) compute_x(cx, xnow);
  if (abs(ynow - cy) > 2) compute_y(cy, ynow);
  if (data[3] >= 7 && data[3] <= 9)  angle[6] = 1000;
  else  angle[6] = 500;
}
void def()
{
  angle[1] = angle[5] = angle[7] = 1500;
  angle[2] = 1800;
  angle[3] = 1500;
  angle[4] = 800;
}

void setup() {
  //put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3, INPUT);
  for (byte i = 0; i < SERVO_NUM; i++ ) {
    myservo[i].attach(servo_pin[i]);    // 将10引脚与声明的舵机对象连接起来
  }
  def();
  t = millis() + 20;
  T = millis() + 300;
}

void loop() {
  //put your main code here, to run repeatedly:
  GetData();
  if (millis() > t)
  {
    if (digitalRead(3)) angle[6] = 500;
    if (ServoPwmDuty[5] == 1000)
    {
      def();
    }
    if (ServoPwmDuty[5] == 500)
    {
      if (!found) num++;
      else num = 0;
      if (num < 10) aim();
      ///Serial.println(1);
    }
    for (int i = 0; i < 6; i++)  ServoSetPluseAndTime(i, angle[i + 1], 60);
    ServoPwmDutyCompare();
    t = millis() + 20;
  }
  //PrintData();
  //Serial.println(ServoPwmDuty[5]);
  //Serial.println(gripped);
}
