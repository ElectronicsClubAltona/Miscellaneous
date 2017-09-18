#include <SoftwareSerial.h>

SoftwareSerial camera(7, 6); // RX, TX

void setup() 
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Camera test!");

  camera.begin(57600);

  // Reset
  camera.print(0x56);
  camera.print(0x00);
  camera.print(0x26);
  camera.print(0x00);
  Serial.print(F("Sending: "));
  Serial.print(0x56, HEX);
  Serial.print(F(" "));
  Serial.print(0x00, HEX);
  Serial.print(F(" "));
  Serial.print(0X26, HEX);
  Serial.print(F(" "));
  Serial.print(0X00, HEX);
  Serial.print(F(" => "));

  while (!Serial.available())
  {
  }
  
  while (Serial.available())
  {
    Serial.print(camera.read(), HEX);
  }
  Serial.println();
}

void loop() 
{
  
}

