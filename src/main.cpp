
//By Trevor Gruszynski
#include <Arduino.h>
#include <OBD2UART.h>
#include <GLCD.h>

COBD obd;
LCD lcd;

bool testMode = true;
bool throttleReadyToFire = false;

int rpm = 6000;
int bottleTemp = 80;
int throttlePos = 100;
int afr = 11;
int coolantTemp; //Not nessassary
const int fireButtonPin = 1;
const int nosRelayPin = 2;
const int autoPin = 3;
const int armPin = 0;
const int purgeButtonPin = 5;
const int purgeRelayPin = 6;
const int heatPin = 7;
const int bottlePin = 8;
const int thermostatRelayPin = 9;

//TODO
// PurgeControl logic
// Bottle Heat logic
// Pedal Switch in sereis with NOS RElAY
// Screen Error

void getData(){
  obd.readPID(PID_RPM, rpm);
  obd.readPID(PID_THROTTLE, throttlePos);
  obd.readPID(PID_AIR_FUEL_EQUIV_RATIO, afr);
  obd.readPID(PID_COOLANT_TEMP, coolantTemp);

}

void nosOn(){
  if (testMode){
      Serial.println("NOS On");
  }else{
    digitalWrite(nosRelayPin, 1);
  }
  //Fun neopixle stuff
}
void nosOff(){
  if (testMode){
    Serial.println("NOS Off");
  }else{
    digitalWrite(nosRelayPin, 0);
  }
    //shut off neopixle
}

void nosControl(){
    if (throttlePos > 98){throttleReadyToFire=true;}


    if ((fireButtonPin == HIGH) && (armPin == HIGH) && (4000 < rpm) && (rpm<7500) && (throttleReadyToFire)) {
      digitalWrite(thermostatRelayPin, HIGH);
    }

    if ((rpm > 3000) && (armPin == HIGH)) {
      digitalWrite(bottlePin, HIGH);
    } else {
      digitalWrite(bottlePin, LOW);
    }

    if ((coolantTemp < -100) && (afr < 12) && (throttlePos > 98) && (armPin == HIGH) && (4000 < rpm) && (rpm < 6000) && (autoPin == HIGH)) {
      nosOn();
    } else if ((coolantTemp < -100) && (afr < 12) && (throttleReadyToFire) && (armPin == HIGH) && (fireButtonPin == HIGH) && (4000 < rpm) && (rpm < 7500)){
      nosOn();
    } else {
      nosOff();
    }

    if ((armPin == HIGH) && (purgeButtonPin == HIGH)) {
      digitalWrite(purgeRelayPin, HIGH);
    } else {
      digitalWrite(purgeRelayPin, LOW);
    }

    if ((bottleTemp > 90) && (armPin == HIGH)) {
        digitalWrite(purgeRelayPin, HIGH);
    }

    if ((bottleTemp < 80) && (armPin == HIGH)) {
      digitalWrite(heatPin, HIGH);
    }
}

void purgeControl(){
  //place holder for autoperge and manual purge logic
}

void purgeValveOn(){
  if (testMode){
    Serial.println("Purging");
  }else{
    digitalWrite(purgeRelayPin, HIGH);
  }
}
void purgeValveOff(){
  if (testMode){
    Serial.println("Purge Off");
  }else{
    digitalWrite(purgeRelayPin, LOW);
  }
}

void bottleHeaterOn(){
  if (testMode){
    Serial.println("Heater On");
  }else{
    digitalWrite(heatPin, HIGH);
  }
}
void bottleHeaterOff(){
  if (testMode){
    Serial.println("Heater Off");
  }else{
    digitalWrite(heatPin, LOW);
  }
}

void bottleControlSolonoidOn(){
  if (testMode){
    Serial.println("Bottle Control Solonoid On");
  }else{
    digitalWrite(bottlePin, HIGH);
  }
}
void bottleControlSolonoidOff(){
  if (testMode){
    Serial.println("Bottle Control Solonoid Off");
  }else{
    digitalWrite(bottlePin, LOW);
  }
}

void thermostatRelayOff(){
  if (testMode){
    Serial.println("thermostat Relay Off");
  }else{
    digitalWrite(thermostatRelayPin, LOW);
  }
}

void thermostatRelayOn(){
  if (testMode){
    Serial.println("thermostat Relay On");
  }else{
    digitalWrite(thermostatRelayPin, HIGH);
  }
}

void testFunction(){
  Serial.print("armPin...");         Serial.println(digitalRead(armPin));
  Serial.print("fireButtonPin...");      Serial.println(digitalRead(fireButtonPin));
  Serial.print("nosRelayPin...");         Serial.println(digitalRead(nosRelayPin));
  Serial.print("autoPin...");        Serial.println(digitalRead(autoPin));
  Serial.print("heatPin...");        Serial.println(digitalRead(heatPin));
  Serial.print("purgeRelayPin...");       Serial.println(digitalRead(purgeRelayPin));
  Serial.print("purgeButtonPin..."); Serial.println(digitalRead(purgeButtonPin));
  Serial.print("bottlePin...");      Serial.println(digitalRead(bottlePin));
}

