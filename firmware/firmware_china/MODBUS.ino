#include <ModbusRTU.h>
#include <HardwareSerial.h>
#define SLAVE_ID 1  //从机ID

//引脚定义
#define SWITCH0 23     //继电器1引脚
#define SWITCH1 5      //继电器2引脚
#define SWITCH2 4      //继电器3引脚
#define SWITCH3 13     //继电器引4脚
#define SWITCH_IN0 25  //光耦1输入
#define SWITCH_IN1 26  //光耦2输入
#define SWITCH_IN2 27  //光耦3输入
#define SWITCH_IN3 33  //光耦4输入
#define RXTX_PIN 32    //485的输入输出控制引脚
ModbusRTU mb;
#define MBUS_TXD_PIN 19  //485对应串口的引脚
#define MBUS_RXD_PIN 18
HardwareSerial mySerial(1);  //MODBUS应用于串口1
void setup() {
  delay(100);
  //串口0初始化
  Serial.begin(115200, SERIAL_8N1);
  //串口1初始化
  mySerial.begin(115200, SERIAL_8N1, MBUS_RXD_PIN, MBUS_TXD_PIN);  //RX1 18 TX1 19
  //MODBUS初始化
  mb.begin(&mySerial, RXTX_PIN);
  mb.slave(SLAVE_ID);
  mb.addCoil(0, false);  //添加地址，用到哪些地址设备都是需要添加的
  mb.addCoil(1, false);  //支持功能码保持寄存器 03 06 10 这里采用10写入多路寄存器实现4路继电器开关
  mb.addCoil(2, false);  //支持功能码保持寄存器 03 06 10 这里采用10写入多路寄存器实现4路继电器开关
  mb.addCoil(3, false);  //支持功能码保持寄存器 03 06 10 这里采用10写入多路寄存器实现4路继电器开关

  //
  pinMode(SWITCH0, OUTPUT);
  pinMode(SWITCH1, OUTPUT);
  pinMode(SWITCH2, OUTPUT);
  pinMode(SWITCH3, OUTPUT);
  //
  pinMode(SWITCH_IN0, INPUT);
  pinMode(SWITCH_IN1, INPUT);
  pinMode(SWITCH_IN2, INPUT);
  pinMode(SWITCH_IN3, INPUT);
  Serial.println("init ok");
}

void loop() {
  if (mb.Coil(0) == 0) {
    digitalWrite(SWITCH0, HIGH);
  } else {
    digitalWrite(SWITCH0, LOW);
  }
  if (mb.Coil(1) == 0) {
    digitalWrite(SWITCH1, HIGH);
  } else {
    digitalWrite(SWITCH1, LOW);
  }
  if (mb.Coil(2) == 0) {
    digitalWrite(SWITCH2, HIGH);
  } else {
    digitalWrite(SWITCH2, LOW);
  }
  if (mb.Coil(3) == 0) {
    digitalWrite(SWITCH3, HIGH);
  } else {
    digitalWrite(SWITCH3, LOW);
  }

  if (digitalRead(SWITCH_IN0) == 0) {
    mb.Coil(0, true);
  }
  if (digitalRead(SWITCH_IN1) == 0) {
    mb.Coil(1, true);
  }
  if (digitalRead(SWITCH_IN2) == 0) {
    mb.Coil(2, true);
  }
  if (digitalRead(SWITCH_IN3) == 0) {
    mb.Coil(3, true);
  }
  switch (KeyEvent()) {                //按键上下沿触发
    case 1: mb.Coil(0, false); break;  //当光耦输入口为低电平
    case -1: mb.Coil(0, true); break;  //当光耦输入口为高电平
    case 2: mb.Coil(1, false); break;  //当光耦输入口为低电平
    case -2: mb.Coil(1, true); break;  //当光耦输入口为高电平
    case 3: mb.Coil(2, false); break;  //当光耦输入口为低电平
    case -3: mb.Coil(2, true); break;  //当光耦输入口为高电平
    case 4: mb.Coil(3, false); break;  //当光耦输入口为低电平
    case -4: mb.Coil(3, true); break;  //当光耦输入口为高电平
  }
  mb.task();
}

int KeyEvent() {
  static int key[4] = {0}, keyLast[4] = {0};
  key[0] = digitalRead(SWITCH_IN0);
  if (key[0] != keyLast[0]) {
    keyLast[0] = key[0];
    if (key[0])
      return 1;
    else
      return -1;
  }
  key[1] = digitalRead(SWITCH_IN1);
  if (key[1] != keyLast[1]) {
    keyLast[1] = key[1];
    if (key[1])
      return 2;
    else
      return -2;
  }
  key[2] = digitalRead(SWITCH_IN2);
  if (key[2] != keyLast[2]) {
    keyLast[2] = key[2];
    if (key[2])
      return 3;
    else
      return -3;
  }
  key[3] = digitalRead(SWITCH_IN3);
  if (key[3] != keyLast[3]) {
    keyLast[3] = key[3];
    if (key[3])
      return 4;
    else
      return -4;
  }
  return 0;
}