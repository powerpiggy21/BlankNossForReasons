
// By Trevor Gruszynski && Evan Gruszynski
#include <Arduino.h>
#include <OBD2UART.h>
#include <Nextion.h>
// for hardcoded debug stuff
#define DEBUG 1

// for softcoded print funcutions and to disable outputs
bool testMode = true;

COBD obd;



int rpm;
int bottleTemp = 80;
int throttlePos;
int afr = 11;
int coolantTemp;
int oilPressure;
bool bottlePressureLow;
bool bottlePressureHigh;
bool fuelPressureOK;


const int fireButtonPin = 13;
const int nosRelayPin = 14;
const int autoPin = 12;
const int armPin = 27;
const int purgeButtonPin = 26;
const int purgeRelayPin = 25;
const int heatPin = 33; // bottle heater relay pin
const int bottlePin = 32; // high pressure solonoid for bottle cut off
const int thermostatRelayPin = 35; // sets car thermostate to -255 F
const int bottlePressureHighPin = 34; // 1000 psig pressure switch
const int bottlePressureLowPin = 5; // 800 psig pressure switch
const int linePressurePin = 18; // 4 psig pressure switch
const int fuelPressure = 19; //fuel pressure to trigger when we go below 40 psi
const int oilPressurePin = 23; //
// TODO
// PurgeControl logic
// Bottle Heat logic
// Pedal Switch in sereis with NOS REl

void getData() {
  obd.readPID(PID_RPM, rpm);
  obd.readPID(PID_THROTTLE, throttlePos);
  obd.readPID(PID_AIR_FUEL_EQUIV_RATIO, afr);
  obd.readPID(PID_COOLANT_TEMP, coolantTemp);
  oilPressure=analogRead(oilPressurePin); //add the math to make this psi


  if(digitalRead(bottlePressureHigh)){
    bottlePressureHigh=1;
  }else{
    bottlePressureHigh=0;
  }

  if(digitalRead(bottlePressureLow)){
    bottlePressureLow=1;
  }else{
    bottlePressureLow=0;
  }
}

void nosControl(){
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

void nextonTerminatSequence(){
  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(0xFF);
}

void putDataToHMI(){
  //RPM
  Serial2.print("n0.val="); Serial2.print(rpm); nextonTerminatSequence();
  if (fuelPressureOK){Serial2.print("t103.txt=\"OK\"");}else{Serial2.print("t103.txt=\"LOW\"");}nextonTerminatSequence();

  if (bottlePressureHigh){
    Serial2.print("t100.txt=\"HIGH\"");
  }else if(bottlePressureLow){
    Serial2.print("t100.txt=\"LOW\"");
  }else{
    Serial2.print("t100.txt=\"OK\"");
  }
nextonTerminatSequence();

  Serial2.print("n3.val=");Serial2.print(throttlePos);nextonTerminatSequence();

  Serial2.print("j0.val=");Serial2.print(map(coolantTemp,0,300,0,100));nextonTerminatSequence();
  Serial2.print("j0.pco=");
  if(coolantTemp>=200){
    Serial2.print("RED");
  }else if((coolantTemp>=-100)&&(coolantTemp<=165)){
    Serial2.print("BLUE");
  }else{
    Serial2.print("1024");
  }
  nextonTerminatSequence();

  Serial2.print("j1.val=");Serial2.print(map(afr,0,15,0,100));nextonTerminatSequence();
  Serial2.print("n2.val=");Serial2.print(oilPressure);nextonTerminatSequence();
}

void testHMI(){
  rpm++; //force rpm test
  if (fuelPressureOK){ //forse fuelpressuretest
    fuelPressureOK=false;

  }else{
    fuelPressureOK=true;

  }
  Serial2.print("t255.txt=\"Test mode\""); nextonTerminatSequence();
  throttlePos++;
  if (throttlePos==100){throttlePos=0;}
  coolantTemp++;
  if (coolantTemp==300){coolantTemp=0;}

  afr++;
  if(afr==15){afr=0;}

  oilPressure++;
  if(oilPressure==200){
    oilPressure=0;
  }

}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting test mode...");

  Serial2.begin(9600);


  if(!testMode){
   while (!obd.init());
  }

  delay(2000);
  Serial2.print("page 1");
  nextonTerminatSequence();

  pinMode(fireButtonPin, INPUT);
  pinMode(autoPin, INPUT);
  pinMode(armPin, INPUT);
  pinMode(purgeButtonPin, INPUT);
  pinMode(bottlePressureLowPin, INPUT);
  pinMode(bottlePressureHighPin, INPUT);
  pinMode(linePressurePin, INPUT);
  pinMode(fuelPressure, INPUT);
  pinMode(oilPressurePin, INPUT);

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
  if (testMode){testHMI();}else{getData();}
  if (armPin == HIGH) {
    nosControl();
    autoHeat();
  }
  purge();
  autoPurge();
  putDataToHMI();
}
