#include <Arduino.h>
#include <GLCD.h>

LCD lcd;

void setup() {
  lcd.init();
  lcd.clear();
}

void loop() {
  lcd.print(0,0,"v----------------------------v");
  lcd.print(1,0,"|       lcd display          |");
  lcd.print(2,0,"|      The Shed 2018         |");
  lcd.print(3,0,"|                            |");
  lcd.print(4,0,"|                            |");
  lcd.print(5,0,"|                            |");
  lcd.print(6,0,"|                            |");
  lcd.print(7,0,"|                            |");
  lcd.print(8,0,"|                            |");
  lcd.print(9,0,"^----------------------------^");
  delay(1000);
  lcd.clear();
  lcd.print(0,4,"Value of analog port 0");
  while(true){
    lcd.print(1,11,String(analogRead(0)));
  }
}

