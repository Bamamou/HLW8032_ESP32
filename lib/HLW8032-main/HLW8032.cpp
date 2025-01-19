#include "Hlw8032.h"


Hlw8032::Hlw8032() {
  VF = VolR1 / VolR2;
  CF = 1.0 / (CurrentRF *1000.0);    // Current calibration factor
}

void Hlw8032::onReceiveCallBack(PowerDataCallback callback) {
  _callback = callback;
}

// (header)	(Uk)	(Ut)	    (Ik)	    (It)	    (Pk)	    (Pt)	    (Adj)	(CFm)	(CFl)	(CS) =	(test)
// (55 5A)	(02 C6 28)	(00 01 95)	(00 3C 3C)	(00 67 0E)	(4B 49 70)	(00 49 AC)	(71)	(BF)	(FB)	(BD) =	(230V 0.180A 40W)
// (55 5A)	(02 C6 28)	(00 01 9E)	(00 3C 3C)	(00 02 A1)	(4B 49 70)	(00 01 ED)	(71)	(F9)	(7F)	(85) =	(230V 7A 1600W)

void Hlw8032::rxProcess(uint8_t data) {
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

void Hlw8032::processFrame(uint8_t *frame) {
  uint32_t Uk = (frame[2] << 16) | (frame[3] << 8) | frame[4];
  uint32_t Ut = (frame[5] << 16) | (frame[6] << 8) | frame[7];
  uint32_t Ik = (frame[8] << 16) | (frame[9] << 8) | frame[10];
  uint32_t It = (frame[11] << 16) | (frame[12] << 8) | frame[13];
  uint32_t Pk = (frame[14] << 16) | (frame[15] << 8) | frame[16];
  uint32_t Pt = (frame[17] << 16) | (frame[18] << 8) | frame[19];
  uint32_t CF = (frame[20] << 8) | frame[21];
  
  // Get the voltgae register
  VolPar = ((uint32_t)frame[2]  <<16) + ((uint32_t)frame[3] <<8) + frame[4]; //Get Voltage Parameter Register
	if(bitRead(frame[20], 6) == 1)  //If the voltage register is refreshed, fetch the data
		{
			VolData = ((uint32_t)frame[5]  <<16) + ((uint32_t)frame[6] <<8) + frame[7]; //Get Voltage Register
		}

  // Get the current register
  CurrentPar = ((uint32_t)frame[8]  <<16) + ((uint32_t)frame[9] <<8) + frame[10];  //Get Current Parameter Register 
	if(bitRead(frame[20], 5) == 1)   //If the Current register is refreshed, fetch the data
		{
			CurrentData = ((uint32_t)frame[11]  <<16) + ((uint32_t)frame[12] <<8) + frame[13];  //Get Current Register
		}
  const int32_t AdjustmentFactorV = 37760/1.986;
  float AdjustedRv2_Rv1_Tv = 1934;
  const int32_t Ri_times_10000 = 200000;

  float Vrms = (float)Uk / Ut * (AdjustmentFactorV / 10000.0f);
  float Irms = (float)Ik / It /20;
  float currentout = (float)(Ik / It );
  float P = (float)Pk * (AdjustedRv2_Rv1_Tv / 10000.0f) / (Pt * 20.0f);
  

  if (_callback) {
    _callback(magic, Vrms, Irms, P, CF);
  }

  
}

float Hlw8032::GetCurrent()
{
  float FCurrentPar = CurrentPar;
	float Current  = FCurrentPar / (float)CurrentData;
	return Current* CF;  //return CurrentData;
}


// Gte the voltage value
float Hlw8032::GetVoltage()
{	
	float FVolPar = VolPar;   // Get the voltage parameter
	float Vol = FVolPar / VolData;
	return Vol* VF;; //return VolData;
}


