#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <LSM303_DLH.h>

/* Assign a unique ID to this sensor at the same time */
CLSM303_DLH_Accelerator accel = CLSM303_DLH_Accelerator(54321);

void setup(void) 
{
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  Serial.begin(115200);
  Serial.println("Accelerometer Test"); Serial.println("");
  
  accel.begin(19, LOW);
}

void loop(void) 
{
  /* Get a new sensor event */ 
  CVector vect = accel.getAxes();
 
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(vect.m_fX); Serial.print("  ");
  Serial.print("Y: "); Serial.print(vect.m_fY); Serial.print("  ");
  Serial.print("Z: "); Serial.print(vect.m_fZ); Serial.print("  ");Serial.println("m/s^2 ");
  delay(500);
}
