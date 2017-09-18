#include <Wire.h>
#include <LSM303_D.h>

CLSM303_D compass;

CVector vectMin, vectMax;
void setup() 
{
  Serial.begin(115200);
  Wire.begin();
  compass.begin(19, LOW);
  compass.setDefaultMode();
}

void loop() 
{
  /*
  When given no arguments, the heading() function returns the angular
  difference in the horizontal plane between a default vector and
  north, in degrees.
  
  The default vector is chosen by the library to point along the
  surface of the PCB, in the direction of the top of the text on the
  silkscreen. This is the +X axis on the Pololu LSM303D carrier and
  the -Y axis on the Pololu LSM303DLHC, LSM303DLM, and LSM303DLH
  carriers.
  
  To use a different vector as a reference, use the version of heading()
  that takes a vector argument; for example, use
  
    compass.heading((CLSM303_D::vector<int>){0, 0, 1});
  
  to use the +Z axis as a reference.
  */
  float heading = compass.getHeading();
  
  Serial.println(heading);
  delay(100);
}
