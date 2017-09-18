#include <Wire.h>
#include <LSM303_D.h>

CLSM303_D lsm303d;
CVector vectRunningMin = CVector(32767, 32767, 32767), vectRunningMax = CVector(-32768, -32768, -32768);
CVector vectMag, vectAccel;

void setup() 
{
  Serial.begin(115200);
  Wire.begin();
  lsm303d.begin(19, LOW);
  lsm303d.setDefaultMode();
}

void loop() 
{  
	lsm303d.getAxes(vectMag, vectAccel);
	
/*
  static uint16_t nI = 0;
  static double dX = 0.0, dY = 0.0, dZ = 0.0;

  if ((nI >= 0) && (nI < 10000))
  {
      lsm303d.getAxes(vectMag, vectAccel);
      dX += vectAccel.m_dX;
      dY += vectAccel.m_dY;
      dZ += vectAccel.m_dZ;
      nI++;
  }
  else
  {
    Serial.println("dX = " + String(dX / nI) + ", dY = " + String(dY / nI) + ", dZ = " + String(dZ / nI));
    delay(500);
  }
*/
	
  
  vectRunningMin.m_dX = min(vectRunningMin.m_dX, vectMag.m_dX);
  vectRunningMin.m_dY = min(vectRunningMin.m_dY, vectMag.m_dY);
  vectRunningMin.m_dZ = min(vectRunningMin.m_dZ, vectMag.m_dZ);

  vectRunningMax.m_dX = max(vectRunningMax.m_dX, vectMag.m_dX);
  vectRunningMax.m_dY = max(vectRunningMax.m_dY, vectMag.m_dY);
  vectRunningMax.m_dZ = max(vectRunningMax.m_dZ, vectMag.m_dZ);
  
  Serial.println("Accelerator\n-----------");
  Serial.println("X = " + String(vectAccel.m_dX));
  Serial.println("Y = " + String(vectAccel.m_dY));
  Serial.println("Z = " + String(vectAccel.m_dZ));
  Serial.println("");
  
  Serial.println("Magnetometer\n-----------");
  Serial.println("X = " + String(vectMag.m_dX) + " (Max = " + String(vectRunningMax.m_dX) + ", Min = " + String(vectRunningMin.m_dX) + ")");
  Serial.println("Y = " + String(vectMag.m_dY) + " (Max = " + String(vectRunningMax.m_dY) + ", Min = " + String(vectRunningMin.m_dY) + ")");
  Serial.println("Z = " + String(vectMag.m_dZ) + " (Max = " + String(vectRunningMax.m_dZ) + ", Min = " + String(vectRunningMin.m_dZ) + ")");
  Serial.println("=====================================================================================");

  delay(500);
}
