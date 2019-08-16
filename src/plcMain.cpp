#include <Arduino.h>
#include <OBD2UART.h>
#include <GLCD.h>
// for hardcoded debug stuff
#define DEBUG 1

// for softcoded print funcutions and to disable outputs
bool testMode = true;

COBD obd;
LCD lcd;

int rpm = 6000;
int bottlePressure = 900;
int throttlePos = 100;
int afr = 11;
int coolantTemp = -150;

const int rpmPin = 1;
const int bottlePressureOutputPin = 2;
const int bottlePressureInputPin = 10;
const int throttlePosPin = 3;
const int afrPin = 4;
const int coolantTempPin = 5;
const int armInputPin = 6;
const int armOutputPin = 7;
const int autoInputPin = 8;
const int autoOutputPin = 9;
// TODO
// Pedal Switch in sereis with NOS RElAY

void getData(){
  obd.readPID(PID_RPM, rpm);
  obd.readPID(PID_THROTTLE, throttlePos);
  obd.readPID(PID_AIR_FUEL_EQUIV_RATIO, afr);
  obd.readPID(PID_COOLANT_TEMP, coolantTemp);
}

void getTemp(){

}

void conditionCheck(){
if (armInputPin == HIGH ) {

  if ((rpmPin > 4000) && (rpm < 8000)) {
    digitalWrite(rpmPin, HIGH);
  } else {
    digitalWrite(rpmPin, LOW);
  }

  if ((bottlePressure > 1050) && (bottlePressure < 800)) {
    digitalWrite(bottlePressureOutputPin, HIGH);
  } else {
    digitalWrite(bottlePressureOutputPin, LOW);
  }

  if (throttlePos > 98) {
    digitalWrite(throttlePosPin, HIGH);
  } else {
    digitalWrite(throttlePosPin, LOW);
  }

  if (afr < 12) {
    digitalWrite(afrPin, HIGH);
  } else {
    digitalWrite(afrPin, LOW);
  }

  if (coolantTemp < -100) {
    digitalWrite(coolantTempPin, HIGH);
  } else {
    digitalWrite(coolantTempPin, LOW);
  }

  if (armInputPin == HIGH) {
    digitalWrite(armOutputPin, HIGH);
  } else {
    digitalWrite(armOutputPin, LOW);
  }

}

}

void welcomeScreen() {
  lcd.clear();
  delay(60);
  lcd.print(0, 0, "v----------------------------v");
  delay(60);
  lcd.print(1, 0, "|       NOS CONTROL          |");
  delay(60);
  lcd.print(2, 0, "|      The Shed 2019         |");
  delay(60);
  lcd.print(3, 0, "|                            |");
  delay(60);
  lcd.print(4, 0, "|                            |");
  delay(60);
  lcd.print(5, 0, "|    Hartford Race Team      |");
  delay(60);
  lcd.print(6, 0, "|     Test Build      V02    |");
  delay(60);
  if (testMode) {
    lcd.print(7, 0, "|  ***!!Test Mode On!!***    |");
    delay(60);
    lcd.print(8, 0, "|     OUTPUTS   DISABLED     |");
  } else {
    lcd.print(7, 0, "|                            |");
    delay(60);
    lcd.print(8, 0, "|                            |");
  }
  lcd.print(9, 0, "^----------------------------^");
  delay(1000);
  lcd.clear();
}

void statScreen() {

    lcd.print(0, 0, "---------NOS CONTROL---------");

    if (autoInputPin == HIGH) {
      lcd.print(1, 0, "AUTO NOS:            ENABLED");
    } else {
      lcd.print(1, 0, "AUTO NOS:            DISABLED");
    }

    lcd.print(2, 0, "BOTTLE PRESSURE:      " + (String(bottlePressure)) + " PSI");

    lcd.print(3, 0, "THROTTLE POSITION:    " + (String(throttlePos)) + " %");

    lcd.print(4, 0, "ENGINE SPEED:          " + (String(rpm)) + " RPM");
    lcd.print(5, 0, "COOLENT TEMP:          " + (String(coolantTemp)) + " F" );


    lcd.print(6, 0, "AFR                    " + (String(afr)) + " λ");
if (armInputPin == HIGH) {
  lcd.print(7, 7, "**SYSTEM ARMED**");
  lcd.print(8, 7, "****************");
} else {
  lcd.print(7, 0, "                                        ");
  lcd.print(8, 0, "                                        ");
}

}

void setup() {
  delay(1); // change to 5000 for production
  Serial.begin(9600);
  Serial.println("Starting test mode...");
  lcd.init();
  welcomeScreen();
#ifdef DEBUG
#else
  obd.begin(); // Wait for ODB Coms
  while (!obd.init());
#endif

  pinMode(armInputPin, INPUT);
  pinMode(autoInputPin, INPUT);
  pinMode(bottlePressureInputPin, INPUT);

  pinMode(rpmPin, OUTPUT);
  pinMode(bottlePressureInputPin, OUTPUT);
  pinMode(throttlePosPin, OUTPUT);
  pinMode(afrPin, OUTPUT);
  pinMode(coolantTempPin, OUTPUT);
  pinMode(armOutputPin, OUTPUT);
  pinMode(autoOutputPin, OUTPUT);
  pinMode(bottlePressureOutputPin, OUTPUT);

}

void loop() {
  getData();
  conditionCheck();
  statScreen();
}
