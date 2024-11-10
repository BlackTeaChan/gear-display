/**
 * Gear Display
 * 挡位显示器
 *   MAX7219 8x8 Display & HS-S50P(EG49)
 *   Max7219矩阵屏 & 霍尔传感器（EG49）
 * 
 * 接线：
 * MAX7219 -> Arduino
 *  GND ------ GND
 *  VCC ------ 3.3V
 *  DIN ------ MOSI
 *  CS  ------ CS
 *  CLK ------ SCK
 * 参考资料：http://www.hellostem.cn/?pjiliechuanganqipeijiandeng/167.html
 * 
 * 包含功能：
 *     1. 自动判定挡位
 *     2. 可手动配置挡位数据
 *     3. 【TODO】配置文件读写进EEPROM
 *     4. 【TODO】优化N档瞬闪（延时展示）
 *     5. 【TODO】手动调节亮度
 *     6. 【TODO】自动调节亮度
 * 
 * 占用EEPROM地址：
 *     0~16, 20~36, 40~56, 60~76
 * @author BlackTeaChan
 */
#ifndef GEAR_DISPLAY_H
#define GEAR_DISPLAY_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <EEPROM.h>
// #include <avr/pgmspace.h>

// 空屏
extern const uint8_t LED_EMPTY[8] PROGMEM;
// 爱心
extern const uint8_t LED_HEART[2][8] PROGMEM;
// 档位
extern const uint8_t LED_GEAR[8][8] PROGMEM;

// 函数声明
void loadConfig();
void saveConfig();
void initGearDisplay(int csPin, int sensorPins[4], int keyDownPin);
int getCurrentGear();
void drawMax7219(const void* ledArray, uint delayTime);
void gearDisplayLoop();
bool inRange(int value, int target);

#endif