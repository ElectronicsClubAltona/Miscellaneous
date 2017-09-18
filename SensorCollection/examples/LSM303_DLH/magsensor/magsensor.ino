#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <LSM303_DLH.h>

/* Assign a unique ID to this sensor at the same time */
CLSM303_DLH_Magnetometer mag = CLSM303_DLH_Magnetometer(12345);

void setup(void) 
{
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  Serial.begin(115200);
  Serial.println("Magnetometer Test"); Serial.println("");
  
 mag.begin(19, LOW);
}

void loop(void) 
{
  /* Get a new sensor event */ 
  CVector vect = mag.getAxes();
 
  Serial.print("X: "); Serial.print(vect.m_fX); Serial.print("g  ");
  Serial.print("Y: "); Serial.print(vect.m_fY); Serial.print("g  ");
  Serial.print("Z: "); Serial.print(vect.m_fZ); Serial.println("g");
  delay(500);
}
