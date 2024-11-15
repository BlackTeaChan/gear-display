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
 * @author BlackTeaChan
 */
#include <GearDisplay.h>

void setup() {
  Serial.begin(9600);
  
  // 传感器引脚
  int sensorPins[4] = { A3, A4, A5, A6 };
  // 按键引脚
  int keyDownPin = 4;
  // 初始化
  initGearDisplay(5, sensorPins, keyDownPin);

  // boot animation
  for (int i = 0; i < 3; i++) {
    drawMax7219(&LED_HEART[0], 700);
    drawMax7219(&LED_HEART[1], 150);
  }
  drawMax7219(&LED_HEART[0], 700);
  drawMax7219(&LED_EMPTY, 0);
}

void loop() {

  gearDisplayLoop();
  delay(250);
}
