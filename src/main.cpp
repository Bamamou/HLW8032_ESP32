/**
 * @file hlw8032-ESP32.cpp
 * @brief Simple Test for HLW8032 Library using plaformio
 *
 * Basic example demonstrating the use of the HLW8032 library for power measurement.
 *
 * @author Bamamounicolas
 * @email bamamounicolas@gmail.com
 * @version 1.0
 * @date January 10, 2025
 * Copyright (c) 2025 Bamamou  Nicolas
 * Licensed under LGPL-3.0, see <http://www.gnu.org/licenses/>.
 */

#include "Hlw8032.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>  // for the LCD display


// Create HLW8032 instance
Hlw8032 hlw;
// Create Serial2 instance for HLW8032 communication
HardwareSerial SerialHLW(2); // Using ESP32's Serial2  (RX2=GPIO16, TX2=GPIO17)
#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

float Current, Power, Voltage;


void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  // Init
  lcd.init();
  lcd.backlight();
  Serial.println("HLW8032 Initialization complete");
  // Initialize Serial2 for HLW8032 communication
  hlw.begin(SerialHLW);
  // Optional: Set calibration values if needed
  hlw.setVF(1.9); // Voltage calibration
  hlw.setCF(0.005); // Current calibration
  lcd.setCursor(0,0);
  lcd.print("Voltage: ");
  lcd.setCursor(0,1);
  //lcd.print("Current: ");
  lcd.print("Power: ");
    

}

void loop() {
  // Call the HLW8032 update method
  Voltage = hlw.GetVoltage();
  Current = hlw.GetCurrent();
  Power = hlw.GetPower();
  lcd.setCursor(10,0);
  lcd.print(Voltage);
  lcd.setCursor(10,1);
  lcd.print(Power);
  
  //Serial.printf("Voltage: %.2f \n", Voltage); // Voltage at 2 digits precision
  Serial.printf("Current: %.2f \n", Current); // Voltage at 2 digits precision
  // Serial.printf("Power: %.2f \n", Power); // Voltage at 2 digits precision


}