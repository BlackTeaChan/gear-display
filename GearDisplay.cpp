#include <stdint.h>

// #include <SPI.h>
// #include <Adafruit_GFX.h>
// #include <Max72xxPanel.h>
#include "GearDisplay.h"

// *************************** 矩阵屏图案 ***************************
// LOGO
// BTC（大）
const uint8_t LED_LOGO1[8] PROGMEM = {0x00, 0x1C, 0xC8, 0xAB, 0xCA, 0xAA, 0xC3, 0x00};
// BTC（小）
const uint8_t LED_LOGO2[8] PROGMEM = {0x00, 0x1C, 0xC8, 0xAB, 0xCA, 0xA3, 0xC0, 0x00};
// TEA
const uint8_t LED_LOGO3[8] PROGMEM = {0x3E, 0x08, 0x08, 0xE8, 0x82, 0xE5, 0x87, 0xE5};
// CHC
const uint8_t LED_LOGO4[8] PROGMEM = {0xF5, 0x97, 0x85, 0x80, 0x80, 0x87, 0x94, 0xF7};
// H
const uint8_t LED_LOGO5[8] PROGMEM = {0x66, 0x66, 0x66, 0x7E, 0x7E, 0x66, 0x66, 0x66};

// 空
const uint8_t LED_EMPTY[8] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// 爱心
const uint8_t LED_HEART[2][8] PROGMEM = {
  // 小
  { 0x00, 0x00, 0x24, 0x7E, 0x7E, 0x3C, 0x18, 0x00 },
  // 大
  { 0x00, 0x66, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C, 0x18 }
};
// 挡位
const uint8_t LED_GEAR[8][8] PROGMEM = {
  // N
  { 0xC3, 0xE3, 0xF3, 0xDB, 0xDB, 0xCF, 0xC7, 0xC3 },
  // 1
  { 0x18, 0x38, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18 },
  // 2
  { 0x38, 0x7E, 0x06, 0x06, 0x1C, 0x30, 0x7E, 0x7E },
  // 3
  { 0x7C, 0x7E, 0x06, 0x3C, 0x3C, 0x06, 0x7E, 0x7C },
  // 4
  { 0x0C, 0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x0C },
  // 5
  { 0x7C, 0x7C, 0x60, 0x78, 0x7C, 0x0C, 0x7C, 0x78 },
  // 6
  { 0x1C, 0x3C, 0x70, 0x6C, 0x7E, 0x66, 0x7E, 0x3C },
  // R
  { 0x7C, 0x7E, 0x66, 0x66, 0x7C, 0x7C, 0x6E, 0x67 }
};
// 设置
const uint8_t LED_SETTINGS[8][8] PROGMEM = {
  // N
  { 0xA0, 0xE0, 0x40, 0x40, 0xE0, 0xA7, 0x05, 0x05 },
  // 1
  { 0xA0, 0xE0, 0x40, 0x46, 0xE2, 0xA2, 0x02, 0x07 },
  // 2
  { 0xA0, 0xE0, 0x40, 0x47, 0xE1, 0xA7, 0x04, 0x07 },
  // 3
  { 0xA0, 0xE0, 0x40, 0x47, 0xE1, 0xA7, 0x01, 0x07 },
  // 4
  { 0xA0, 0xE0, 0x40, 0x45, 0xE5, 0xA7, 0x01, 0x01 },
  // 5
  { 0xA0, 0xE0, 0x40, 0x47, 0xE4, 0xA7, 0x01, 0x07 },
  // 6
  { 0xA0, 0xE0, 0x40, 0x47, 0xE4, 0xA7, 0x05, 0x07 },
  // R
  { 0xA0, 0xE0, 0x40, 0x40, 0xE0, 0xA6, 0x04, 0x04 }
};

// *************************** 传感器预设 ***************************
// 每个传感器在各个挡位下的值（N、1、2、3、4、5、6、R）
int16_t MAP[4][8] PROGMEM = {
  // 
  // ↖|X
  // { 0, 10, 0, 10, 0, 0, 0, 0 },
  { -1, -1, -1, -1, -1, -1, -1, -1 },
  // ↗|Y
  // { 0, 1, 0, 5, 0, 20, 1, 1 },
  { -1, -1, -1, -1, -1, -1, -1, -1 },
  // ↙|Z
  // { 0, 2, 13, 0, -13, 0, -5, -5 },
  { -1, -1, -1, -1, -1, -1, -1, -1 },
  // ↘|null
  // { 0, -7, 0, -7, 26, -5, 25, 34 }
  { -1, -1, -1, -1, -1, -1, -1, -1 },
};

