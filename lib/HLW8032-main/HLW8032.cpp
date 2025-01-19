#include "Hlw8032.h"


Hlw8032::Hlw8032() {
  // VF = VolR1 / VolR2;
  // CF = 1.0 / (CurrentRF *1000.0);    // Current calibration factor
}
void Hlw8032::begin(HardwareSerial& SerialData)
{
  delay(10);
	SerialID = &SerialData; 
	SerialID->begin(4800,SERIAL_8E1);   //指定4800波特率，偶校验  符号为->指针调用
  VF = VolR1 / VolR2;
  CF = 1.0 / (CurrentRF *1000.0);    // Current calibration factor
}


// (header)	(Uk)	(Ut)	    (Ik)	    (It)	    (Pk)	    (Pt)	    (Adj)	(CFm)	(CFl)	(CS) =	(test)
// (55 5A)	(02 C6 28)	(00 01 95)	(00 3C 3C)	(00 67 0E)	(4B 49 70)	(00 49 AC)	(71)	(BF)	(FB)	(BD) =	(230V 0.180A 40W)
// (55 5A)	(02 C6 28)	(00 01 9E)	(00 3C 3C)	(00 02 A1)	(4B 49 70)	(00 01 ED)	(71)	(F9)	(7F)	(85) =	(230V 7A 1600W)

void Hlw8032::rxProcess(uint8_t data) {
  if (SerialID->available() > 0) 
  {
    
  if (data == 0x55) {
    headerFound = true;
    buffer[0] = data;
  } else if (data == 0x5A && headerFound) {
    buffer[1] = data;
    index = 2;
    check = 0;
  } else if (index >= 2 && index < 24) {
    buffer[index] = data;
    if (index < 23) {
      check += data;
    }
    index++;
    if (index == 24) {
      if (check == buffer[23]) {
        processFrame(buffer);
      }
      index = 0;
      headerFound = false;
    }
  } else {
    index = 0;
    headerFound = false;
  }
  }
  else
  {
    Serial.println("No data available");
  }
}

void Hlw8032::processFrame(uint8_t *frame) {
  VolPar      = (frame[2] << 16) | (frame[3] << 8) | frame[4];
  VolData     = (frame[5] << 16) | (frame[6] << 8) | frame[7];
  CurrentPar  = (frame[8] << 16) | (frame[9] << 8) | frame[10];
  CurrentData = (frame[11] << 16) | (frame[12] << 8) | frame[13];
  PowerPar    = (frame[14] << 16) | (frame[15] << 8) | frame[16];
  PowerData   = (frame[17] << 16) | (frame[18] << 8) | frame[19];
  uint32_t CF = (frame[20] << 8) | frame[21]; 
}

// Get the current value
float Hlw8032::GetCurrent()
{
  rxProcess(SerialID->read());
  float FCurrentPar = CurrentPar;
	float Current  = FCurrentPar / (float)CurrentData;
	return Current* CF;  //return CurrentData;
}


// Gte the voltage value
float Hlw8032::GetVoltage()
{	
  rxProcess(SerialID->read());
	float FVolPar = VolPar;   // Get the voltage parameter
	float Vol = FVolPar / VolData;
	return Vol* VF;; //return VolData;
}

// Get the power value
float Hlw8032::GetPower()
{	
  rxProcess(SerialID->read());
  float FPowerPar = PowerPar;
	float FPowerData = PowerData;
	float Power = FPowerPar/FPowerData * VF * CF;  // 求有功功率
	return Power;
}
