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


// Create HLW8032 instance
Hlw8032 hlw;
// Create Serial2 instance for HLW8032 communication
HardwareSerial SerialHLW(2); // Using ESP32's Serial2

float Current;
float Voltage;


void powerDataCallBack(int magic, float Vrms, float Irms, float P, float CF) {
  (void)magic;
  Serial.print("Vrms: ");
  Serial.print(Vrms);
  Serial.print(" V, Irms: ");
  Serial.print(Irms);
  Serial.print(" A, Power: ");
  Serial.print(P);
  Serial.print(" W, PF: ");
  Serial.print(CF);
  Serial.println(".");
}


void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);

  Serial.println("HLW8032 Initialization complete");

  // Initialize Serial2 for HLW8032
  SerialHLW.begin(4800, SERIAL_8E1);
  // Optional: Set calibration values if needed
  // hlw.setVF(1.9); // Voltage calibration
  hlw.setCF(0.0022); // Current calibration
    
 // hlw.onReceiveCallBack(powerDataCallBack);

}

void loop() {
  if (SerialHLW.available() > 0) {

    hlw.rxProcess(SerialHLW.read());  // This function will process the data received from the HLW8032 IC (It is a must to call this function in the loop)
    Voltage = hlw.GetVoltage();
    Current = hlw.GetCurrent();


    // Printing the values
    //Serial.println(Voltage);
     Serial.println(Current);
   
  }
}