// 上次的挡位
int lastGear = -1;
// 传感器引脚
int sensorPins[4];
// 按键引脚
int keyDownPin = -1;
// 显示画面（0：设置N挡，1：设置1挡，...，6：设置6挡，7：设置倒挡，8：挡显），默认显示挡显
int displayFrame = 8;
int displayFrameTemp = -1;
// 计算挡位位置传感器信息临时变量
int16_t sensorMinArray[4] = { -1, -1, -1, -1 };
int16_t sensorMaxArray[4] = { -1, -1, -1, -1 };

// Max72xxPanel myMatrix;
Max72xxPanel* myMatrix;

// MLX90393
Adafruit_MLX90393 mlx = Adafruit_MLX90393();

EepromUtil eeprom(EEPROM_SIZE);

void loadConfig() {
  // MLX90393是三组，霍尔传感器是四组
  int size = 3;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < 8; j++) {
      int addr = (i * 10 + j) * 2;
      // EEPROM.get(addr, MAP[i][j]);
      MAP[i][j] = eeprom.readInt16(addr);
      Serial.print("get: ");
      Serial.print(addr);
      Serial.print(", ");
      Serial.println(MAP[i][j]);
    }
    Serial.println();
  }
}

void saveConfig() {
  // MLX90393是三组，霍尔传感器是四组
  int size = 3;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < 8; j++) {
      int addr = (i * 10 + j) * 2;
      // EEPROM.put(addr, MAP[i][j]);
      eeprom.writeInt16(addr, MAP[i][j]);
      Serial.print("put: ");
      Serial.print(addr);
      Serial.print(", ");
      Serial.println(MAP[i][j]);
    }
  }
  eeprom.commit();
}

// 初始化Max72xxPanel，并设置传感器引脚
// 初始化MLX90393
void initGearDisplay(int csPin, int inputKeyDownPin) {
  EEPROM.begin(128);

  myMatrix = new Max72xxPanel(csPin, 1, 1);
  // 设置亮度（0~15）
  myMatrix->setIntensity(3);
  // 将第一个屏幕旋转 270°
  myMatrix->setRotation(0, 3);
  // 熄灭屏幕
  myMatrix->fillScreen(0);
  myMatrix->write();
  drawMax7219(&LED_LOGO5, 1000);
  
  // 加载配置
  eeprom.begin();
  loadConfig();
  Serial.println("load config finish");

  // 保存按键引脚
  keyDownPin = inputKeyDownPin;
  pinMode(keyDownPin, INPUT);

  // 初始化MLX90393
  if (!mlx.begin_I2C()) {
      Serial.println("Could not find a valid MLX90393 sensor, check wiring!");
  }
  Serial.println("MLX90393 Initialized successfully.");

}

// 初始化Max72xxPanel，并设置传感器引脚
void initGearDisplay(int csPin, int inputSensorPins[4], int inputKeyDownPin) {
  EEPROM.begin(128);

  myMatrix = new Max72xxPanel(csPin, 1, 1);
  myMatrix->setRotation(0, 3);
  myMatrix->fillScreen(0);
  myMatrix->write();
  drawMax7219(&LED_LOGO5, 1000);
  
  // 加载配置
  loadConfig();
  Serial.println("load config finish");

  // 保存传感器引脚
  for (int i = 0; i < 4; i++) {
    sensorPins[i] = inputSensorPins[i];
  }
  // 保存按键引脚
  keyDownPin = inputKeyDownPin;
  pinMode(keyDownPin, INPUT);
}

// 显示图案
void drawMax7219(const void* ledArray, uint delayTime) {
  // 这里fori是循环列（也就是y轴），forj是循环行（也就是x轴）
  for (int i = 0; i < 8; i++) {
    uint8_t row = pgm_read_byte_near(ledArray + i);
    for (int j = 0; j < 8; j++) {
      // int pixel = (row & 0x80) ? 1 : 0;
      int pixel = row & 0x80;
      myMatrix->drawPixel(j, i, pixel);
      row <<= 1;
    }
  }
  myMatrix->write();
  if (delayTime > 0) {
    delay(delayTime);
  }
}

