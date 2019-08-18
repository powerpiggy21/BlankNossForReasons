
// By Trevor Gruszynski && Evan Gruszynski
#include <Arduino.h>
#include <OBD2UART.h>
//#include <Nextion.h>
// for hardcoded debug stuff
#define DEBUG 1

// for softcoded print funcutions and to disable outputs
bool testMode = true;

COBD obd;



int rpm = 6000;
int bottleTemp = 80;
int throttlePos = 100;
int afr = 11;
int coolantTemp;
const int fireButtonPin = 1;
const int nosRelayPin = 2;
const int autoPin = 3;
const int armPin = 4;
const int purgeButtonPin = 5;
const int purgeRelayPin = 6;
const int heatPin = 7; // bottle heater relay pin
const int bottlePin = 8; // high pressure solonoid for bottle cut off
const int thermostatRelayPin = 9; // sets car thermostate to -255 F
const int bottlePressureHighPin = 10; // 1000 psig pressure switch
const int bottlePressureLowPin = 11; // 800 psig pressure switch
const int linePressurePin = 12; // 4 psig pressure switch


// TODO
// PurgeControl logic
// Bottle Heat logic
// Pedal Switch in sereis with NOS REl

void getData() {
  obd.readPID(PID_RPM, rpm);
  obd.readPID(PID_THROTTLE, throttlePos);
  obd.readPID(PID_AIR_FUEL_EQUIV_RATIO, afr);
  obd.readPID(PID_COOLANT_TEMP, coolantTemp);
}

void nosControl() {


  if ((fireButtonPin == HIGH) && (armPin == HIGH) && (4000 < rpm) &&
      (rpm < 7500) && (throttlePos > 98)) {
    digitalWrite(thermostatRelayPin, HIGH);
  }

  if (armPin == HIGH) {
    digitalWrite(bottlePin, HIGH);
  } else {
    digitalWrite(bottlePin, LOW);
  }

  if ((coolantTemp < -100) && (afr < 12) && (throttlePos > 98) &&
      (armPin == HIGH) && (4000 < rpm) && (rpm < 6000) && (autoPin == HIGH)) {
    digitalWrite(nosRelayPin, HIGH);
  } else if ((coolantTemp < -100) && (afr < 12) && (throttlePos > 98) &&
             (armPin == HIGH) && (fireButtonPin == HIGH) && (4000 < rpm) &&
             (rpm < 7500)) {
    digitalWrite(nosRelayPin, HIGH);
  } else {
    digitalWrite(nosRelayPin, LOW);
  }

  if ((armPin == HIGH) && (purgeButtonPin == HIGH)) {
    digitalWrite(purgeRelayPin, HIGH);
  } else {
    digitalWrite(purgeRelayPin, LOW);
  }

}

void autoPurge() {
 if ((armPin == LOW) && (linePressurePin == HIGH) && (bottlePin == LOW)) {
   digitalWrite(purgeRelayPin, HIGH);
 } else {
   digitalWrite(purgeRelayPin, LOW);
 }

}

void purge() {
  if ((purgeButtonPin == HIGH) && (armPin == HIGH)) {
    digitalWrite(purgeRelayPin, HIGH);
  } else {
    digitalWrite(purgeRelayPin, LOW);
  }
}

void autoHeat() {
  if ((bottlePressureLowPin == LOW) && (linePressurePin == HIGH) &&
  (bottlePin == HIGH) && (armPin == HIGH)) {

    digitalWrite(heatPin, HIGH);
  } else {
    digitalWrite(heatPin, LOW);
  }
}

void printToScreen() {

}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial.println("Starting test mode...");

#ifdef DEBUG
#else
  obd.begin(); // Wait for ODB Coms
  while (!obd.init());
#endif

  pinMode(fireButtonPin, INPUT);
  pinMode(autoPin, INPUT);
  pinMode(armPin, INPUT);
  pinMode(purgeButtonPin, INPUT);
  pinMode(bottlePressureLowPin, INPUT);
  pinMode(bottlePressureHighPin, INPUT);
  pinMode(linePressurePin, INPUT);

  pinMode(thermostatRelayPin, OUTPUT);
  pinMode(purgeRelayPin, OUTPUT);
  pinMode(heatPin, OUTPUT);
  pinMode(bottlePin, OUTPUT);
  pinMode(nosRelayPin, OUTPUT);


  digitalWrite(thermostatRelayPin, LOW);
  digitalWrite(purgeRelayPin, LOW);
  digitalWrite(heatPin, LOW);
  digitalWrite(bottlePin, LOW);
  digitalWrite(nosRelayPin, LOW);
}

void loop() {
getData();
if (armPin == HIGH) {
  nosControl();
  autoHeat();
  }
printToScreen();
purge();
autoPurge();


}
