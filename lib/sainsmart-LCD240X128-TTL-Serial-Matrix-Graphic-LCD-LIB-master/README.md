# Sainsmart 240x128 TTL Lib

**will soon add the abbility to change scerial port or to use software scerial**



## Example

```cpp
#include <GLCD.h> //Incldue the library

LCD lcd; //Initlise it

void setup() { 
  lcd.init(); //Initlize the lcd and port
  lcd.clear(); //Clear the intro message off of the lcd
  lcd.print(0, 4, "Value of analog port 0"); //Print some text on the screen
  //Other setup code can go here
}

void loop() {
  lcd.print(1, 11, String(analogRead(0))); //print the value of analog 0 to the screen
                                         // the String(alalogRead(0)) is so that the int analog returns is returned as a string for the screial print
}
```
## Usage

```cpp
lcd.print(row, col, string)
```

print accepts a row, from 0 to 9. col is a distance in px. and string is whatever data you would like to see.

```cpp
lcd.clear()
```

Dose what it says, clears the lcd

```cpp
lcd.init()
```

Initlises the scerial port and lcd

