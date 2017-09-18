with its default full scale setting of +/- 2 g. Dropping the lowest 4
bits gives a 12-bit raw value of -1009. The LA_So specification in the
LSM303DLH datasheet (page 11) states a conversion factor of 1 mg/digit
at this FS setting, so the value of -1009 corresponds to -1009 * 1 =
1009 mg = 1.009 g.
*/

#include <Wire.h>
#include <LSM303_D.h>

CLSM303_D compass;

char report[80];

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  compass.begin(19, LOW);
  compass.setDefaultMode();
}

void loop()
{
  CVector vectMag, vectAccel;
  compass.getAxes(vectMag, vectAccel);

  snprintf(report, sizeof(report), "A: %6d %6d %6d    M: %6d %6d %6d",
    vectAccel.m_dX, vectAccel.m_dY, vectAccel.m_dZ,
    vectMag.m_dX, vectMag.m_dY, vectMag.m_dZ);
  Serial.println(report);

  delay(100);
}