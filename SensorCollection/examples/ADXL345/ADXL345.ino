#include <ADXL345.h>
#include <Wire.h>                                                                //include the Wire library (necessary for I2C and SPI protocols)

boolean bSingleTap = false;                                                        //declare and initialize boolean variables to track tap type
boolean bDoubleTap = false;
boolean bFreeFall = false;

CADXL345 accel;                                                        //create an instance of class ADXL345() named accel

void setup()
{
  Serial.begin(115200);                                                            //join the serial buss at 9600 baud
  //attachInterrupt(0, interrupt0, RISING);                                        //attach an interrupt on digital pin 2 (interrupt 0) for rising signal  
  //attachInterrupt(1, interrupt1, RISING);                                        //attach an interrupt on digital pin 3 (interrupt 1) for rising signal
  pinMode(4, OUTPUT);                                                            //set LED pins to output
  accel.begin(0, 0);
  accel.setTapInterrupt(singleTapISR, 0, doubleTapISR, 1, true);
  //accel.setFreeFallInterrupt(freeFallISR, 0, true);
/*
  accel.setTHRESH_TAP(THRESH_TAP);                                               //set registers on the ADXL345 to the above defined values (see datasheet for tuning and explanations)
  accel.setDUR(DUR);
  accel.setLATENT(LATENT);
  accel.setWINDOW(WINDOW);
  accel.setTAP_AXES(TAP_AXES);
  accel.setPOWER_CTL(POWER_CTL);
  accel.setINT_MAP(INT_MAP);
  accel.setDATA_FORMAT(DATA_FORMAT);
  accel.setINT_ENABLE(INT_ENABLE);
*/
}

void loop()                                                                      //main loop
{
  float fX = accel.getX(), fY = accel.getY(), fZ = accel.getZ();
  Serial.print("X = ");
  Serial.print(fX);
  Serial.print(", Y = ");
  Serial.print(fY);
  Serial.print(", Z = ");
  Serial.print(fZ);

  Serial.print(", Roll = ");
  Serial.print(accel.getRoll(fX, fZ));
  Serial.print(", Pitch = ");
  Serial.println(accel.getPitch(fY, fZ));
  accel.clearInts();
    
  //Serial.println("SOURCE = ");
  //Serial.println(accel.readINT_SOURCE());
  
  if(bSingleTap)                                                                  //if single tap is detected
  {
    digitalWrite(4, HIGH);
    delay(500);
    digitalWrite(4, LOW);
    if(!bDoubleTap)
      Serial.println("Single Tap!");
    else                                                                         //if a second tap is deceted during the half second wait, preceed to blink pin 5 (which will light the LED green)
      Serial.println("Double Tap");
    bSingleTap = false;                                                          //reset tap values to ensure the lights stop blinking until the next tap sets either singleTap or doubleTap to true (see below)
    bDoubleTap = false;
  }
  else if (bFreeFall)
  {
    digitalWrite(4, HIGH);
    delay(500);
    digitalWrite(4, LOW);
    Serial.println("Free Fall!");
    bFreeFall = false;
  }
  else
    delay(500);

}

void singleTapISR()                                                              //a RISING on interrupt pin 2 (interrupt 0) would mean a single tap (from how we have wired and configured the ADXL345), therefore, set singleTap to true
{
	bSingleTap = true;
}

void doubleTapISR()                                                              //a RISING on interrupt pin 3 (interrupt 1) would mean a double tap (again, from how we have wired and configured the ADXL345), therefore, set doubleTap to true
{
	bDoubleTap = true;
}

void freeFallISR()                                                              //a RISING on interrupt pin 3 (interrupt 1) would mean a double tap (again, from how we have wired and configured the ADXL345), therefore, set doubleTap to true
{
	bFreeFall = true;
}