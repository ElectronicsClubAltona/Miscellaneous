#include <BMP180.h>

void setup() 
{
    bmp180.begin();
}

void loop() 
{
    bmp180.DoDisplay();
    Serial.println("------------------------------------------");
    delay(1000);
}