void welcomeScreen(){
  lcd.clear();
  delay(60);
  lcd.print(0,0,"v----------------------------v");
  delay(60);
  lcd.print(1,0,"|       NOS CONTROL          |");
  delay(60);
  lcd.print(2,0,"|      The Shed 2019         |");
  delay(60);
  lcd.print(3,0,"|                            |");
  delay(60);
  lcd.print(4,0,"|                            |");
  delay(60);
  lcd.print(5,0,"| Hartford Race Team         |");
  delay(60);
  lcd.print(6,0,"|      Test Build      V01   |");
  delay(60);
if (testMode){
  lcd.print(7,0,"|  ***!!Test Mode On!!***    |");
  delay(60);
  lcd.print(8,0,"|     OUTPUTS   DISABLED     |");
}else{
  lcd.print(7,0,"|                            |");
  delay(60);
  lcd.print(8,0,"|                            |");
}
  lcd.print(9,0,"^----------------------------^");
  delay(3000);
}

void statScreen(){
  if (armPin == LOW){
  lcd.print(0,9,"NOS  Control");

  lcd.print(1,0,"NOS Status:");

//  if ((coolantTemp < -100) && (afr < 12) && (throttlePos > 98) && (armPin == HIGH) && (fireButtonPin == HIGH) && (4000 < rpm) && (rpm < 7500) && (bottlePin == HIGH)) {
  //  lcd.print(1,20,"READY");
//  }else if(nosRelayPin == 1){lcd.print(1,20,"**FIRING**");
//}else {lcd.print(1,20,"NOT READY");}

  lcd.print(2,0,"Bottle Temp:");

  lcd.print(3,0,"Throttle Position:");

  lcd.print(4,0,"Auto NOS:");
  //if (autoPin == HIGH) {
  //  lcd.print(4,20,"Disabled");
//  } else {
//    lcd.print(4,20,"Enabled");
//  }

  lcd.print(5,0,"Purge Status:");
//  if (purgeRelayPin == HIGH) {
  //  lcd.print(5,20,"PURGING");
  //}

  lcd.print(6,0,"RPM:                   " + (String(rpm)));
  // lcd.print(6,20,String(rpm));
   // print value



  lcd.print(7,0,"Coolant Temp:");



  lcd.print(8,7,"**SYSTEM ARMED**");
  lcd.print(9,7,"****************");
}else{//place holder for dope ass stat screen
  lcd.clear();
  lcd.print(0,0,"v----------------------------v");
  lcd.print(1,0,"|       NOS CONTROL          |");
  lcd.print(2,0,"|      The Shed 2019         |");
  lcd.print(3,0,"|                            |");
  lcd.print(4,0,"|                            |");
  lcd.print(5,0,"|          HRT               |");
  lcd.print(6,0,"|      Test Build      V01   |");
  lcd.print(7,0,"|  ***!!Test Mode On!!***    |");
  lcd.print(8,0,"|     OUTPUTS   DISABLED     |");
  lcd.print(9,0,"^----------------------------^");
}



}
void statsValues() {
  lcd.print(6,20,String(rpm));
}

void setup(){
  delay(1); //change to 5000 for production
  Serial.begin(9600);
  Serial.println("Starting test mode...");
  lcd.init();
  welcomeScreen();
  //add self check stuff here
  // we'll use the debug LED as output
  pinMode(13, OUTPUT);
  // start communication with OBD-II UART adapter
//  obd.begin(); //Wait for ODB Coms
//  while (!obd.init());

  pinMode(fireButtonPin, INPUT);
  pinMode(autoPin, INPUT);
  pinMode(armPin, INPUT);
  pinMode(purgeButtonPin, INPUT);

  pinMode(thermostatRelayPin, OUTPUT);
  pinMode(purgeRelayPin, OUTPUT);
  pinMode(heatPin, OUTPUT);
  pinMode(bottlePin, OUTPUT);
  pinMode(nosRelayPin, OUTPUT);

  bottleControlSolonoidOff();
  bottleHeaterOff();
  purgeValveOff();
  nosOff();
  thermostatRelayOff();
}

void loop(){
  for (int i = 0; i <= 255; i++) {
    rpm = i;
    statScreen();
}
 if (!testMode){
   getData();
 }else{
   Serial.println("-----------------START-------------------");
 }
 nosControl();

 purgeControl();
  if (testMode){
   testFunction();
   //delay(5000);
   Serial.println("----------------------END--------------------");
  }
}
