#include <Arduino.h>
#include <LedControl.h>

LedControl lc=LedControl(14,13,12,3); // NodeMCU Pins: D5 = 14, D6=12, D7=13

// pin 14 is connected to the MAX7219 pin 1 (DIN)
// pin 13 is connected to the CLK pin 13
// pin 12 is connected to LOAD pin 12 (CS)
// 3 as we are using 3 MAX7219

void setup()
{
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0,false);// turn off power saving, enables display
  lc.shutdown(1,false);// turn off power saving, enables display
  lc.shutdown(2,false);// turn off power saving, enables display
  lc.setIntensity(0,1);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  Serial.begin(115200);
}

void loop()
{
  //Turn the LEDs on one after another...
  Serial.println("Turning LEDs on...");
  lc.setLed(2,0,0,true);
  delay(500);
  lc.setLed(1, 4, 4, true);
  delay(500);
  lc.setLed(0,7,7,true);
  delay(500);
  //... and turn them off again
  Serial.println("Turning LEDs off...");
  lc.setLed(2,0,0,false);
  delay(500);
  lc.setLed(1, 4, 4, false);
  delay(500);
  lc.setLed(0,7,7,false);
  delay(500);
}