// 判断当前挡位
int getCurrentGear() {
  // 默认空挡
  int gear = 0;

  // 通过四个传感器来获取档位
  // int leftTop = (analogRead(sensorPins[0]));      // - 2030) / 10;
  // int rightTop = (analogRead(sensorPins[1]));     // - 1860) / 10;
  // int leftBottom = (analogRead(sensorPins[2]));   // - 2130) / 10;
  // int rightBottom = (analogRead(sensorPins[3]));  // - 1750) / 10;
  // Serial.print("sensors: ");
  // Serial.print(leftTop);
  // Serial.print(", ");
  // Serial.print(rightTop);
  // Serial.print(", ");
  // Serial.print(leftBottom);
  // Serial.print(", ");
  // Serial.println(rightBottom);
  // for (int i = 1; i < 8; i++) {
  //   if (inRange(leftTop, MAP[0][i]) && inRange(rightTop, MAP[1][i])
  //       && inRange(leftBottom, MAP[2][i]) && inRange(rightBottom, MAP[3][i])) {
  //     gear = i;
  //   }
  // }

  // 通过MLX90393获取档位
  float x, y, z;
  // 读取 X、Y、Z 轴的磁场强度
  if (mlx.readData(&x, &y, &z)) {
    Serial.print("X: ");
    Serial.print(x);
    Serial.print(", Y: ");
    Serial.print(y);
    Serial.print(", Z: ");
    Serial.println(z);
    for (int i = 1; i < 8; i++) {
      if (inRange((int16_t)x, MAP[0][i]) && inRange((int16_t)y, MAP[1][i]) && inRange((int16_t)z, MAP[2][i])) {
        gear = i;
        Serial.println(gear);
        break;
      }
    }
  } else {
      Serial.println("Failed to read data from MLX90393.");
  }
  return gear;
}

// 档显循环函数
void gearDisplayLoop() {
  // 当前挡位
  int currentGear = -1;
  switch (displayFrame) {
    case 8:
      currentGear = getCurrentGear();
      // Serial.print(lastGear);
      // Serial.print(" =?= ");
      // Serial.println(currentGear);
      // 绘制挡位
      if (lastGear != currentGear) {
        lastGear = currentGear;
        Serial.print("gear: ");
        Serial.print(lastGear);
        Serial.print(" => ");
        Serial.println(currentGear);
        drawMax7219(&LED_GEAR[currentGear], 10);
      }
      break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      drawMax7219(&LED_SETTINGS[displayFrame], 10);
      break;
    default:
      Serial.println("[error] Invalid displayFrame");
  }

  // 按键监听
  if (digitalRead(keyDownPin) == 0) {
    // 按下 - 先缓存下一个页面的下标
    displayFrameTemp = (displayFrame != 8) ? displayFrame + 1 : 0;
  } else if (displayFrameTemp != -1) {
    if (displayFrame >= 0 && displayFrame < 8) {
      // 通过算法推断出当前挡位的值，保存，然后去下一个设置
      for (int i = 0; i < 5; i++) {
        float temp[3] = { -1, -1, -1 };
        mlx.readData(&temp[0], &temp[1], &temp[2]);
        for (int item = 0; item < 3; item++) {
          // 第一种方式：霍尔传感器
          // int itemValue = analogRead(sensorPins[item]);
          // 第二种方式：MLX9039
          int itemValue = (int)temp[item];
          if (sensorMinArray[item] == -1 || itemValue < sensorMinArray[item]) {
            sensorMinArray[item] = itemValue;
          }
          if (sensorMaxArray[item] == -1 || itemValue > sensorMaxArray[item]) {
            sensorMaxArray[item] = itemValue;
          }
        }
      }
      Serial.print("  sensorMinArray  ");
      Serial.print(sensorMinArray[0]);
      Serial.print(",");
      Serial.print(sensorMinArray[1]);
      Serial.print(",");
      Serial.print(sensorMinArray[2]);
      Serial.print(",");
      Serial.println(sensorMinArray[3]);
      Serial.print("  sensorMaxArray  ");
      Serial.print(sensorMaxArray[0]);
      Serial.print(",");
      Serial.print(sensorMaxArray[1]);
      Serial.print(",");
      Serial.print(sensorMaxArray[2]);
      Serial.print(",");
      Serial.println(sensorMaxArray[3]);
      // 取平均（平均值±20绝对不会有问题），存入MAP
      for (int item = 0; item < 4; item++) {
        MAP[item][displayFrame] = (sensorMaxArray[item] + sensorMinArray[item]) / 2;
        // 计算完平均值立即清空，防止后续计算错误
        sensorMaxArray[item] = -1;
        sensorMinArray[item] = -1;
      }
      Serial.print("             MAP  ");
      Serial.print(MAP[0][displayFrame]);
      Serial.print(",");
      Serial.print(MAP[1][displayFrame]);
      Serial.print(",");
      Serial.print(MAP[2][displayFrame]);
      Serial.print(",");
      Serial.println(MAP[3][displayFrame]);
    }
    // 释放 - 读取缓存的下标，然后清空缓存
    displayFrame = displayFrameTemp;
    displayFrameTemp = -1;
    Serial.print("new displayFrame => ");
    Serial.println(displayFrame);

    if (displayFrame == 8) {
      Serial.println("start save config");
      saveConfig();
      Serial.println("save config finish");
      drawMax7219(&LED_EMPTY, 0);
    }
  }
}

// 判断数值是否在目标范围内
bool inRange(int value, int target) {
  int range = 50;
  return abs(target - value) <= range;
}
