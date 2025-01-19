/**
 * @file Hlw8032.h
 * @brief HLW8032 Power Measurement Class
 *
 * Provides an interface for the HLW8032 power/current/voltage monitor IC. 
 * Includes methods for setting calibration factors, processing data, and 
 * handling power data callbacks.
 * 
 * @author Ahmed ARIF
 * @email arif193@gmail.com
 * @version 1.0
 * @date Nov 18, 2023
 * Copyright (c) 2023 Ahmed ARIF
 * Licensed under LGPL-3.0, see <http://www.gnu.org/licenses/>.
 */

#include <Arduino.h>

typedef void (*PowerDataCallback)(int magic, float Vrms, float Irms, float P, float CF);


class Hlw8032 {
  uint8_t buffer[25];
  int index = 0;
  bool headerFound = false;
  byte check = 0;
public:
  // Constructor
  Hlw8032();
  void setVF(float data){
    VF=data;
  }
  void setCF(float data){
    CurrentRF=data;
  }

  void setMagic(int magic){
    this->magic = magic;
  }
  // to get the current value
  float GetVoltage();
  //Method to get the analog value of the current
  float GetCurrent();

  // Method to set the callback function
  void onReceiveCallBack(PowerDataCallback callback);
  // Method to process received data
  void rxProcess(uint8_t data);
  float CurrentRF = 0.0022; 

  



private:
  bool Checksum(uint8_t *frame) {
    
    byte check = 0;
    for (byte a = 2; a <= 22; a++) {
      check += frame[a];
    }
    return check == frame[23];
  }
  float VF; // Voltage Factor
  float CF; // Current Factor

  uint32_t VolR1 = 1880000;  //Voltage divider Upstream resistors 470K*4  1880K
  uint32_t VolR2 = 1000;     //Voltage divider downstream resistors  1K
  uint32_t VolData;         //voltage value from the serial port
  uint32_t VolPar;         //voltage parameter
  uint32_t CurrentPar; 		//current parameter
  uint32_t PowerPar;     //power parameter
  uint32_t CurrentData; //current data

  PowerDataCallback _callback;
  void processFrame(uint8_t *frame);
  int magic = 0;

};